#ifndef _INCLUDE_DATE_H_
#define _INCLUDE_DATE_H_

#include <string>

class Date
{
    public:
        Date(std::string& str);
        ~Date();

        static Date* today();

        virtual Date* nWorkdaysAfter(int days) const = 0;
        virtual Date* nWorkdaysBefore(int days) const = 0;
        virtual bool isWorkday(Date& date) const = 0;

        virtual int operator-(Date& rhs) const = 0;
};

class ACT365:
    public Date
{
    public:
        ACT365(std::string& str);
        ~ACT365();

        virtual Date* nWorkdaysAfter(int days) const;
        virtual Date* nWorkdaysBefore(int days) const;
        virtual bool isWorkday(Date& date) const;

        virtual int operator-(Date& rhs) const;
};

class ACT365J:
    public Date
{
    public:
        ACT365J(std::string& str);
        ~ACT365J();

        virtual Date* nWorkdaysAfter(int days) const;
        virtual Date* nWorkdaysBefore(int days) const;
        virtual bool isWorkday(Date& date) const;

        virtual int operator-(Date& rhs) const;
};

//typedef ACT365 CurrDateType;
#endif // _INCLUDE_DATE_H_ 
