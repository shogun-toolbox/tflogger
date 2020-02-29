#ifndef __TFLOGGER_EVENT_READER_H__
#define __TFLOGGER_EVENT_READER_H__

#include <memory>
#include <string>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <tflogger/record/record_reader.h>

namespace tensorflow
{
    class Event;
}

namespace tflogger
{
    /**
     * Writes TensorFlow's Event into a file
     */
    class EventReader
    {
        public:
            explicit EventReader(const char* filename);
            ~EventReader() = default;

            Status isCompatible();
            Status readEvent(tensorflow::Event*);
            Status readEventStr(std::string*);

        private:
            std::string mFilename;
            std::unique_ptr<RecordReader> mRecordReader;
            std::shared_ptr<google::protobuf::io::FileInputStream> mInputStream;
    };

}

#endif /* __TFLOGGER_EVENT_READER_H__ */
