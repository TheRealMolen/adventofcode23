#include "pch.h"
#include "harness.h"


struct MapRange
{
    u32 FromBegin, FromEnd;
    u32 ToBegin;
    u32 Offset;

    MapRange(const string& s)
    {
        u32 count;
        istringstream is(s);
        is >> ToBegin >> FromBegin >> count;
        FromEnd = FromBegin + count;
        Offset = ToBegin - FromBegin;
    }

    bool contains(u32 val) const
    {
        return (val >= FromBegin) && (val < FromEnd);
    }

    u32 remap(u32 val) const
    {
        u32 dist = val - FromBegin;
        return ToBegin + dist;
    }
};

using RemapTable = vector<MapRange>;

void sort_table(RemapTable& table)
{
    sort(begin(table), end(table), [](auto& a, auto& b) { return a.FromBegin < b.FromBegin; });
}

u32 remap_val(u32 val, const RemapTable& table)
{
    for (const auto& range : table)
    {
        if (val < range.FromBegin)
            continue;

        if (val < range.FromEnd)
            return range.remap(val);
    }

    return val;
}



int day5(const stringlist& input)
{
    vector<u32> current;

    auto itLine = begin(input);

    // read seed
    {
        istringstream is(*itLine);
        ++itLine;

        is >> "seeds: ";
        while (is)
        {
            u32 seed;
            is >> seed;
            current.push_back(seed);
        }
    }

    auto nextTable = [&itLine, &current, &input]()
    {
        vector<u32> next;
        next.reserve(size(current));

        // skip ws
        while (itLine->empty())
            ++itLine;

        // skip map header
        ++itLine;

        RemapTable table;
        for (/**/; itLine != end(input) && !itLine->empty(); ++itLine)
            table.emplace_back(*itLine);
        sort_table(table);

        transform(begin(current), end(current), back_inserter(next), [&table](u32 val)
            {
                return remap_val(val, table);
            });

        current.swap(next);
    };

    while (itLine != end(input))
        nextTable();

    return int(*min_element(begin(current), end(current)));
}

int day5_2(const stringlist& input)
{
    vector<RemapTable> tables;
    vector<pair<u32,u32>> current;

    auto itLine = begin(input);

    // read seed
    {
        istringstream is(*itLine);
        ++itLine;

        is >> "seeds: ";
        u32 totalSeeds = 0;
        while (is && !is.eof())
        {
            u32 start, count;
            is >> start >> count;
            current.emplace_back(start, start+count);
            totalSeeds += count;
        }
        sort(begin(current), end(current));
        cout << "need to think about " << format(std::locale(""), "{:L}", totalSeeds) << " seeds" << endl;
    }

    while (itLine != end(input))
    {
        // skip ws
        while (itLine->empty())
            ++itLine;

        // skip map header
        ++itLine;

        RemapTable table;
        for (/**/; itLine != end(input) && !itLine->empty(); ++itLine)
            table.emplace_back(*itLine);
        sort_table(table);

        tables.push_back(table);
    }

    for (const auto& table : tables)
    {
        vector<pair<u32, u32>> next;

        // intersect [lo,hi) with each range in the table
        for (auto [lo, hi] : current)
        {
            for (const MapRange& range : table)
            {
                if (hi <= range.FromBegin)
                {
                    next.emplace_back(lo, hi);
                    break;
                }

                if (lo >= range.FromEnd)
                    continue;

                if (lo < range.FromBegin)
                {
                    next.emplace_back(lo, range.FromBegin);
                    lo = range.FromBegin;
                }

                u32 rangeHi = min(hi, range.FromEnd);
                next.emplace_back(lo + range.Offset, rangeHi + range.Offset);
                lo = rangeHi;

                if (lo == hi)
                    break;
            }

            if (lo != hi)
                next.emplace_back(lo, hi);
        }

        sort(begin(next), end(next));
        next.swap(current);
    }

    return current.front().first;
}


void run_day5()
{
    string sample =
R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)";

    test(35, day5(READ(sample)));
    gogogo(day5(LOAD(5)));

    test(46, day5_2(READ(sample)));
    gogogo(day5_2(LOAD(5)), 78775051);
}