#include "tflogger/record/record_reader.h"
#include "tflogger/utils.h"

using namespace tflogger;
using namespace google::protobuf::io;

RecordReader::RecordReader(const std::shared_ptr<ZeroCopyInputStream>& input): mZeroIn(input)
{
    mInputStream.reset(new CodedInputStream(mZeroIn.get()));
}

Status RecordReader::readRecord(std::string* record) const
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


Status RecordReader::readChecksummed(size_t length, std::string* data) const
{
    //data->resize(length);
    if (!mInputStream->ReadString(data, length))
        return Status::IOError;

    if (mInputStream->ExpectAtEnd())
        return Status::EndOfFile;

    if (data->size() != length)
        return Status::DataLoss;

    uint32_t masked_crc;
    if (!mInputStream->ReadLittleEndian32(&masked_crc))
        return Status::IOError;
    if (crc32c::Unmask(masked_crc) != crc32c::Value(data->data(), length)) {
        return Status::CorruptedData;
    }
    return Status::OK;
}

const uint64_t RecordReader::readRecordLength() const
{
    std::string header;
    header.reserve(sizeof(uint64_t));
    auto status = readChecksummed(sizeof(uint64_t), &header);
    if (status != Status::OK)
        return status;
    uint64_t recordLength;
    CodedInputStream::ReadLittleEndian64FromArray((const uint8_t*)header.data(), &recordLength);
    return recordLength;
}
