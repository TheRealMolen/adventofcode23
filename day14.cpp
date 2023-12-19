#include "pch.h"
#include "harness.h"


int day14(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day14_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day14()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day14(READ(sample)));
    //gogogo(day14(LOAD(14)));

    //test(-100, day14_2(READ(sample)));
    //gogogo(day14_2(LOAD(14)));
}