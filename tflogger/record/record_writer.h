#ifndef __RECORD_WRITER_H__
#define __RECORD_WRITER_H__

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <memory>
#include <string>

namespace tflogger
{
    /**
     * write TensorFlow Record format
     * Format of a single record:
     *  uint64    length
     *  uint32    masked crc of length
     *  byte      data[length]
     *  uint32    masked crc of data
     */
    class RecordWriter
    {
        public:
            RecordWriter(const std::shared_ptr<google::protobuf::io::ZeroCopyOutputStream>& input);
            ~RecordWriter() = default;

            bool writeRecord(const std::string& record) const;
            bool writeRecord(const char* record, const uint64_t len) const;

        private:
            bool writeHeader(const uint64_t recordSize) const;
            void writeFooter(const uint32_t maskedCrc) const;
        private:
            std::shared_ptr<google::protobuf::io::ZeroCopyOutputStream> mZeroOut;
            std::unique_ptr<google::protobuf::io::CodedOutputStream> mOutputStream;
    };
}

#endif /* __RECORD_WRITER_H__ */
