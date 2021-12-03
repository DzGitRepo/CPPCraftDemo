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

    template<class T>
    class MatchValue {

    public:

        MatchValue(std::function<T(std::string)> conv, const std::string& matchString = "") :
            _matchValue(conv(matchString)),
            _conv(conv) {
        }

        /**
            This allows for a comparator to be reused.
        */
        void SetMatchValue(const std::string& matchString) {
            _matchValue = _conv(matchString);
        }

    protected:

        T _matchValue;

    private:

        std::function<T(std::string)> _conv;
    };

    /**
        The default value comparator template (does nothing by itsef).
    */
    template<class T>
    class ValueComparator : public MatchValue<T> {
    
    public:
    
        using value_type = T;
        using Base = MatchValue<value_type>;
    };

    /**
        The value comparator template oveload for the type 'long'.
    */
    template<> 
    class ValueComparator<long> : public MatchValue<long> {

    public:

        using value_type = long;
        using Base = MatchValue<value_type>;

        ValueComparator(const std::string& matchString = "") :
            Base([](const std::string& matchString) { return std::stol(matchString); }, matchString) {
        }

        bool operator()(value_type value) const {
            return _matchValue == value;
        }
    };

    /**
        Value comparator oveload for the type 'uint32_t'.
    */
    template<> 
    class ValueComparator<uint32_t> : public MatchValue<uint32_t> {

    public:

        using value_type = uint32_t;
        using Base = MatchValue<value_type>;

        ValueComparator(const std::string& matchString = "") :
            Base([](const std::string& matchString) { return std::stoul(matchString); }, matchString) {
        }

        bool operator()(value_type value) const {
            return _matchValue == value;
        }
    };

    /**
        The value comparator template oveload for the type 'std::string'.
    */
    template<> 
    class ValueComparator<std::string> : public MatchValue<std::string> {

    public:

        using value_type = std::string;
        using Base = MatchValue<value_type>;

        ValueComparator(const std::string& matchString = "") :
            Base([](const std::string& matchString) { return matchString; }, matchString) {
        }

        bool operator()(const value_type& value) const {
            return value.find(_matchValue) != std::string::npos;
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