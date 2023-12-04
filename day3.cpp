#include "pch.h"
#include "harness.h"


bool find_symbol(int startx, int starty, int stopx, int stopy, const vector<string>& plan)
{
    for (int y=starty; y<=stopy; ++y)
    {
        const string& row = plan[y];
        for (int x=startx; x<stopx; ++x)
        {
            char c = row[x];
            if (c == '.')
                continue;
            if (isdigit(c))
                continue;
            return true;
        }
    }
    return false;
}

int day3(const stringlist& input)
{
    int result = 0;

    int height = int(size(input));
    int width = int(size(input.front()));

    for (int y=0; y<height; ++y)
    {
        const string& row = input[y];
        int starty = max(0, y - 1);
        int stopy = min(height - 1, y + 1);

        int x = 0;
        while (x < width)
        {
            // skip to next number
            while (!isdigit(row[x]))
            {
                ++x;
                if (x >= width)
                    goto endrow;
            }

            int num = row[x] - '0';
            int startx = max(0, x - 1);

            // read the whole number
            for (++x ; (x < width) && isdigit(row[x]); ++x)
            {
                num *= 10;
                num += row[x] - '0';
            }

            int stopx = min(width - 1, x + 1);

            if (find_symbol(startx, starty, stopx, stopy, input))
                result += num;
        }

endrow: y = y;
    }

    return result;
}


int find_gear(int x, int y, const stringlist& plan)
{
    const string& row = plan[y];

    if (!isdigit(row[x]))
        return 0;

    while (x > 0 && isdigit(row[x - 1]))
        --x;

    int num = row[x] - '0';
    for (++x ; x < size(row) && isdigit(row[x]); ++x)
    {
        num *= 10;
        num += row[x] - '0';
    }
    return num;
}

int get_ratio(int x, int y, const stringlist& plan)
{
    vector<int> gears;
    const int width = int(size(plan.front()));
    const int height = int(size(plan));

    // e & w are trivial
    if (x > 0)
    {
        if (int gear = find_gear(x - 1, y, plan))
            gears.push_back(gear);
    }
    if (x < width-1)
    {
        if (int gear = find_gear(x + 1, y, plan))
            gears.push_back(gear);
    }

    if (y > 0)
    {
        if (int gear = find_gear(x, y-1, plan))
        {
            gears.push_back(gear);
        }
        else
        {
            if (x > 0)
            {
                if (int gear = find_gear(x - 1, y-1, plan))
                    gears.push_back(gear);
            }
            if (x < width - 1)
            {
                if (int gear = find_gear(x + 1, y-1, plan))
                    gears.push_back(gear);
            }
        }
    }

    if (y < height-1)
    {
        if (int gear = find_gear(x, y + 1, plan))
        {
            gears.push_back(gear);
        }
        else
        {
            if (x > 0)
            {
                if (int gear = find_gear(x - 1, y + 1, plan))
                    gears.push_back(gear);
            }
            if (x < width - 1)
            {
                if (int gear = find_gear(x + 1, y + 1, plan))
                    gears.push_back(gear);
            }
        }
    }

    if (size(gears) == 2)
        return gears.front() * gears.back();

    return 0;
}

i64 day3_2(const stringlist& input)
{
    i64 result = 0;

    int height = int(size(input));
    int width = int(size(input.front()));

    for (int y = 0; y < height; ++y)
    {
        const string& row = input[y];

        auto itC = begin(row);
        for (int x = 0; x < width; ++x, ++itC)
        {
            if (*itC != '*')
                continue;

            result += get_ratio(x, y, input);
        }
    }

    return result;
}


void run_day3()
{
    string sample =
R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..)";

    test(4361, day3(READ(sample)));
    gogogo(day3(LOAD(3)));

    test(467835, day3_2(READ(sample)));
    gogogo(day3_2(LOAD(3)));
}