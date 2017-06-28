#include <tflogger/record/record_writer.h>
#include <tflogger/utils.h>

using namespace tflogger;

RecordWriter::RecordWriter(std::ostream& output):
    mOutputStream(output)
{
}

RecordWriter::~RecordWriter()
{
}


bool RecordWriter::writeRecord(const std::string& record)
{
    char header[sizeof(uint64_t) + sizeof(uint32_t)];
    utils::EncodeFixed64(header + 0, record.size());
    utils::EncodeFixed32(header + sizeof(uint64_t), utils::MaskedCrc(header, sizeof(uint64_t)));
    char footer[sizeof(uint32_t)];
    utils::EncodeFixed32(footer, utils::MaskedCrc(record.data(), record.size()));

    mOutputStream.write(header, sizeof(header));
    mOutputStream.write(record.data(), record.size());
    mOutputStream.write(footer, sizeof(footer));
    return !mOutputStream.fail();
}

