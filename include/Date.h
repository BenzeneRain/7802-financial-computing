#ifndef _INCLUDE_DATE_H_
#define _INCLUDE_DATE_H_

#include <string>
#include <ctime>
#include <stdexcept>
#include <boost/date_time/gregorian/gregorian.hpp>

class Duration
{
    public:
        enum TYPE {DAY, WEEK, MONTH, QUARTER, YEAR, INVALID};

        Duration():_duration(-1), _type(INVALID){};
        explicit Duration(std::string& durStr);
        Duration(double duration, Duration::TYPE type):
            _duration(duration), _type(type){};
        Duration(const Duration& rDur):
            _duration(rDur._duration),
            _type(rDur._type){};

        ~Duration();

        static bool isValidDuration(std::string& durStr);

        double getDuration(Duration::TYPE type) const;
        inline Duration::TYPE type() const {return _type;}
        std::string toString(bool literal = false, bool hasUnit = true) const ;
        std::string toString(Duration::TYPE type,
                bool literal = false, bool hasUnit = true) const;
        
        
        Duration operator/(double rhs) const;
        double operator/(Duration& rhs) const;
        Duration operator*(double rhs) const;
    private:
        Duration::TYPE _type;
        double _duration;
};

class DurationException : public std::runtime_error
{
    public:
        DurationException(std::string& str):
            std::runtime_error(str){};
};

class Date
{
    public:
        enum TYPE {ACT365, ACT365J};

        Date(const boost::gregorian::date& date);
        Date(const Date& rdate):
            _date(rdate._date){};

        ~Date();

        static Date today();

        // Return the nearest next working day after a given
        // Duration
        Date operator+(const Duration& rhs) const;

        // Return the duration between two dates
        Duration operator-(const Date& rhs) const;
        
        bool operator<(const Date& rhs) const;
        bool operator==(const Date& rhs) const;
    private:
        TYPE _type;
        boost::gregorian::date _date;
};

// Calculate normlized difference of the date
// according to given Date time standard
double normDiffDate(Date&, Date&, Date::TYPE);

class DateException : public std::runtime_error
{
    public:
        DateException(std::string& e):
            std::runtime_error(e){};
};

#endif // _INCLUDE_DATE_H_ 
