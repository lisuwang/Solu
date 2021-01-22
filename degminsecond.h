#ifndef DEGMINSECOND_H
#define DEGMINSECOND_H
#include <string>

class DegMinSecond
{
public:
    DegMinSecond() = delete;
    DegMinSecond(int deg, int min , double second);
    DegMinSecond(int deg, double min);
    DegMinSecond(double deg);
    std::string OutPutDegFormat()const;
    std::string OutPutDegMinFormat()const;
    std::string OutPutDegMinSecondFormat()const;

private:
    struct ThreeSegFormat
    {
        int deg_;
        int min_;
        double second_;

    };
    struct OneSegFormat
    {
        double deg_;

    };
    struct TwoSegFormat
    {
        int deg_;
        double min_;

    };

    ThreeSegFormat threeSegFormat_;
    TwoSegFormat twoSegFormat_;
    OneSegFormat oneSegFormat_;


};

#endif // DEGMINSECOND_H
