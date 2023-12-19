#include "pch.h"
#include "harness.h"


int extrap_history(const string& line)
{
    vector<int> readings = string_to_intlist(line);
    int prediction = readings.back();

    vector<int> curr = readings;
    vector<int> next;
    next.reserve(100);
    do {
        for (auto a = begin(curr), b=a+1; b != end(curr); ++a, ++b)
        {
            next.push_back(*b - *a);
        }

        prediction += next.back();
        next.swap(curr);
        next.clear();

    } while (any_of(begin(curr), end(curr), [](int i) { return i != 0; }));

    return prediction;
}

int unextrap_history(const string& line)
{
    vector<int> readings = string_to_intlist(line);
    vector<int> head;
    head.reserve(100);
    head.push_back(readings.front());

    vector<int> curr = readings;
    vector<int> next;
    next.reserve(100);
    do {
        for (auto a = begin(curr), b = a + 1; b != end(curr); ++a, ++b)
        {
            next.push_back(*b - *a);
        }

        head.push_back(next.front());
        next.swap(curr);
        next.clear();

    } while (any_of(begin(curr), end(curr), [](int i) { return i != 0; }));

    int prediction = 0;
    for (int h : views::reverse(head))
        prediction = h - prediction;

    return prediction;
}


int day9(const stringlist& input)
{
    int result = 0;

    for (auto& line : input)
    {
        result += extrap_history(line);
    }

    return result;
}

int day9_2(const stringlist& input)
{
    int result = 0;

    for (auto& line : input)
    {
        result += unextrap_history(line);
    }

    return result;
}


void run_day9()
{
    string sample =
R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45)";

    test(18, extrap_history("0 3 6 9 12 15"));
    test(28, extrap_history("1 3 6 10 15 21"));
    test(114, day9(READ(sample)));
    gogogo(day9(LOAD(9)));

    test(0, unextrap_history("1 3 6 10 15 21"));
    test(5, unextrap_history("10 13 16 21 30 45"));
    test(2, day9_2(READ(sample)));
    gogogo(day9_2(LOAD(9)));
}