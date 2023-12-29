#include "pch.h"
#include "harness.h"

#include "pt2.h"
#include "vector2d.h"

#include <execution>


struct LavaCell
{
    char c = 'X';
    u8 beams = 0;

    LavaCell(char c) : c(c) { /**/ }
};
using coord = vector2d<LavaCell>::coord;

enum LavaDir { Left, Up, Right, Down };
const static array<coord, 4> DirVecs = { coord{-1,0}, coord{0,-1}, coord{1,0}, coord{0,1} };

// reflections for a / mirror
const static array<LavaDir, 4> PosReflections = { Down, Right, Up, Left };
// reflections for a \ mirror
const static array<LavaDir, 4> NegReflections = { Up, Left, Down, Right };

struct OpenMove
{
    coord pos;
    LavaDir dir;
    LavaCell* pcell;

    bool operator==(const OpenMove& o) const
    {
        return (dir == o.dir) && (pcell == o.pcell);
    }
};


int count_energised(const stringlist& input, const coord& startPos, LavaDir startDir)
{
    vector2d<LavaCell> cells(input);

    vector<OpenMove> open;
    open.emplace_back(startPos, startDir, data(cells) + (startPos.x + (cells.width() * startPos.y)));

    auto tryMoveFrom = [&open, &cells](const coord& pos, LavaDir dir)
    {
        coord dirVec = DirVecs[dir];
        coord newPos = pos + dirVec;
        if (!cells.isInMap(newPos))
            return;

        LavaCell* newCell = data(cells) + (newPos.x + (cells.width() * newPos.y));

        const u8 bmask = (1 << dir);
        if (!(newCell->beams & bmask))
        {
            OpenMove move(newPos, dir, newCell);
            if (find(data(open), data(open) + size(open), move) == (data(open) + size(open)))
                open.push_back(move);
        }
    };

    do
    {
        const OpenMove move = open.back();
        open.pop_back();

        const u8 bmask = (1 << move.dir);
        if (move.pcell->beams & bmask)
        {   // already visited
            continue;
        }
        move.pcell->beams |= bmask;

        switch (move.pcell->c)
        {
        case '.':
            tryMoveFrom(move.pos, move.dir);
            break;

        case '-':
            if (move.dir == Up || move.dir == Down)
            {
                tryMoveFrom(move.pos, Left);
                tryMoveFrom(move.pos, Right);
            }
            else
            {
                tryMoveFrom(move.pos, move.dir);
            }
            break;

        case '|':
            if (move.dir == Left || move.dir == Right)
            {
                tryMoveFrom(move.pos, Up);
                tryMoveFrom(move.pos, Down);
            }
            else
            {
                tryMoveFrom(move.pos, move.dir);
            }
            break;

        case '/':
            tryMoveFrom(move.pos, PosReflections[move.dir]);
            break;

        case '\\':
            tryMoveFrom(move.pos, NegReflections[move.dir]);
            break;

        default:
            throw format("wtf is a '{}'?", move.pcell->c);
        }

    } while (!open.empty());

    return int(ranges::count_if(cells, [](auto& c) { return c.beams > 0; }));
}


int day16(const stringlist& input)
{
    return count_energised(input, { 0,0 }, Right);
}

int day16_2(const stringlist& input)
{
    i16 w = i16(isize(input.front()));
    i16 h = i16(isize(input));

    vector<pair<coord, LavaDir>> starts;
    for (i16 x = 0; x < w; ++x)
    {
        starts.emplace_back(coord{ x, 0 }, Down);
        starts.emplace_back(coord{ x, i16(h - 1)}, Up);
    }
    for (i16 y = 0; y < h; ++y)
    {
        starts.emplace_back(coord{ 0, y }, Right);
        starts.emplace_back(coord{ i16(w - 1), y }, Left);
    }

    return transform_reduce(execution::par_unseq,
        begin(starts), end(starts), 0, 
        [](int a, int v) { return max(a, v); },
        [&input](auto& start)
        {
            return count_energised(input, start.first, start.second);
        });
}


void run_day16()
{
    string sample =
R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....)";

    test(46, day16(READ(sample)));
    gogogo(day16(LOAD(16)));

    test(51, day16_2(READ(sample)));
    nononoD(day16_2(LOAD(16)));
}