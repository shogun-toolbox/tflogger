#ifndef __RECORD_READER_H__
#define __RECORD_READER_H__

#include <memory>
#include <string>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include "tflogger/status.h"

namespace tflogger
{
    class RecordReader
    {
        public:
            RecordReader(const std::shared_ptr<google::protobuf::io::ZeroCopyInputStream>& input);
            ~RecordReader() = default;

            Status readRecord(std::string* record) const;

        private:
            const uint64_t readRecordLength() const;
            Status readChecksummed(size_t length, std::string* data) const;

        private:
            std::shared_ptr<google::protobuf::io::ZeroCopyInputStream> mZeroIn;
            std::unique_ptr<google::protobuf::io::CodedInputStream> mInputStream;
    };
}

#endif /* __RECORD_READER_H__ */
