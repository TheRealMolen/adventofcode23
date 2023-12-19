#include "pch.h"
#include "harness.h"


int day17(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day17_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day17()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day17(READ(sample)));
    //gogogo(day17(LOAD(17)));

    //test(-100, day17_2(READ(sample)));
    //gogogo(day17_2(LOAD(17)));
}