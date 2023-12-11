#include "pch.h"
#include "harness.h"
#include "chargrid.h"
#include "pt2.h"


void expandRows(CharGrid& universe)
{
    for (int i = isize(universe) - 1; i >= 0; --i)
    {
        if (universe[i].find('#') == string::npos)
        {
            string row = universe[i];
            universe.insert(begin(universe) + i, move(row));
        }
    }
};

void expandCols(CharGrid& universe)
{
    for (int col = isize(universe.front()) - 1; col >= 0; --col)
    {
        if (all_of(begin(universe), end(universe), [col](const string& s) { return s[col] == '.'; }))
        {
            for (string& row : universe)
            {
                row.insert(col, 1, '.');
            }
        }
    }
};

vector<Pt2i> getStarPositions(const CharGrid& universe)
{
    vector<Pt2i> stars;

    int height = isize(universe);
    int width = isize(universe.front());
    for (int y = 0; y < height; ++y)
    {
        auto it = begin(universe[y]);
        for (int x = 0; x < width; ++x, ++it)
        {
            if (*it == '#')
                stars.emplace_back(x, y);
        }
    }

    return stars;
}


int day11(const stringlist& input)
{
    CharGrid universe(input);
    expandRows(universe);
    expandCols(universe);

    vector<Pt2i> stars = getStarPositions(universe);

    int totalDist = 0;
    for (auto itA = begin(stars); (itA+1) != end(stars); ++itA)
    {
        for (auto itB = itA + 1; itB != end(stars); ++itB)
        {
            Pt2i d = *itB - *itA;
            int dist = abs(d.x) + abs(d.y);
            totalDist += dist;
        }
    }

    return totalDist;
}

i64 day11_2(const stringlist& input, i64 expansion)
{
    auto& universe = input;

    vector<u8> expandedRow;
    ranges::transform(input, back_inserter(expandedRow), [](const auto& row) { return row.find('#') == string::npos; });

    vector<u8> expandedCol;
    ranges::transform(views::iota(0, isize(input.front())), back_inserter(expandedCol),
        [&universe](int col)
        {
            return all_of(begin(universe), end(universe), [col](auto& row) { return row[col] == '.'; });
        });

    vector<Pt2i> stars = getStarPositions(universe);

    auto measureSpan = [expansion](const auto& expanded, int a, int b) -> i64
    {
        auto [lo, hi] = minmax(a, b);
        auto runStart = data(expanded) + lo;
        auto runEnd = data(expanded) + hi;

        i64 dist = 0;
        for (auto itExp = runStart; itExp != runEnd; ++itExp)
            dist += *itExp ? expansion : 1ll;
        return dist;
    };

    i64 totalDist = 0;
    for (auto itA = begin(stars); (itA + 1) != end(stars); ++itA)
    {
        for (auto itB = itA + 1; itB != end(stars); ++itB)
        {
            totalDist += measureSpan(expandedCol, itA->x, itB->x);
            totalDist += measureSpan(expandedRow, itA->y, itB->y);
        }
    }

    return totalDist;
}


void run_day11()
{
    string sample =
R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";

    test(374, day11(READ(sample)));
    gogogo(day11(LOAD(11)));

    test(1030, day11_2(READ(sample), 10));
    test(8410, day11_2(READ(sample), 100));
    gogogo(day11_2(LOAD(11), 1'000'000));
}