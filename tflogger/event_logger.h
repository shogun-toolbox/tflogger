#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__

#include <memory>
#include <string>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <tflogger/record/record_writer.h>

namespace tensorflow
{
    class Event;
}

namespace tflogger
{
    /**
     * Writes TensorFlow's Event into a file
     */
    class EventLogger
    {
        public:
            explicit EventLogger(const char* prefix);
            ~EventLogger();

            void writeEvent(const tensorflow::Event& event);
            void writeSerializedEvent(const std::string& eventStr);
            void writeSerializedEvent(const char* eventStr, size_t len);

            bool init();
            bool close();

            std::string filename();

        private:
            std::string generateFilename(const int64_t& nowSeconds) const;

        private:
            const std::string mPrefix;
            std::string mFilename;

            std::unique_ptr<RecordWriter> mRecordWriter;
            std::shared_ptr<google::protobuf::io::FileOutputStream> mOutputStream;
    };

}

#endif /* __EVENT_LOGGER_H__ */
