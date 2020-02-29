#include "tflogger/event_reader.h"
#include "tflogger/utils.h"
#include "tflogger/proto/event.pb.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>


using namespace tflogger;
using namespace google::protobuf::io;

EventReader::EventReader(const char* filename):
    mFilename(filename)
{
}

Status EventReader::isCompatible()
{
    if (mInputStream.get() != nullptr && !mFilename.empty())
    {
        return Status::OK;
    }

    int fd = open(mFilename.c_str(), O_RDONLY);
    if (fd < 0)
        return Status::IOError;

    mInputStream = std::make_shared<FileInputStream>(fd);
    mInputStream->SetCloseOnDelete(true);
    mRecordReader.reset(new RecordReader(mInputStream));

    tensorflow::Event event;
    auto status = readEvent(&event);
    if (status != Status::OK)
        return status;
    auto fileVersion = event.file_version();
    // check version
    if (fileVersion != utils::supportedFileVersion())
    {
        mRecordReader.reset(nullptr);
        mInputStream = nullptr;
        close(fd);
        return Status::InvalidVersion;
    }
    return status;
}

Status EventReader::readEventStr(std::string* eventStr)
{
    Status s;
    if (mInputStream.get() == nullptr)
    {
        if ((s = isCompatible()) != Status::OK)
        {
            std::cerr << "Read failed because file is not compatible with reader.";
            return s;
        }
    }
    return mRecordReader->readRecord(eventStr);
}

Status EventReader::readEvent(tensorflow::Event* e)
{
    std::string eventStr;
    auto s = readEventStr(&eventStr);
    if (s != Status::OK)
        return s;
    e->ParseFromString(eventStr);
    return s;
}
