#include "pch.h"
#include "harness.h"

#include "vector2d.h"


using coord = Pt2<i16>;
enum CruxDir : i8 { Left, Up, Right, Down, BADDIR };
const static array<coord, 4> DirVecs = { coord{-1,0}, coord{0,-1}, coord{1,0}, coord{0,1} };

const static array<CruxDir, 4> TurnCWDirs = { Up, Right, Down, Left, };
const static array<CruxDir, 4> TurnACWDirs = { Down, Left, Up, Right, };

struct Crucible
{
    coord pos;
    CruxDir facing;
    i8 straightMovesLeft = 3;

    i16 loss = 0;

    static const u32 kMaxUid = (1 << 22) - 1;
    u32 getUid() const
    {
        return u32(pos.x) | (u32(pos.y) << 8) | (u32(facing) << 16) | (u32(straightMovesLeft) << 18);
    }

    auto operator<=>(const Crucible&) const = default;
};

struct CruxPrioGreater
{
    bool operator()(const Crucible& l, const Crucible& r) const
    {
        return l.loss > r.loss;
    }
};


int day17(const stringlist& input)
{
    const vector2d<char> cells(input);
    vector<u8> visitedIds(Crucible::kMaxUid + 1, 0);

    const coord start{ 0,0 };
    const coord dest{ i16(cells.width() - 1), i16(cells.height() - 1) };

    priority_queue<Crucible, vector<Crucible>, CruxPrioGreater> open;
    open.emplace(start, Right);

    auto tryAddMove = [&open, &cells, &visitedIds](const coord& pos, CruxDir dir, i8 straightsLeft, int loss)
    {
        coord dirVec = DirVecs[dir];
        coord newPos = pos + dirVec;
        if (cells.isInMap(newPos))
        {
            i16 newLoss = i16(loss + (cells[newPos] - '0'));

            Crucible crux(newPos, dir, straightsLeft, newLoss);
            if (!visitedIds[crux.getUid()])
                open.push(crux);
        }
    };

    auto processMovement = [&open, &cells, &tryAddMove](const Crucible& crux)
    {
        if (crux.straightMovesLeft > 0)
            tryAddMove(crux.pos, crux.facing, crux.straightMovesLeft - 1, crux.loss);

        tryAddMove(crux.pos, TurnCWDirs[crux.facing], 2, crux.loss);
        tryAddMove(crux.pos, TurnACWDirs[crux.facing], 2, crux.loss);
    };

    while (!open.empty())
    {
        const Crucible crux = open.top();
        open.pop();

        u32 uid = crux.getUid();
        if (visitedIds[uid])
            continue;
        visitedIds[uid] = 1;

        if (crux.pos == dest)
            return crux.loss;

        processMovement(crux);
    }

    return -1;
}

int day17_2(const stringlist& input)
{
    constexpr i8 kMinMovesInLine = 4;
    constexpr i8 kMaxMovesInLine = 10;

    const vector2d<char> cells(input);
    vector<u8> visitedIds(Crucible::kMaxUid + 1, 0);

    const coord start{ 0,0 };
    const coord dest{ i16(cells.width() - 1), i16(cells.height() - 1) };

    priority_queue<Crucible, vector<Crucible>, CruxPrioGreater> open;
    open.emplace(start, Right, kMaxMovesInLine);

    auto tryAddMove = [&open, &cells, &visitedIds](const coord& pos, CruxDir dir, i8 straightsLeft, int loss)
    {
        coord dirVec = DirVecs[dir];
        coord newPos = pos + dirVec;
        if (cells.isInMap(newPos))
        {
            i16 newLoss = i16(loss + (cells[newPos] - '0'));
            Crucible crux(newPos, dir, straightsLeft, newLoss);
            if (!visitedIds[crux.getUid()])
                open.push(crux);
        }
    };

    auto processMovement = [&open, &cells, &tryAddMove](const Crucible& crux)
    {
        if (crux.straightMovesLeft > 0)
            tryAddMove(crux.pos, crux.facing, crux.straightMovesLeft - 1, crux.loss);

        if (crux.straightMovesLeft <= (kMaxMovesInLine - kMinMovesInLine))
        {
            tryAddMove(crux.pos, TurnCWDirs[crux.facing], kMaxMovesInLine - 1, crux.loss);
            tryAddMove(crux.pos, TurnACWDirs[crux.facing], kMaxMovesInLine - 1, crux.loss);
        }
    };

    while (!open.empty())
    {
        const Crucible crux = open.top();
        open.pop();

        u32 uid = crux.getUid();
        if (visitedIds[uid])
            continue;
        visitedIds[uid] = 1;

        if (crux.pos == dest && crux.straightMovesLeft <= (kMaxMovesInLine - kMinMovesInLine))
            return crux.loss;

        processMovement(crux);
    }

    return -1;
}


void run_day17()
{
    string simple =
R"(91199
99119
99919
99911)";

    string simple2 =
R"(91111
99111
99911
99911)";

    string sample =
R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533)";

    string sample2 =
R"(111111111111
999999999991
999999999991
999999999991
999999999991)";

    test(7, day17(READ(simple)));
    test(7, day17(READ(simple2)));
    test(102, day17(READ(sample)));
    gogogo(day17(LOAD(17)));

    test(94, day17_2(READ(sample)));
    test(71, day17_2(READ(sample2)));
    gogogo(day17_2(LOAD(17)));
}