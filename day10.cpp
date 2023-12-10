#include "pch.h"
#include "harness.h"
#include "pt2.h"
#include <assert.h>


enum Direction { North, East, South, West, DirCount, InvalidDir=DirCount };
array<Pt2i, DirCount> DirVecs = { Pt2i{0,-1}, Pt2i{1,0}, Pt2i{0,1}, Pt2i{-1,0} };

inline Direction invert(Direction dir)
{
    return Direction((dir + 2) & (DirCount-1));
}


// if you're on a cell, which directions can you move in?
const unordered_map<char, pair<Direction, Direction>> kPipeInfo =
{
    { '|', { North, South }},
    { '-', { East, West }},
    { 'F', { South, East }},
    { 'L', { North, East }},
    { 'J', { North, West }},
    { '7', { West, South }},
};


struct CharMap
{
    int w, h;
    vector<char> data;
    Pt2i start;

    CharMap(const stringlist& input) : w(int(size(input.front()))), h(int(size(input)))
    {
        data.reserve(w * h);
        for (auto& line : input)
            data.append_range(line);

        auto itStart = ranges::find(data, 'S');
        int ixStart = int(distance(begin(data), itStart));
        start.x = ixStart % w;
        start.y = ixStart / w;

        // replace S with the correct pipe
        vector<Direction> dirs;
        if (PipeHasExit(start + Pt2i{ 1,0 }, West))
            dirs.push_back(East);
        if (PipeHasExit(start + Pt2i{ -1,0 }, East))
            dirs.push_back(West);
        if (PipeHasExit(start + Pt2i{ 0,1 }, North))
            dirs.push_back(South);
        if (PipeHasExit(start + Pt2i{ 0,-1 }, South))
            dirs.push_back(North);

        auto itInfo = ranges::find_if(kPipeInfo, [&dirs](auto& info) -> bool
            {
                if (info.second.first == dirs[0] && info.second.second == dirs[1])
                    return true;
                if (info.second.first == dirs[1] && info.second.second == dirs[0])
                    return true;
                return false;
            });
        *itStart = itInfo->first;
    }

    char Cell(const Pt2i& p) const
    {
        assert(p.x >= 0);
        assert(p.y >= 0);
        assert(p.x < w);
        assert(p.y < h);
        return data[p.x + p.y * w];
    }

    bool PipeHasExit(const Pt2i& pos, Direction dir) const
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= w || pos.y >= h)
            return false;

        char pipe = Cell(pos);
        if (pipe == '.')
            return false;

        auto& exits = kPipeInfo.at(pipe);
        return ((exits.first == dir) || (exits.second == dir));
    }

    pair<Direction, Direction> GetCellExits(const Pt2i& pos)
    {
        char cell = Cell(pos);
        return kPipeInfo.at(cell);
    }
};


struct Rodent
{
    Pt2i pos;
    Direction dir;
    int steps;
};

int day10(const stringlist& input)
{
    CharMap pipes(input);

    auto initDirs = pipes.GetCellExits(pipes.start);
    array<Rodent, 2> mice {
        Rodent{ pipes.start, initDirs.first, 0 },
        Rodent{ pipes.start, initDirs.second, 0 },
    };

    do
    {
        for (auto& m : mice)
        {
            Pt2i move = DirVecs[m.dir];
            Pt2i newpos = m.pos + move;

            Direction fromdir = invert(m.dir);
            auto exits = pipes.GetCellExits(newpos);

            m.dir = (fromdir == exits.first) ? exits.second : exits.first;
            m.pos = newpos;
            ++m.steps;
        }

    } while (mice[0].pos != mice[1].pos);

    return mice[0].steps;
}


