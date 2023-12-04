#include "pch.h"
#include "harness.h"


int day1(const stringlist& input)
{
    int total = 0;
    for (auto& line : input)
    {
        char hi = line[line.find_first_of("0123456789")];
        char lo = line[line.find_last_of("0123456789")];

        total += (hi - '0') * 10;
        total += (lo - '0');
    }

    return total;
}

static const vector<string> kDigits = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int get_digit_fwd(const string& s)
{
    for (int ofs = 0; ofs < size(s); ++ofs)
    {
        string_view sub(s.data() + ofs);
        if (isdigit(sub.front()))
            return (sub.front() - '0');

        for (int i = 0; i < size(kDigits); ++i)
        {
            if (sub.starts_with(kDigits[i]))
                return i + 1;
        }
    }

    throw format("no digits in '{}'", s);
}

int get_digit_bak(const string& s)
{
    for (int ofs = int(size(s)) - 1; ofs >= 0; --ofs)
    {
        string_view sub(s.data() + ofs);
        if (isdigit(sub.front()))
            return (sub.front() - '0');

        for (int i = 0; i < size(kDigits); ++i)
        {
            if (sub.starts_with(kDigits[i]))
                return i + 1;
        }
    }

    throw format("no digits in '{}'", s);
}

int day1_2(const stringlist& input)
{
    int total = 0;
    for (auto& line : input)
    {
        total += 10 * get_digit_fwd(line);
        total += get_digit_bak(line);
    }

    return total;
}


void run_day1()
{
    string sample =
R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)";

    test(142, day1(READ(sample)));
    gogogo(day1(LOAD(1)));

    string sample2 =
R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";

    test(281, day1_2(READ(sample2)));
    gogogo(day1_2(LOAD(1)));
}
