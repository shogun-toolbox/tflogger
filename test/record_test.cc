#include <catch2/catch.hpp>

#include <sstream>

#include <tflogger/record/record_reader.h>
#include <tflogger/record/record_writer.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace tflogger;
using namespace google::protobuf::io;

TEST_CASE("Record", "[Record][readRecord/writeRecord]")
{
	std::stringstream recordStream;
	{
		auto ostr = std::make_shared<OstreamOutputStream>(&recordStream);
		RecordWriter rw(ostr);

		REQUIRE(rw.writeRecord("foo"));
		REQUIRE(rw.writeRecord("dead"));
	}
	recordStream.flush();
	{
		auto istr = std::make_shared<IstreamInputStream>(&recordStream);
		RecordReader rr(istr);
		std::string record;
		REQUIRE(rr.readRecord(&record) == Status::OK);
		REQUIRE_THAT(record, Catch::Equals("foo"));
		REQUIRE(rr.readRecord(&record) == Status::OK);
		REQUIRE_THAT(record, Catch::Equals("dead"));
	}
}
