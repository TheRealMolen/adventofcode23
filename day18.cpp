#include "pch.h"
#include "harness.h"


int day18(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day18_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day18()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day18(READ(sample)));
    //gogogo(day18(LOAD(18)));

    //test(-100, day18_2(READ(sample)));
    //gogogo(day18_2(LOAD(18)));
}