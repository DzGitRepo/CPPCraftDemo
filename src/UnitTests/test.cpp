/**
	This is a collection of sample unit tests for the QB library.
*/

#include "gtest/gtest.h"

#include <stdexcept>

#include "qb.h"

using namespace QB;

/**
	Test the deleting of an existing record.
*/
TEST(DeleteRecordByID, ExistingRecord) {
	RecordCollection records;
	constexpr auto N = 10;
	for (auto i = 1; i <= N; ++i) {
		Record rec{ i, std::to_string(i), i, "" };
		records.emplace_back(std::move(rec));
	}
	DeleteRecordByID(records, N / 2 + 1);
	EXPECT_EQ(records.size(), N - 1);
}

/**
	Test the deleting of a non-existing record.
*/
TEST(DeleteRecordByID, MissingRecord) {
	RecordCollection records;
	constexpr auto N = 10;
	for (auto i = 1; i <= N; ++i) {
		Record rec{ i, std::to_string(i), i, "" };
		records.emplace_back(std::move(rec));
	}
	DeleteRecordByID(records, N + 1);
	EXPECT_EQ(records.size(), N);
}

/**
	Test the search for existing records.
*/
TEST(FindMatchingRecords, ExistingRecordsByString) {
	RecordCollection records;
	constexpr auto N = 100;
	for (auto i = 1; i <= N; ++i) {
		Record rec{ i, std::to_string(i % 10), i, "" };
		records.emplace_back(std::move(rec));
	}
	auto result = FindMatchingRecords(records, "column1", "5");
	EXPECT_EQ(result.size(), N / 10);
}

/**
	Test the search for records that do not exist.
*/
TEST(FindMatchingRecords, MissingRecordsByLong) {
	RecordCollection records;
	constexpr auto N = 100;
	for (auto i = 1; i <= N; ++i) {
		Record rec{ i, std::to_string(i % 10), i, "" };
		records.emplace_back(std::move(rec));
	}
	auto result = FindMatchingRecords(records, "column2", std::to_string(N + 1));
	EXPECT_TRUE(result.empty());
}

/**
	Test the search by a column that does not exist or not supported.
*/
TEST(FindMatchingRecords, InvalidColumn) {
	RecordCollection records;
	constexpr auto N = 10;
	for (auto i = 1; i <= N; ++i) {
		Record rec{ i, std::to_string(i), i, "" };
		records.emplace_back(std::move(rec));
	}
	bool error = false;
	try {
		auto result = FindMatchingRecords(records, "column", "xyz");
	}
	catch (std::runtime_error exc) {
		error = true;
	}
	EXPECT_TRUE(error);
}