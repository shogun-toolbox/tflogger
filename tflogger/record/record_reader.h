#ifndef __RECORD_READER_H__
#define __RECORD_READER_H__

#include <istream>
#include <string>

#include <tflogger/status.h>

namespace tflogger
{
    class RecordReader
    {
        public:
            RecordReader(std::istream& input);
            ~RecordReader();

            Status readRecord(std::string* record);

        private:
            const uint64_t readRecordLength();
            Status readChecksummed(size_t length, std::string* data);

        private:
            std::istream& mInputStream;
    };
}

#endif /* __RECORD_READER_H__ */
