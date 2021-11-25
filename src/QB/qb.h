#pragma once

/**
    This defines the structure and the API of a simple database table (unmodified).
*/

#include <cstdint>
#include <string>
#include <vector>

namespace QB {

    /**
        Represents a Record Object
    */
    struct Record
    {
        uint32_t column0; // unique id column
        std::string column1;
        long column2;
        std::string column3;
    };

    /**
        Represents a Record Collection.
        Note: could be split into two representations:
        - one as a std::map (with column0 being the key) for the table,
        - and another one kept as std::vector for the result set (a snapshot).
    */
    typedef std::vector<Record> RecordCollection;

    /**
        Return records that contains a string in the StringValue field
        records - the initial set of records to filter
        matchString - the string to search for

        Note: the empty match string matches all.
    */
    RecordCollection FindMatchingRecords(const RecordCollection& records, const std::string& columnName, const std::string& matchString);

    /**
        Deletes the record with the given unique id, if it exists; otherwise does nothing.
    */
    void DeleteRecordByID(RecordCollection& records, uint32_t id);
}