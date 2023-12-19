#include "pch.h"
#include "harness.h"


int day19(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}

int day19_2(const stringlist& input)
{
    for (auto& line : input)
    {
        (void)line;
    }

    return -1;
}


void run_day19()
{
    string sample =
R"(beep
boop
blarp)";

    test(-100, day19(READ(sample)));
    //gogogo(day19(LOAD(19)));

    //test(-100, day19_2(READ(sample)));
    //gogogo(day19_2(LOAD(19)));
}