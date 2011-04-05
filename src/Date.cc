#include "Date.h"

/////////////////////////////////////////
// Definition of the interface class Date
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
}

Date Date::nWorkdaysAfter(int days) const
{
}

Date Date::nWorkdaysBefore(int days) const
{
}

bool Date::isWorkday(Date& date) const
{
}

int Date::operator-(Date& rhs) const
{
}

Date& Date::operator+(std::string& durationStr) const
{
}
