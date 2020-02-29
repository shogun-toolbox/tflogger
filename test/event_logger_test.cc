#include <catch2/catch.hpp>

#include <chrono>
#include <fstream>
#include <string>

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/message_differencer.h>

#include <tflogger/event_logger.h>
#include <tflogger/event_reader.h>
#include <tflogger/utils.h>

#include <tflogger/proto/event.pb.h>

using namespace tflogger;
using namespace google::protobuf::io;

extern bool decodeEvent(const std::string& data, tensorflow::Event* proto);

void writeEvent(
    EventLogger& logger, double time, int64_t step,
    const std::string& tag, float simple_value)
{
    tensorflow::Event e;
    e.set_wall_time(time);
    e.set_step(step);
    tensorflow::Summary::Value* summaryValue = e.mutable_summary()->add_value();
    summaryValue->set_tag(tag);
    summaryValue->set_simple_value(simple_value);
    logger.writeEvent(e);
}

TEST_CASE("EventLogger - default test", "[EventLogger][create/write]") {
    tflogger::EventLogger eventLogger("tf");


    SECTION("init() creates a file")
    {
        REQUIRE(eventLogger.init());

        std::string filename = eventLogger.filename();
        std::ifstream f(filename.c_str());
        REQUIRE(f.good());
    }

    SECTION("serializing Events in a correct format")
    {
        auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        writeEvent(eventLogger, 1111, 1, "tag0", 42.0f);
        writeEvent(eventLogger, 2222, 2, "tag1", 16.01f);
        REQUIRE(eventLogger.close());

        tflogger::EventReader er(eventLogger.filename().c_str());

        REQUIRE(er.isCompatible() == Status::OK);

        tensorflow::Event actual;
        tensorflow::Event expected;
        REQUIRE(er.readEvent(&actual) == Status::OK);
        REQUIRE(google::protobuf::TextFormat::ParseFromString(
            "wall_time: 1111 step: 1 "
            "summary { value { tag: 'tag0' simple_value: 42.0 } }",
            &expected));
        REQUIRE(google::protobuf::util::MessageDifferencer::Equals(actual, expected));

        REQUIRE(er.readEvent(&actual) == Status::OK);
        REQUIRE(google::protobuf::TextFormat::ParseFromString(
            "wall_time: 2222 step: 2 "
            "summary { value { tag: 'tag1' simple_value: 16.01 } }",
            &expected));
        REQUIRE(google::protobuf::util::MessageDifferencer::Equals(actual, expected));
    }
}
