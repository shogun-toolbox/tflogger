#ifndef __TFLOGGER_STATUS_H__
#define __TFLOGGER_STATUS_H__

namespace tflogger
{
    enum Status
    {
        OK = 0,
        EndOfFile,
        IOError,
        DataLoss,
        CorruptedData
    };
}


#endif /* __TFLOGGER_STATUS_H__ */
