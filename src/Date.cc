#include <boost/regex.hpp>

#include "Date.h"

/////////////////////////////////////////
// Definition of the class Date
//////////////////////////////////////////
Date::Date(std::string& str, enum DATETYPE type):
    _type(type)
{
}

Date::~Date()
{
}

Date Date::today(Date::DATETYPE type)
{
    return *(static_cast<Date *>(NULL));
}

Date Date::nWorkdaysAfter(int days) const
{
    return *(static_cast<Date *>(NULL));
}

Date Date::nWorkdaysBefore(int days) const
{
    return *(static_cast<Date *>(NULL));
}

bool Date::isWorkday(Date& date) const
{
    return true;
}

int Date::operator-(Date& rhs) const
{
    return 0;
}

const Date& Date::operator+(std::string& durationStr) const
{
    return *this;
}

bool Date::operator<(const Date& rhs) const
{
    return true;
}

bool operator<(const Date& lhs, const Date& rhs)
{
    return true;
}

//int operator-(const Date& lhs, const Date& rhs)
//{
//    return 0;
//}


/////////////////////////////////////////
// Definition of the class Duration
//////////////////////////////////////////
Duration::Duration(std::string& durStr)
{
    if(!Duration::isValidDuration(durStr))
        throw DurationException(durStr);


}

Duration::~Duration()
{
}

bool Duration::isValidDuration(std::string& durStr)
{
    static const boost::regex format("^(ON)|(O/N)|(TN)|(T/N)|"
            "([[:digit:]]+(\\.[[:digit:]]+)?(D|W|M|Y|Q))$",
            boost::regex::icase);
    
    return boost::regex_match(durStr, format);
}

std::string getDuration(Duration::TYPE type)
{
    return std::string("");
}

