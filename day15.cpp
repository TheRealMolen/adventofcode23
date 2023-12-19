#include "pch.h"
#include "harness.h"


int day15(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day15_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day15()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day15(READ(sample)));
    //gogogo(day15(LOAD(15)));

    //test(-100, day15_2(READ(sample)));
    //gogogo(day15_2(LOAD(15)));
}