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
        virtual Date& operator+(std::string& durationStr) const;

    private:
        DATETYPE _type;
};

#endif // _INCLUDE_DATE_H_ 
