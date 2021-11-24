/**
    This is the original performance and unit test for the QB::FindMatchingRecords function.
    Note: see the UnitTests project for more unit tests.
*/

#include <cassert>
#include <chrono>
#include <iostream>
#include <ratio>
#include <tchar.h>

#include "qb.h"

using namespace QB;

/**
    Utility to populate a record collection
    prefix - prefix for the string value for every record
    numRecords - number of records to populate in the collection
*/
RecordCollection populateDummyData(const std::string& prefix, uint32_t numRecords)
{
    RecordCollection data;
    data.reserve(numRecords);
    for (uint32_t i = 0; i < numRecords; i++)
    {
        Record rec = { i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix };
        data.emplace_back(rec);
    }
    return data;
}

/**
    The test.
*/
int main(int argc, _TCHAR* argv[])
{
    using namespace std::chrono;

    // populate a bunch of data
    auto data = populateDummyData("testdata", 1000);

    // Delete a record.
    DeleteRecordByID(data, 123);
    auto emptyCollection = FindMatchingRecords(data, "column0", "123");
    assert(emptyCollection.empty());

    // Find a record that contains and measure the perf
    auto startTimer = steady_clock::now();
    auto filteredSet = FindMatchingRecords(data, "column1", "testdata500");
    auto filteredSet2 = FindMatchingRecords(data, "column2", "24");
    std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

    // make sure that the function is correct
    assert(filteredSet.size() == 1);
    return 0;
}