int day10_2(const stringlist& input)
{
    CharMap pipes(input);
    vector<char> path(size(pipes.data), '.');
    const int width = pipes.w;

    auto markpath = [&path, width](const Pt2i& p, Direction dir)
    {
        char c = "^>v<"[dir];
        path[p.x + p.y * width] = c;
    };
    auto dumppath = [&path, width]()
    {
        for (auto itPath = begin(path); itPath != end(path); itPath += width)
        {
            string_view row(itPath, itPath + width);
            cout << row << endl;
        }
    };

    auto initDirs = pipes.GetCellExits(pipes.start);
    array<Rodent, 2> mice{
        Rodent{ pipes.start, initDirs.first, 0 },
        Rodent{ pipes.start, initDirs.second, 0 },
    };

    for (auto& m : mice)
        markpath(m.pos, m.dir);

    do
    {
        for (auto& m : mice)
        {
            Pt2i move = DirVecs[m.dir];
            Pt2i newpos = m.pos + move;

            Direction fromdir = invert(m.dir);
            auto exits = pipes.GetCellExits(newpos);

            m.dir = (fromdir == exits.first) ? exits.second : exits.first;
            m.pos = newpos;
            ++m.steps;

            markpath(newpos, m.dir);
        }

    } while (mice[0].pos != mice[1].pos);


 //   dumppath();

    stringlist e;
    int enclosed = 0;
    auto itPipe = begin(pipes.data);
    for (auto itPath = begin(path); itPath != end(path); /**/)
    {
        string s;
        bool inside = false;
        bool boundary = false;
        bool enteredFromNorth = false;

        // .F-------7.
        // oiiiiiiiiio
        // nBBBBBBBBBn

        // .|L-7.F-J|.
        // oiooooiiioo
        // nnBBnnBBnnn

        for (auto rowEnd = itPath + width; itPath != rowEnd; ++itPath, ++itPipe)
        {
            char c = *itPath;
            char p = (c != '.') ? *itPipe : '.';
            char dbg = '.';// p;
            if (c == '.' && inside)
            {
                dbg = 'I';
                ++enclosed;
            }
            else if (p == '|')
            {
                assert(!boundary);
                inside = !inside;
            }
            else if (p == '7' || p == 'J')
            {
                assert(boundary);
                bool leavingToNorth = (p == 'J');
                if (enteredFromNorth == leavingToNorth)
                    inside = !inside;
                boundary = false;
            }
            else if (p == 'F' || p == 'L')
            {
                inside = !inside;
                boundary = true;
                enteredFromNorth = (p == 'L');
            }
            else if (p == '-')
            {
                assert(boundary);
                //boundary = true;
            }
            s += dbg;
        }

        e.push_back(s);
    }
   // cout << endl << e << endl;

    return enclosed;
}


void run_day10()
{
    string sample1 =
R"(.....
.S-7.
.|.|.
.L-J.
.....)";
    string sample1b =
R"(-L|F7
7S-7|
L|7||
-L-J|
L|-JF)";

    string sample2 =
R"(..F7.
.FJ|.
SJ.L7
|F--J
LJ...)";
    string sample2b =
R"(7-F7-
.FJ|7
SJLL7
|F--J
LJ.LJ)";

    test(4, day10(READ(sample1)));
    test(4, day10(READ(sample1b)));
    test(8, day10(READ(sample2)));
    test(8, day10(READ(sample2b)));
    gogogo(day10(LOAD(10)));


    string sample3 =
R"(...........
.S-------7.
.|F-----7|.
.||.....||.
.||.....||.
.|L-7.F-J|.
.|..|.|..|.
.L--J.L--J.
...........)";
    string sample3b =
R"(..........
.S------7.
.|F----7|.
.||....||.
.||....||.
.|L-7F-J|.
.|..||..|.
.L--JL--J.
..........)";
    string sample4 =
R"(.F----7F7F7F7F-7....
.|F--7||||||||FJ....
.||.FJ||||||||L7....
FJL7L7LJLJ||LJ.L-7..
L--J.L7...LJS7F-7L7.
....F-J..F7FJ|L7L7L7
....L7.F7||L7|.L7L7|
.....|FJLJ|FJ|F7|.LJ
....FJL-7.||.||||...
....L---J.LJ.LJLJ...)";
    string sample5 =
R"(FF7FSF7F7F7F7F7F---7
L|LJ||||||||||||F--J
FL-7LJLJ||||||LJL-77
F--JF--7||LJLJ7F7FJ-
L---JF-JLJ.||-FJLJJ7
|F|F-JF---7F7-L7L|7|
|FFJF7L7F-JF7|JL---7
7-L-JL7||F7|L7F-7F7|
L.L7LFJ|||||FJL7||LJ
L7JLJL-JLJLJL--JLJ.L)";
    test(4, day10_2(READ(sample3)));
    test(4, day10_2(READ(sample3b)));
    test(8, day10_2(READ(sample4)));
    test(10, day10_2(READ(sample5)));
    gogogo(day10_2(LOAD(10)));
}