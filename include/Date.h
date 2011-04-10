#ifndef _INCLUDE_DATE_H_
#define _INCLUDE_DATE_H_

#include <string>

class Date
{
    public:
        enum DATETYPE {ACT365, ACT365J};

        explicit Date(std::string& str, enum DATETYPE type);
        ~Date();

        static Date today(Date::DATETYPE type);

        virtual Date nWorkdaysAfter(int days) const;
        virtual Date nWorkdaysBefore(int days) const;
        virtual bool isWorkday(Date& date) const;

        virtual int operator-(Date& rhs) const;
        virtual const Date& operator+(std::string& durationStr) const;
        virtual bool operator<(const Date& rhs) const;

    private:
        DATETYPE _type;
};

// Non-member function
//bool operator<(const Date& lhs, const Date& rhs);
int operator-(const Date& lhs, const Date& rhs);


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
        
    private:
        TYPE _type;
        double _duration;
};

class DurationException
{
    public:
        DurationException(std::string& str):
            _info(str){};
        ~DurationException(){};

        inline std::string info() {return _info;}

    private:
        std::string _info;
};
#endif // _INCLUDE_DATE_H_ 
