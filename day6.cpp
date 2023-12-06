#include "pch.h"
#include "harness.h"


int count_ways_to_win(int raceTime, int record)
{
    int ways = 0;
    for (int i=1; i<raceTime; ++i)
    {
        const int speed = i;
        const int dist = speed * (raceTime - i);
        if (dist > record)
            ++ways;
        else if (ways > 0)
            break;
    }
    return ways;
}

i64 day6(const stringlist& input)
{
    istringstream times(input[0]);
    istringstream dists(input[1]);
    times >> "Time:" >> skipws;
    dists >> "Distance:" >> skipws;

    i64 result = 1;
    while (!times.eof())
    {
        int raceTime, record;
        times >> raceTime >> skipws;
        dists >> record >> skipws;

        result *= count_ways_to_win(raceTime, record);
    }

    return result;
}


double glom_digits(const string& s)
{
    double i = 0;
    for (char c : s)
    {
        if (isdigit(c))
        {
            i = (i * 10) + (c - '0');
        }
    }

    return i;
}

int day6_2(const stringlist& input)
{
    double raceTime = glom_digits(input[0]);
    double record = glom_digits(input[1]);

    // if T is time, R is record, we want the integers inside the roots of
    // -x^2 + Tx - R
    double a = -1.f;
    double b = raceTime;
    double c = -record;

    double disc = (b*b) - (4*a*c);
    double rt = sqrt(disc);

    double lo = (-b + rt) / (2 * a);
    double hi = (-b - rt) / (2 * a);

    double first = ceil(lo);
    double last = floor(hi);

    return int(last - first + 1);
}


void run_day6()
{
    string sample =
R"(Time:      7  15   30
Distance:  9  40  200)";

    test(288, day6(READ(sample)));
    gogogo(day6(LOAD(6)));

    test(71503, day6_2(READ(sample)));
    gogogo(day6_2(LOAD(6)));
}