#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

#include "Date.h"

/////////////////////////////////////////
// Definition of the class Date
//////////////////////////////////////////
Date::Date(boost::gregorian::date& date, enum TYPE type):
    _type(type), _date(date)
{
}

Date::~Date()
{
}

Date Date::today(Date::TYPE type)
{
    boost::gregorian::date today = 
        boost::gregorian::day_clock::local_day();
    Date dateToday(today, type);
    return dateToday;
}


Date Date::operator+(const Duration& rhs) const
{
    //TODO:
    return Date(*this);
}

Duration Date::operator-(const Date& rhs) const
{
    //TODO:
    return Duration();
}

bool Date::operator<(const Date& rhs) const
{
    // TODO:
    return true;
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

