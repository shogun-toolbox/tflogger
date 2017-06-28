#ifndef __RECORD_WRITER_H__
#define __RECORD_WRITER_H__

#include <ostream>
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
            RecordWriter(std::ostream& input);
            ~RecordWriter();

            bool writeRecord(const std::string& record);
        private:
            std::ostream& mOutputStream;
    };
}

#endif /* __RECORD_WRITER_H__ */
