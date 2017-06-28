#include <tflogger/event_logger.h>
#include <tflogger/utils.h>

#include <chrono>
#include <iostream>
#include <strstream>

using namespace tflogger;

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

bool EventLogger::flush()
{
    if (mOutputStream.get() == nullptr)
        return false;

    mOutputStream->flush();
    return true;
}

bool EventLogger::close()
{
    auto r = flush();
    if (mOutputStream.get() != nullptr)
    {
        mOutputStream->close();
        if (mOutputStream->fail())
            r = false;
        mRecordWriter.reset(nullptr);
        mOutputStream.reset(nullptr);
    }
    return r;
}

std::string EventLogger::fileName() {
    if (mFileName.empty())
        init();

    return mFileName;
}

bool EventLogger::init()
{
    if (mOutputStream.get() != nullptr && !mFileName.empty())
    {
        return true;
    }

    auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    mFileName = generateFileName(nowSeconds);
    mOutputStream = std::unique_ptr<std::ofstream>(new std::ofstream(mFileName, std::ios::binary));
    if (!mOutputStream->is_open())
    {
        mOutputStream.reset(nullptr);
        return false;
    }
    mRecordWriter.reset(new RecordWriter(*mOutputStream.get()));
    if (mRecordWriter.get() == nullptr)
    {
        return false;
    }

    // Write the first event with the current version
    tensorflow::Event event;
    event.set_wall_time(nowSeconds);
    std::ostrstream fileVersion;
    fileVersion << kVersionPrefix << kCurrentVersion << std::ends;
    event.set_file_version(fileVersion.str());
    fileVersion.freeze(false);
    writeEvent(event);
    flush();

    return true;
}

std::string EventLogger::generateFileName(const int64_t& nowSeconds) const
{
    const char *fmt = "%s.out.tfevents.%010lld.%s";
    auto hostName = utils::Hostname();
    auto sz = std::snprintf(nullptr, 0, fmt, mPrefix.c_str(), nowSeconds, hostName.c_str());
    std::vector<char> buf(sz + 1);
    std::snprintf(&buf[0], buf.size(), fmt, mPrefix.c_str(), nowSeconds, hostName.c_str());
    return std::string(buf.data(), buf.size());
}
