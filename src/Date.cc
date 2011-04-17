#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

#include "Date.h"


/////////////////////////////////////////
// Definition of the class Date
//////////////////////////////////////////
Date::Date(const boost::gregorian::date& date):
    _date(date)
{
}

Date::~Date()
{
}

Date Date::today()
{
    boost::gregorian::date today = 
        boost::gregorian::day_clock::local_day();
    Date dateToday(today);
    return dateToday;
}


Date Date::operator+(const Duration& rhs) const
{
    Duration::TYPE type = rhs.type();

    switch(type)
    {
        case Duration::WEEK:
        case Duration::DAY:
        {
            boost::gregorian::date_duration duration(
                    (long)(rhs.getDuration(Duration::DAY)));
            Date newDay(this->_date + duration);
            return newDay;
        }
        case Duration::YEAR:
        case Duration::MONTH:
        case Duration::QUARTER:
        {
            boost::gregorian::months duration(
                    (int)(rhs.getDuration(Duration::MONTH)));

            Date newDay(this->_date + duration);

            boost::gregorian::date newRawDay(newDay.get());
             
            // Considering the day
            // in the month, if the resulting day is
            // larger then the day before add, then
            // modify the resulting day to the day 
            // before add

            int diff;
            if((diff = _date.day() - newRawDay.day()) < 0)
            {
                boost::gregorian::date modifiedRawDate =
                    newRawDay - boost::gregorian::date_duration(-diff);
                
                Date modifiedDate(modifiedRawDate);
                return modifiedDate;
            }
            else
            {
                return newDay;
            }
        }
        default:
        {
            std::string errorMessage("Invalid Duration");
            throw DateException(errorMessage);
        }
    }
}

Duration Date::operator-(const Date& rhs) const
{
    boost::gregorian::date_duration duration = 
        (*this) < rhs ? rhs._date - this->_date :
                        this->_date - rhs._date;

    return Duration(duration.days(), Duration::DAY);
}

Date Date::operator-(const Duration& rhs) const
{
    switch(rhs.type())
    {
        case Duration::WEEK:
        case Duration::DAY:
            {
                boost::gregorian::date_duration duration(rhs.getDuration(Duration::DAY));
                boost::gregorian::date newDate = this->_date - duration;
                return Date(newDate);
            }
        case Duration::YEAR:
        case Duration::QUARTER:
        case Duration::MONTH:
            {
                boost::gregorian::months duration(rhs.getDuration(Duration::MONTH));
                boost::gregorian::date newDate = this->_date - duration;
                return Date(newDate);
            }
        default:
            {
                std::string errorMessage("Invalid Date");
                throw DurationException(errorMessage);
            }
    }
}

bool Date::operator<(const Date& rhs) const
{
    return _date < rhs._date;
}

bool Date::operator==(const Date& rhs) const
{
    return _date == rhs._date;
}

/////////////////////////////////////////
// Definition of Date Helper functions
//////////////////////////////////////////
double normDiffDate(Date& d1, Date& d2, Date::TYPE type)
{
    Duration duration = d1 < d2 ? d2 - d1 : d1 - d2;

    switch(type)
    {
        case Date::ACT365:
                return duration.getDuration(Duration::DAY) / 365.0;
        default:
            {
                std::string errorMessage("Unsupported Date::TYPE");
                throw DateException(errorMessage);
            }
    }
}


