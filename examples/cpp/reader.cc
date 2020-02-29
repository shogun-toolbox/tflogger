#include <iostream>

#include <tflogger/event_reader.h>
#include <tflogger/proto/event.pb.h>

#include <google/protobuf/text_format.h>

using namespace tflogger;

int main(int argc, char** argv)
{
    EventReader er(argv[1]);
    std::cout << er.isCompatible() << "\n";

    tensorflow::Event e;
    while (er.readEvent(&e) == Status::OK) {
        std::string s;
        if (google::protobuf::TextFormat::PrintToString(e, &s)) {
            std::cout << "Your message: " << s;
        } else {
          std::cerr << "Message not valid (partial content: "
            << e.ShortDebugString() << ")\n";
        }
    }
    std::cout << "fin\n";

    return 0;
}