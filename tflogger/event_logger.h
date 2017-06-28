#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__

#include <fstream>
#include <string>

#include <tflogger/record/record_writer.h>
#include "event.pb.h"

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

            bool init();
            bool flush();
            bool close();

            std::string fileName();

        private:
            std::string generateFileName(const int64_t& nowSeconds) const;

        private:
            const std::string mPrefix;
            std::string mFileName;

            std::unique_ptr<RecordWriter> mRecordWriter;
            std::unique_ptr<std::ofstream> mOutputStream;
    };
    
    // Prefix of version string present in the first entry of every event file.
    static constexpr const char* kVersionPrefix = "brain.Event:";
    static constexpr const int kCurrentVersion = 2;
}

#endif /* __EVENT_LOGGER_H__ */
