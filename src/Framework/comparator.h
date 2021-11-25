#pragma once

/**
    This is a framework for the matching of values and colums (fields) of records of various types.
*/

#include <cstdint>
#include <string>
#include <functional>

namespace Framework {

    /**
        A base class, needed mostly to define the public setter.
    */

    class MatchString {

    public:

        MatchString(const std::string& matchString = "") :
            _matchString(matchString) {
        }

        /**
            This allows for a comparator to be reused.
        */
        void SetMatchString(const std::string& matchString) {
            _matchString = matchString;
        }

    protected:

        std::string _matchString;
    };

    /**
        The default value comparator template (does nothing by itsef).
    */
    template<class T>
    class ValueComparator : public MatchString {
    
    public:
    
        using value_type = T;
        using Base = MatchString;
    };

    /**
        The value comparator template oveload for the type 'long'.
    */
    template<> 
    class ValueComparator<long> : public MatchString {

    public:

        using value_type = long;
        using Base = MatchString;

        ValueComparator(const std::string& matchString = "") :
            Base(matchString) {
        }

        bool operator()(value_type value) const {
            long matchValue = std::stol(_matchString); // note: can throw
            return matchValue == value;
        }
    };

    /**
        Value comparator oveload for the type 'uint32_t'.
    */
    template<> 
    class ValueComparator<uint32_t> : public MatchString {

    public:

        using value_type = uint32_t;
        using Base = MatchString;

        ValueComparator(const std::string& matchString = "") :
            Base(matchString) {
        }

        bool operator()(value_type value) const {
            long matchValue = std::stoul(_matchString); // note: can throw
            return matchValue == value;
        }
    };

    /**
        The value comparator template oveload for the type 'std::string'.
    */
    template<> 
    class ValueComparator<std::string> : public MatchString {

    public:

        using value_type = std::string;
        using Base = MatchString;

        ValueComparator(const std::string& matchString = "") :
            Base(matchString) {
        }

        bool operator()(const value_type& value) const {
            return value.find(_matchString) != std::string::npos;
        }
    };

    /**
        A column comparator base class template.
    */
    template<class Record, class Column>
    class ColumnComparator : public ValueComparator<Column> {

    public:

        using Base = ValueComparator<Column>;

        ColumnComparator() = default;

        ColumnComparator(Column Record::* column, const std::string matchString = "") :
            Base(matchString),
            _column(column) {
        }

        ColumnComparator(const ColumnComparator&) = default;

        bool operator()(const Record& rec) const {
            return Base::operator()(rec.*_column);
        }

    protected:

        Column Record::* _column;
    };
}