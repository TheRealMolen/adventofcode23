#include "pch.h"
#include "harness.h"
#include "chargrid.h"

vector<u32> compress_to_ints(const CharGrid map)
{
    vector<u32> ints(size(map), 0);
    ranges::transform(map, begin(ints), [](const string& s) -> u32
        {
            u32 i = 0;
            for (char c : s)
            {
                i = i << 1;
                if (c == '#')
                    i |= 1;
            }
            return i;
        });
    return ints;
}


namespace pt1 {

bool is_reflected_before_line(int ix, const vector<u32>& lines)
{
    int lo = ix - 1;
    int hi = ix;
    while (lo >= 0 && hi < isize(lines))
    {
        if (lines[lo] != lines[hi])
            return false;

        --lo;
        ++hi;
    }
    return true;
}

int get_hline_reflection(const CharGrid& lines)
{
    auto ints = compress_to_ints(lines);

    for (int i=1; i<isize(lines); ++i)
    {
        if (lines[i - 1] != lines[i])
            continue;

        if (!is_reflected_before_line(i, ints))
            continue;

        return i;
    }

    return 0;
}

int summarise_pattern(auto itBegin, auto itEnd)
{
    CharGrid hMap(itBegin, itEnd);
    CharGrid vMap = rotate(hMap, 3);

    return (100 * get_hline_reflection(hMap)) + get_hline_reflection(vMap);
}

}

int day13(const stringlist& input)
{
    int summary = 0;

    auto itPatternBegin = begin(input);
    for (auto itPatternEnd = itPatternBegin + 1; itPatternEnd != end(input); ++itPatternEnd)
    {
        if (!itPatternEnd->empty())
            continue;

        summary += pt1::summarise_pattern(itPatternBegin, itPatternEnd);

        itPatternBegin = itPatternEnd + 1;
        itPatternEnd = itPatternBegin + 1;
    }

    summary += pt1::summarise_pattern(itPatternBegin, end(input));

    return summary;
}


namespace pt2 {

inline bool is_power_of_two(u32 n)
{
    assert(n > 0);
    return (((n - 1) & n) == 0);
}

bool is_reflected_before_line(int ix, const vector<u32>& lines)
{
    bool found_one_smudge = false;

    int lo = ix - 1;
    int hi = ix;
    while (lo >= 0 && hi < isize(lines))
    {
        u32 error = lines[lo] ^ lines[hi];
        if (error)
        {
            if (found_one_smudge)
                return false;
            if (!is_power_of_two(error))
                return false;

            found_one_smudge = true;
        }

        --lo;
        ++hi;
    }
    return found_one_smudge;
}

int get_hline_reflection(const CharGrid& lines)
{
    auto ints = compress_to_ints(lines);

    for (int i = 1; i < isize(lines); ++i)
    {
        if (is_reflected_before_line(i, ints))
            return i;
    }

    return 0;
}

int summarise_pattern(auto itBegin, auto itEnd)
{
    CharGrid hMap(itBegin, itEnd);
    CharGrid vMap = rotate(hMap, 3);

    return (100 * get_hline_reflection(hMap)) + get_hline_reflection(vMap);
}

}

int day13_2(const stringlist& input)
{
    int summary = 0;

    auto itPatternBegin = begin(input);
    for (auto itPatternEnd = itPatternBegin + 1; itPatternEnd != end(input); ++itPatternEnd)
    {
        if (!itPatternEnd->empty())
            continue;

        summary += pt2::summarise_pattern(itPatternBegin, itPatternEnd);

        itPatternBegin = itPatternEnd + 1;
        itPatternEnd = itPatternBegin + 1;
    }

    summary += pt2::summarise_pattern(itPatternBegin, end(input));

    return summary;
}


void run_day13()
{
    string sample =
R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)";

    test(405, day13(READ(sample)));
    gogogo(day13(LOAD(13)));

    test(400, day13_2(READ(sample)));
    gogogo(day13_2(LOAD(13)));
}