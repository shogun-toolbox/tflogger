%module tflogger

%include <std_string.i>

%{
#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl.h>
%}

%typemap(in) const std::shared_ptr<google::protobuf::io::ZeroCopyInputStream>& (std::shared_ptr<google::protobuf::io::FileInputStream> fstream=NULL) {
  int fd = -1;

  fd = PyObject_AsFileDescriptor($input);
  if (fd < 0) {
    SWIG_Error(SWIG_TypeError, "File object expected.");
    SWIG_fail;
  } else {
    fstream = std::make_shared<google::protobuf::io::FileInputStream>(fd);
    $1 = new std::shared_ptr<google::protobuf::io::ZeroCopyInputStream>(fstream);
  }
}

%typemap(freearg) std::shared_ptr<google::protobuf::io::ZeroCopyInputStream>& {
  delete $1;
}

%typemap(in) const char* eventStr {
  if (PyBytes_Check($input)) {
    $1 = PyBytes_AsString($input);
  } else {
    PyErr_SetString(PyExc_ValueError, "Expecting a byte array (bytes)");
    SWIG_fail;
  }
}

%{
    #include <tflogger/event_logger.h>
    #include <tflogger/event_reader.h>
%}

%ignore tflogger::EventLogger::writeEvent;
%ignore tflogger::EventLogger::writeSerializedEvent(const std::string& eventStr);

%pythoncode %{
from .pb import Event
%}

%include <tflogger/event_logger.h>

%extend tflogger::EventLogger {
  %pythoncode %{
    def write_event(self, event: Event):
      self.writeSerializedEvent(event.SerializeToString(), event.ByteSize())
  %}
}

%typemap(in, numinputs=0) std::string* (std::string temp){
  $1 = &temp;
}

%typemap(argout) std::string* {
  PyObject *status, *outStr;
  if (!PyTuple_Check($result)) {
    PyObject *status = $result;
    $result = PyTuple_New(1);
    PyTuple_SetItem($result, 0, status);
  }
  outStr = PyTuple_New(1);
  PyTuple_SetItem(outStr, 0, PyBytes_FromStringAndSize($1->data(), $1->size()));
  status = $result;
  $result = PySequence_Concat(status, outStr);
  Py_DECREF(status);
  Py_DECREF(outStr);
}

%include <tflogger/status.h>

%ignore tflogger::EventReader::readEvent;

%include <tflogger/event_reader.h>

%extend tflogger::EventReader {
  %pythoncode %{
    def read_event(self) -> Event:
        status, eventStr = self.readEventStr()
        e = None
        if status == OK:
          e = Event.FromString(eventStr)
        return status, e
    %}
}
