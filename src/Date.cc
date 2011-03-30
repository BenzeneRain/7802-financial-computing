#include "Date.h"

/////////////////////////////////////////
// Definition of the interface class Date
//////////////////////////////////////////
Date::Date(std::string& str)
{
}

Date::~Date()
{
}

/////////////////////////////////////////
// Definition of the class ACT365
//////////////////////////////////////////
ACT365::ACT365(std::string& str):
    Date(str)
{
}

ACT365::~ACT365()
{
}

Date* ACT365::nWorkdaysBefore(int days) const
{
    return static_cast<Date *>(0);
}

Date* ACT365::nWorkdaysAfter(int days) const
{
    return static_cast<Date *>(0);
}

bool ACT365::isWorkday(Date& date) const
{
    return true;
}

int ACT365::operator-(Date& rhs) const
{
    return 0;
}

/////////////////////////////////////////
// Definition of the class ACT365J
//////////////////////////////////////////
ACT365J::ACT365J(std::string& str):
    Date(str)
{
}

ACT365J::~ACT365J()
{
}

Date* ACT365J::nWorkdaysBefore(int days) const
{
    return static_cast<Date *>(0);
}

Date* ACT365J::nWorkdaysAfter(int days) const
{
    return static_cast<Date *>(0);
}

bool ACT365J::isWorkday(Date& date) const
{
    return true;
}

int ACT365J::operator-(Date& rhs) const
{
    return 0;
}
