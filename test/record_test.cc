#include <catch.hpp>

#include <sstream>

#include <tflogger/record/record_reader.h>
#include <tflogger/record/record_writer.h>

using namespace tflogger;

TEST_CASE("Record", "[Record][readRecord/writeRecord]")
{
	std::stringstream recordStream;
	RecordWriter rw(recordStream);

	REQUIRE(rw.writeRecord("foo"));
	REQUIRE(rw.writeRecord("dead"));
	recordStream.flush();

	RecordReader rr(recordStream);
	std::string record;
	REQUIRE(rr.readRecord(&record) == Status::OK);
	REQUIRE_THAT(record, Catch::Equals("foo"));
	REQUIRE(rr.readRecord(&record) == Status::OK);
	REQUIRE_THAT(record, Catch::Equals("dead"));
}
