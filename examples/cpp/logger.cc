#include <iostream>

#include <tflogger/event_logger.h>
#include <tflogger/proto/event.pb.h>

using namespace tflogger;
using namespace google::protobuf::io;

int main(int argc, char** argv)
{
    tensorflow::Event e;
    e.set_wall_time(1111);
    e.set_step(1);
    tensorflow::Summary::Value* summaryValue = e.mutable_summary()->add_value();
    summaryValue->set_tag("tag0");
    summaryValue->set_simple_value(11.0);

    EventLogger el("simple");
    el.writeEvent(e);

    std::cout << "fin\n";

    return 0;
}