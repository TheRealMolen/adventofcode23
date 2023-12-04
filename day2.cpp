#include "pch.h"
#include "harness.h"


enum Col : u8 { Red, Green, Blue, COL_COUNT };

inline Col getcol(char c)
{
    switch(c) {
    case 'r': return Red;
    case 'g': return Green;
    case 'b': return Blue;
    default:
        throw format("bad colour: '{}'", c);
    }
}

int day2(const stringlist& input)
{
    array<int, COL_COUNT> bag = { 12, 13, 14 };

    int result = 0;
    int id = 0;
    for (auto& line : input)
    {
        ++id;

        string all_draws = split(line, ": ")[1];
        vector<string> draws = split(all_draws, "; ");

        bool impossible = false;
        for (const string& draw : draws)
        {
            vector<string> counts = split(draw, ", ");
            for (const string& count : counts)
            {
                istringstream is(count);
                int n;
                char col;
                is >> n >> " " >> col;
                if (n > bag[getcol(col)])
                {
                    impossible = true;
                    break;
                }
            }
        }

        if (!impossible)
            result += id;
    }

    return result;
}

i64 day2_2(const stringlist& input)
{
    i64 result = 0;
    for (auto& line : input)
    {
        string all_draws = split(line, ": ")[1];
        vector<string> draws = split(all_draws, "; ");

        array<i64, COL_COUNT> min_bag = { 1, 1, 1 };

        for (const string& draw : draws)
        {
            vector<string> counts = split(draw, ", ");
            for (const string& count : counts)
            {
                istringstream is(count);
                i64 n;
                char col;
                is >> n >> " " >> col;
                Col c = getcol(col);
                min_bag[c] = max(min_bag[c], n);
            }
        }

        i64 power = accumulate(begin(min_bag), end(min_bag), 1ll, [](i64 x, i64 a) { return x * a; });;
        result += power;
    }

    return result;
}


void run_day2()
{
    string sample =
R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";

    test(8, day2(READ(sample)));
    gogogo(day2(LOAD(2)), 2593);

    test(2286, day2_2(READ(sample)));
    gogogo(day2_2(LOAD(2)));
}