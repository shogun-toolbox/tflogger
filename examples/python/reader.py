import sys
import tflogger as tfl

reader = tfl.EventReader(sys.argv[1])
status = reader.isCompatible()
while status == tfl.OK:
    status, e = reader.read_event()
    print(status, e)
