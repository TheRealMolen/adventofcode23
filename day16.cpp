#include "pch.h"
#include "harness.h"


int day16(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day16_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day16()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day16(READ(sample)));
    //gogogo(day16(LOAD(16)));

    //test(-100, day16_2(READ(sample)));
    //gogogo(day16_2(LOAD(16)));
}