/////////////////////////////////////////
// Definition of the class Duration
//////////////////////////////////////////
Duration::Duration(std::string& durStr)
{
    if(!Duration::isValidDuration(durStr))
        throw DurationException(durStr);

    static const boost::regex onFormat("^(ON)|(O/N)$", boost::regex::icase);
    static const boost::regex tnFormat("^(TN)|(T/N)$", boost::regex::icase);
    static const boost::regex dayFormat("[[:digit:]]+(\\.[[:digit:]]+)?D");
    static const boost::regex weekFormat("[[:digit:]]+(\\.[[:digit:]]+)?W");
    static const boost::regex monthFormat("[[:digit:]]+(\\.[[:digit:]]+)?M");
    static const boost::regex quarterFormat("[[:digit:]]+(\\.[[:digit:]]+)?Q");
    static const boost::regex yearFormat("[[:digit:]]+(\\.[[:digit:]]+)?Y");


    try
    {
        if(boost::regex_match(durStr, onFormat))
        {
            _type = Duration::DAY;
            _duration = 1.0f;
        }
        else if(boost::regex_match(durStr, tnFormat))
        {
            _type = Duration::DAY;
            _duration = 2.0f;
        }
        else if(boost::regex_match(durStr, dayFormat))
        {
            _type = Duration::DAY;

            std::string duration = durStr.substr(0, durStr.length() - 1);
            _duration = boost::lexical_cast<double>(duration);
        }
        else if(boost::regex_match(durStr, weekFormat))
        {
            _type = Duration::WEEK;

            std::string duration = durStr.substr(0, durStr.length() - 1);
            _duration = boost::lexical_cast<double>(duration);
        }
        else if(boost::regex_match(durStr, monthFormat))
        {
            _type = Duration::MONTH;

            std::string duration = durStr.substr(0, durStr.length() - 1);
            _duration = boost::lexical_cast<double>(duration);
        }
        else if(boost::regex_match(durStr, quarterFormat))
        {
            _type = Duration::QUARTER;

            std::string duration = durStr.substr(0, durStr.length() - 1);
            _duration = boost::lexical_cast<double>(duration);
        }
        else if(boost::regex_match(durStr, yearFormat))
        {
            _type = Duration::YEAR;

            std::string duration = durStr.substr(0, durStr.length() - 1);
            _duration = boost::lexical_cast<double>(duration);
        }
        else
        {
            std::string errorMsg = "Unreachable branch, input string is " + durStr;
            throw DurationException(errorMsg);
        }
    }
    catch(boost::bad_lexical_cast& e)
    {
        std::string errorMsg = "Fail to read the duration value from " + durStr;
        throw DurationException(errorMsg);
    }
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

double Duration::getDuration(Duration::TYPE type) const
{
    if(type == _type)
        return _duration;

    //FIX: Suppose 1 MONTH = 30.42 DAY
    //  thus 1 MONTH = 4.35 WEEK
    // 1 YEAR = 365 DAY
    // 1 YEAR = 52.2 WEEK
    switch(_type)
    {
        case Duration::DAY:
            {
                switch(type)
                {
                    case Duration::WEEK:
                        return _duration / 7.0f;
                    case Duration::MONTH:
                        return _duration / 30.42f;
                    case Duration::QUARTER:
                        return _duration / 3.0f / 30.42f;
                    case Duration::YEAR:
                        return _duration / 365.0f;
                    default:
                        {
                            std::string errorMsg = "Unknow Duration Type";
                            throw DurationException(errorMsg);
                        }
                }
                break;
            }
        case Duration::WEEK:
            {
                switch(type)
                {
                    case Duration::DAY:
                        return _duration * 7.0f;
                    case Duration::MONTH:
                        return _duration / 4.35f;
                    case Duration::QUARTER:
                        return _duration / 3.0f / 4.35f;
                    case Duration::YEAR:
                        return _duration / 52.2f;
                    default:
                        {
                            std::string errorMsg = "Unknow Duration Type";
                            throw DurationException(errorMsg);
                        }
                }
                break;
            }
        case Duration::MONTH:
            {
                switch(type)
                {
                    case Duration::DAY:
                        return _duration * 30.42f;
                    case Duration::WEEK:
                        return _duration * 4.35f;
                    case Duration::QUARTER:
                        return _duration / 3.0f;
                    case Duration::YEAR:
                        return _duration / 12.0f;
                    default:
                        {
                            std::string errorMsg = "Unknow Duration Type";
                            throw DurationException(errorMsg);
                        }
                }
                break;
            }
        case Duration::QUARTER:
            {
                switch(type)
                {
                    case Duration::DAY:
                        return _duration * 3.0f * 30.42f;
                    case Duration::WEEK:
                        return _duration * 3.0f * 4.35f;
                    case Duration::MONTH:
                        return _duration * 3.0f;
                    case Duration::YEAR:
                        return _duration / 4.0f;
                    default:
                        {
                            std::string errorMsg = "Unknow Duration Type";
                            throw DurationException(errorMsg);
                        }
                }
                break;
            }
        case Duration::YEAR:
            {
                switch(type)
                {
                    case Duration::DAY:
                        return _duration * 365.0f;
                    case Duration::WEEK:
                        return _duration * 52.2f;
                    case Duration::MONTH:
                        return _duration * 12.0f;
                    case Duration::QUARTER:
                        return _duration * 4.0f;
                    default:
                        {
                            std::string errorMsg = "Unknow Duration Type";
                            throw DurationException(errorMsg);
                        }
                }
                break;
            }
        default:
            {
                std::string errorMsg = "Unknow Duration Type";
                throw DurationException(errorMsg);
            }
    }

}

std::string Duration::toString(bool literal, bool hasUnit) const 
{
    return Duration::toString(this->_type, literal, hasUnit);
}

std::string Duration::toString(Duration::TYPE type, bool literal, bool hasUnit) const
{
    if(literal == true && type == Duration::DAY)
    {
        if(_duration == 1.0)
            return "ON";
        else if(_duration == 2.0)
            return "TN";
    }

    std::ostringstream oss;
    try
    {
        double changedDur = this->getDuration(type);
        oss << changedDur;
    }
    catch(DurationException& e)
    {
        throw e;
    }
    catch(boost::bad_lexical_cast& e)
    {
        throw e;
    }

    if(hasUnit)
    {
        switch(type)
        {   
            case Duration::DAY:
                oss << "D";
                break;
            case Duration::WEEK:
                oss << "W";
                break;
            case Duration::MONTH:
                oss << "M";
                break;
            case Duration::QUARTER:
                oss << "Q";
                break;
            case Duration::YEAR:
                oss << "Y";
                break;
            default:
                std::string errorMsg = "Invalid Duration Type";
                throw DurationException(errorMsg);
        }
    }

    return oss.str();
}

Duration Duration::operator/(double rhs) const
{
    Duration newDuration(this->getDuration(this->type()) / rhs,
            this->type());

    return newDuration;
}

double Duration::operator/(const Duration& rhs) const
{
    Duration::TYPE type = this->type();
    return this->getDuration(type) / rhs.getDuration(type);
}

Duration Duration::operator*(double rhs) const
{
    Duration newDuration(this->getDuration(this->type()) * rhs,
            this->type());

    return newDuration;
}

Duration Duration::operator-(const Duration& rhs) const
{
    Duration::TYPE type = std::max(_type, rhs.type());
    Duration newDuration(this->operator<(rhs) ? 
            rhs.getDuration(type) - this->getDuration(type) :
            this->getDuration(type) - rhs.getDuration(type),
            type);

    return newDuration;
}

bool Duration::operator==(const Duration& rhs) const
{
    double time1 = this->getDuration(this->type());
    double time2 = rhs.getDuration(this->type());

    return time1 == time2;
}

bool Duration::operator<(const Duration& rhs) const
{
    double time1 = this->getDuration(_type);
    double time2 = rhs.getDuration(_type);

    return time1 < time2;
}

/////////////////////////////////////////
// Definition of the class WorkDate
//////////////////////////////////////////
WorkDate::WorkDate(const Date& rhs):
    Date(rhs)
{
    _jumpToNearestNextWorkDay();
}

WorkDate::~WorkDate()
{
}

void WorkDate::_jumpToNearestNextWorkDay()
{
    using namespace boost::gregorian;

    greg_weekday gw = _date.day_of_week();
    if(gw == Saturday || gw == Sunday)
        _date = next_weekday(_date, greg_weekday(Monday));
}
