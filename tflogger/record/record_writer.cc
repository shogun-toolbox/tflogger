#include "tflogger/record/record_writer.h"
#include "tflogger/utils.h"

using namespace tflogger;
using namespace google::protobuf::io;

RecordWriter::RecordWriter(const std::shared_ptr<ZeroCopyOutputStream>& output):
    mZeroOut(output)
{
    mOutputStream.reset(new CodedOutputStream(mZeroOut.get()));
}

bool RecordWriter::writeRecord(const char* record, const uint64_t len) const
{
    if (!writeHeader(len))
        return false;
    mOutputStream->WriteRaw(record, len);
    writeFooter(utils::MaskedCrc(record, len));
    return !mOutputStream->HadError();
}

bool RecordWriter::writeRecord(const std::string& record) const
{
    if (!writeHeader(record.size()))
        return false;
    mOutputStream->WriteString(record);
    writeFooter(utils::MaskedCrc(record.data(), record.size()));
    return !mOutputStream->HadError();
}

bool RecordWriter::writeHeader(const uint64_t recordSize) const
{
    uint8_t* buffer =
        mOutputStream->GetDirectBufferForNBytesAndAdvance(sizeof(uint64_t) + sizeof(uint32_t));
    if (buffer != nullptr)
    {
        buffer = CodedOutputStream::WriteLittleEndian64ToArray(recordSize, buffer);
        CodedOutputStream::WriteLittleEndian32ToArray(utils::MaskedCrc(
            (char*)buffer-sizeof(uint64_t), sizeof(uint64_t)), buffer);
        return true;
    }
    return false;
}

void RecordWriter::writeFooter(const uint32_t maskedCrc) const
{
    mOutputStream->WriteLittleEndian32(maskedCrc);
}
