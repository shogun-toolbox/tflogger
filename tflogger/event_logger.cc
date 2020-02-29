#include "tflogger/event_logger.h"
#include "tflogger/utils.h"
#include "tflogger/proto/event.pb.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>



using namespace tflogger;
using namespace google::protobuf::io;

EventLogger::EventLogger(const char* prefix):
    mPrefix(prefix)
{
}

EventLogger::~EventLogger()
{
    close();
}

void EventLogger::writeEvent(const tensorflow::Event& event)
{
    std::string record;
    event.AppendToString(&record);
    writeSerializedEvent(record);
}

void EventLogger::writeSerializedEvent(const char* eventStr, size_t len)
{
    if (mOutputStream.get() == nullptr)
    {
        if (!init())
        {
            std::cerr << "Write failed because file could not be opened.";
            return;
        }
    }
    mRecordWriter->writeRecord(eventStr, len);
}

void EventLogger::writeSerializedEvent(const std::string& eventStr)
{
    if (mOutputStream.get() == nullptr)
    {
        if (!init())
        {
            std::cerr << "Write failed because file could not be opened.";
            return;
        }
    }
    mRecordWriter->writeRecord(eventStr);
}

bool EventLogger::close()
{
    bool r = true;
    if (mOutputStream.get() != nullptr)
    {
        mRecordWriter.reset(nullptr);
        r &= mOutputStream->Flush();
        r &= mOutputStream->Close();
        mOutputStream = nullptr;
    }
    return r;
}

std::string EventLogger::filename() {
    if (mFilename.empty())
        init();

    return mFilename;
}

bool EventLogger::init()
{
    if (mOutputStream.get() != nullptr && !mFilename.empty())
    {
        return true;
    }

    auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    mFilename = generateFilename(nowSeconds);
    int fd = open(mFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0)
        return false;
    mOutputStream = std::make_shared<FileOutputStream>(fd);
    mRecordWriter.reset(new RecordWriter(mOutputStream));
    if (mRecordWriter.get() == nullptr)
    {
        return false;
    }

    // Write the first event with the current version
    tensorflow::Event event;
    event.set_wall_time(nowSeconds);
    event.set_file_version(utils::supportedFileVersion());
    writeEvent(event);

    return true;
}

std::string EventLogger::generateFilename(const int64_t& nowSeconds) const
{
    const char *fmt = "%s.out.tfevents.%010lld.%s";
    auto hostName = utils::Hostname();
    auto sz = std::snprintf(nullptr, 0, fmt, mPrefix.c_str(), nowSeconds, hostName.c_str());
    std::vector<char> buf(sz + 1);
    std::snprintf(&buf[0], buf.size(), fmt, mPrefix.c_str(), nowSeconds, hostName.c_str());
    return std::string(buf.data(), buf.size());
}
