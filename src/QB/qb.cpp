/*
    This is an implementation of the API of a simple database table.
    Note:   can switch between the new and the original implementation below; 
            when tested on 1M records, the new one was shown to be about 4x faster
            when built in the Release mode. 
*/

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <stdexcept>

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

    using RecordFilter = std::function<bool(const Record&)>;

    class ColumnFilter {

    public:

        ColumnFilter() = default;

        ColumnFilter(int columnIndex) :
            _columnIndex(columnIndex) {
        }

        RecordFilter GetRecordFilter(const std::string& matchString) const {
            switch (_columnIndex) {
            case 0: return ColumnComparator<decltype(Record::column0)>(&Record::column0, matchString);
            case 1: return ColumnComparator<decltype(Record::column1)>(&Record::column1, matchString);
            case 2: return ColumnComparator<decltype(Record::column2)>(&Record::column2, matchString);
            case 3: return ColumnComparator<decltype(Record::column3)>(&Record::column3, matchString);
            default: return [](const Record&) { return false; };
            }
        }

    private:
        
        int _columnIndex{};
    };

    static std::unordered_map<std::string, ColumnFilter> column_filter{
        { "column0", ColumnFilter(0)},
        { "column1", ColumnFilter(1)},
        { "column2", ColumnFilter(2)},
        { "column3", ColumnFilter(3)}
    };

    /**
        Return records that contains a string in the StringValue field
        records - the initial set of records to filter
        matchString - the string to search for

        Note: the empty match string matches all.
    */
    RecordCollection FindMatchingRecords(const RecordCollection& records, const std::string& columnName, const std::string& matchString)
    {
#if NEW_IMPL

        // Obtain the appropriate column filter.
        auto it = column_filter.find(columnName);
        if (it == std::end(column_filter))
            throw std::runtime_error(std::string(__func__) + ": invalid column name");
        auto& column { it->second };
        auto filter { column.GetRecordFilter(matchString) };

        // Filter the records.
        RecordCollection result;
        std::copy_if(std::begin(records), std::end(records), std::back_inserter(result), filter);

        return result; // note: the copy elision (RVO) is at worke here

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