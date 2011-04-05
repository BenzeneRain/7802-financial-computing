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
