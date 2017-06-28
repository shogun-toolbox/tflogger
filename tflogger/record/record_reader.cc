#include <tflogger/record/record_reader.h>
#include <tflogger/utils.h>

using namespace tflogger;

RecordReader::RecordReader(std::istream& input):
    mInputStream(input)
{
}

RecordReader::~RecordReader()
{
}

Status RecordReader::readRecord(std::string* record)
{
    static const size_t kHeaderSize = sizeof(uint64_t) + sizeof(uint32_t);
    static const size_t kFooterSize = sizeof(uint32_t);

    // Read header data.
    const uint64_t length = readRecordLength();

    // Read data
    std::string data;
    auto status = readChecksummed(length, &data);
    if (status != Status::OK)
        return status;

    record->resize(data.size());
    memmove(&(*record)[0], data.data(), data.size());
    return Status::OK;
}


Status RecordReader::readChecksummed(size_t length, std::string* data)
{
    const size_t expected = length + sizeof(uint32_t);
    data->resize(expected);
    mInputStream.read(&(data->front()), expected);
    
    if (mInputStream.fail())
        return Status::IOError;

    if (mInputStream.eof())
        return Status::EndOfFile;

    if (data->size() != expected)
        return Status::DataLoss;

    uint32_t masked_crc = utils::DecodeFixed32(data->data() + length);
    if (crc32c::Unmask(masked_crc) != crc32c::Value(data->data(), length)) {
        return Status::CorruptedData;
    }
    data->erase(length, sizeof(uint32_t));
    return Status::OK;
}

const uint64_t RecordReader::readRecordLength()
{
    std::string header;
    header.reserve(sizeof(uint64_t));
    auto status = readChecksummed(sizeof(uint64_t), &header);
    if (status != Status::OK)
        return status;
    return utils::DecodeFixed64(header.data());
}
