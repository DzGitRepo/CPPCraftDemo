/*
    This is an implementation of the API of a simple database table.
    Note:   can switch between the new and the original implementation below; 
            when tested on 1M records, the new one is about 8x faster in Debug, 
            and about 35% faster in Release.
*/

#include <algorithm>
#include <iterator>

#include "qb.h"
#include "comparator.h"

#ifndef NEW_IMPL
#define NEW_IMPL true
#endif

namespace QB {

    /**
        The column comparator for the Record type.
    */
    template<class Column>
    using ColumnComparator = Framework::ColumnComparator<Record, Column>;

    /**
        Return records that contains a string in the StringValue field
        records - the initial set of records to filter
        matchString - the string to search for
    */
    RecordCollection FindMatchingRecords(const RecordCollection& records, const std::string& columnName, const std::string& matchString)
    {
#if NEW_IMPL

        std::function<bool(const Record&)> filter;

        // Select the filter.
        if (columnName == "column0")
            filter = ColumnComparator<decltype(Record::column0)>(&Record::column0, matchString);
        else if (columnName == "column1")
            filter = ColumnComparator<decltype(Record::column1)>(&Record::column1, matchString);
        else if (columnName == "column2")
            filter = ColumnComparator<decltype(Record::column2)>(&Record::column2, matchString);
        else if (columnName == "column3")
            filter = ColumnComparator<decltype(Record::column3)>(&Record::column3, matchString);
        else
            filter = [](const Record&) {
            return false;
        };

        // Filter the records.
        RecordCollection result;
        std::copy_if(records.begin(), records.end(), std::back_inserter(result), filter);

        return result; // note: copy elision (RVO) is at worke here

#else // the original version

        RecordCollection result;
        std::copy_if(records.begin(), records.end(), std::back_inserter(result), [&](Record rec) {
            if (columnName == "column0") {
                uint32_t matchValue = std::stoul(matchString);
                return matchValue == rec.column0;
            }
            else if (columnName == "column1") {
                return rec.column1.find(matchString) != std::string::npos;
            }
            else if (columnName == "column2") {
                long matchValue = std::stol(matchString);
                return matchValue == rec.column2;
            }
            else if (columnName == "column3") {
                return rec.column3.find(matchString) != std::string::npos;
            }
            else {
                return false;
            }
            });
        return result;
#endif
    }

    /**
        Deletes the record with the given unique id, if it exists; otherwise does nothing.
        Note: the implementation does rely on the assumption of uniqueness.
    */
    void DeleteRecordByID(RecordCollection& records, uint32_t id) {

        auto it = std::find_if(std::begin(records), std::end(records), [id](const Record& rec) {
            return rec.column0 == id;
            });

        if (it != std::end(records))
            records.erase(it);
    }
}