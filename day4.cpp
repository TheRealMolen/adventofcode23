#include "pch.h"
#include "harness.h"

#include <assert.h>


int day4(const stringlist& input)
{
    int result = 0;

    for (auto& line : input)
    {
        const char* pc = line.c_str();
        while (*pc && *pc != ':')
            ++pc;
        ++pc;

        set<int> winning;
        for (; *pc != '|'; ++pc)
        {
            if (*pc == ' ')
                continue;

            assert(isdigit(*pc));
            int num = *pc - '0';
            for (++pc; isdigit(*pc); ++pc)
                num = (num * 10) + (*pc - '0');

            winning.insert(num);
        }
        ++pc;

        int points = 0;
        while (*pc)
        {
            if (*pc == ' ')
            {
                ++pc;
                continue;
            }

            assert(isdigit(*pc));
            int num = *pc - '0';
            for (++pc; *pc && isdigit(*pc); ++pc)
                num = (num * 10) + (*pc - '0');

            if (winning.contains(num))
                points = points ? points * 2 : 1;
        }

        result += points;
    }

    return result;
}

int day4_2(const stringlist& input)
{
    const int height = int(size(input));
    vector<int> copies(height, 1);
    
    for (int i=0; i<height; ++i)
    {
        const string& line = input[i];

        const char* pc = line.c_str();
        while (*pc && *pc != ':')
            ++pc;
        ++pc;

        set<int> winning;
        for (; *pc != '|'; ++pc)
        {
            if (*pc == ' ')
                continue;

            assert(isdigit(*pc));
            int num = *pc - '0';
            for (++pc; isdigit(*pc); ++pc)
                num = (num * 10) + (*pc - '0');

            winning.insert(num);
        }
        ++pc;

        int points = 0;
        while (*pc)
        {
            if (*pc == ' ')
            {
                ++pc;
                continue;
            }

            assert(isdigit(*pc));
            int num = *pc - '0';
            for (++pc; *pc && isdigit(*pc); ++pc)
                num = (num * 10) + (*pc - '0');

            if (winning.contains(num))
                ++points;
        }

        for (int w=min(height, i+1); w<min(height, i+points+1); ++w)
        {
            copies[w] += copies[i];
        }
    }

    return accumulate(begin(copies), end(copies), 0);
}


void run_day4()
{
    string sample =
R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";

    test(13, day4(READ(sample)));
    gogogo(day4(LOAD(4)));

    test(30, day4_2(READ(sample)));
    gogogo(day4_2(LOAD(4)));
}