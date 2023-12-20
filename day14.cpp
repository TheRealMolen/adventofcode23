#include "pch.h"
#include "harness.h"


using TwoDMap = vector<char>;

TwoDMap load_map(const stringlist& lines)
{
    int w = isize(lines.front());
    int h = isize(lines);
    TwoDMap map;
    map.reserve(w * h);
    for (auto& line : lines)
        ranges::copy(line, back_inserter(map));

    return map;
}


void roll_north(TwoDMap& map, int w, int h)
{
    auto itCell = begin(map) + w;
    for (int y = 1; y < h; ++y)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            if (*itCell == 'O')
            {
                auto itDest = itCell;
                auto destY = y;
                while (destY > 0 && *(itDest - w) == '.')
                {
                    itDest -= w;
                    --destY;
                }
                if (itDest != itCell)
                {
                    *itDest = 'O';
                    *itCell = '.';
                }
            }
        }
    }
}

void roll_west(TwoDMap& map, int w, int h)
{
    auto itCell = begin(map);
    for (int y = 0; y < h; ++y)
    {
        ++itCell;
        for (int x = 1; x < w; ++x, ++itCell)
        {
            if (*itCell == 'O')
            {
                auto itDest = itCell;
                auto destX = x;
                while (destX > 0 && *(itDest - 1) == '.')
                {
                    --itDest;
                    --destX;
                }
                if (itDest != itCell)
                {
                    *itDest = 'O';
                    *itCell = '.';
                }
            }
        }
    }
}

void roll_south(TwoDMap& map, int w, int h)
{
    auto itCell = begin(map) + ((h - 1) * w);
    for (int y = h-2; y >= 0; --y)
    {
        for (int x = w-1; x >= 0; --x)
        {
            --itCell;
            if (*itCell == 'O')
            {
                auto itDest = itCell;
                auto destY = y;
                while (destY+1 < h && *(itDest + w) == '.')
                {
                    itDest += w;
                    ++destY;
                }
                if (itDest != itCell)
                {
                    *itDest = 'O';
                    *itCell = '.';
                }
            }
        }
    }
}

void roll_east(TwoDMap& map, int w, int h)
{
    auto itCell = end(map);
    for (int y = 0; y < h; ++y)
    {
        --itCell;
        for (int x = w-2; x >= 0; --x)
        {
            --itCell;
            if (*itCell == 'O')
            {
                auto itDest = itCell;
                auto destX = x;
                while (destX+1 < w && *(itDest + 1) == '.')
                {
                    ++itDest;
                    ++destX;
                }
                if (itDest != itCell)
                {
                    *itDest = 'O';
                    *itCell = '.';
                }
            }
        }
    }
}

int compute_load(const TwoDMap& map, int w, int h)
{
    int load = 0;
    auto itCell = begin(map);
    for (int ny = h; ny > 0; --ny)
    {
        for (int x = 0; x < w; ++x, ++itCell)
        {
            if (*itCell == 'O')
                load += ny;
        }
    }
    return load;
}

void dump(const TwoDMap& map, int w)
{
    for (auto itLine = begin(map); itLine != end(map); itLine += w)
        cout << string_view(itLine, itLine + w) << "\n";
    cout << endl;
}


int day14(const stringlist& input)
{
    int w = isize(input.front());
    int h = isize(input);
    TwoDMap map = load_map(input);

    roll_north(map, w, h);

    return compute_load(map, w, h);
}


size_t hash_map_state(const TwoDMap& map)
{
    return hash<string_view>()(string_view(map.data(), map.data() + map.size()));
}

int day14_2(const stringlist& input)
{
    int w = isize(input.front());
    int h = isize(input);
    TwoDMap map = load_map(input);

    unordered_set<size_t> encounteredStates;
    encounteredStates.insert(hash_map_state(map));

    const int kTargetCycles = 1000000000;

    vector<int> loopLoads;
    int startOfLoop = -1;
    size_t firstCycleHash = 0;
    for (int cycle=1; /**/; ++cycle)
    {
        roll_north(map, w, h);
        roll_west(map, w, h);
        roll_south(map, w, h);
        roll_east(map, w, h);

        size_t shash = hash_map_state(map);
        int load = compute_load(map, w, h);
        if (!encounteredStates.contains(shash))
        {
            encounteredStates.insert(shash);
        }
        else if (startOfLoop < 0)
        {
            startOfLoop = cycle;
            firstCycleHash = shash;
            loopLoads.push_back(load);
        }
        else if (shash != firstCycleHash)
        {
            loopLoads.push_back(load);
        }
        else
        {
            const int loopLen = cycle - startOfLoop;
            const int targetLoopCycles = kTargetCycles - startOfLoop;
            const int loopPos = targetLoopCycles % loopLen;

            return loopLoads[loopPos];
        }
    }
}


void run_day14()
{
    string sample =
R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....)";

    test(136, day14(READ(sample)));
    gogogo(day14(LOAD(14)));

    test(64, day14_2(READ(sample)));
    nononoD(day14_2(LOAD(14)));
}