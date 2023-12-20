#include "pch.h"
#include "harness.h"


int runHASH(const string_view& s)
{
    u8 h = 0;
    for (char c : s)
        h = (h + u8(c)) * 17;
    return h;
}

struct Lens
{
    string label;
    int focus = -1;
};

struct HashBox
{
    vector<Lens> lenses;

    void AddLens(const string_view label, int focus)
    {
        for (Lens& lens : lenses)
        {
            if (lens.label == label)
            {
                lens.focus = focus;
                return;
            }
        }

        lenses.emplace_back(string(label), focus);
    }

    void RemoveLens(const string_view label)
    {
        erase_if(lenses, [label](const Lens& lens) { return lens.label == label; });
    }
};


int day15(const stringlist& input)
{
    int sum = 0;
    for (const string& step : split(input.front(), ","))
        sum += runHASH(step);

    return sum;
}

i64 day15_2(const stringlist& input)
{
    array<HashBox, 256> boxes;

    for (const string& step : split(input.front(), ","))
    {
        if (step.back() == '-')
        {
            string_view label(begin(step), end(step) - 1);
            int box = runHASH(label);

            boxes[box].RemoveLens(label);
        }
        else
        {
            string_view label(begin(step), end(step) - 2);
            int box = runHASH(label);
            int focus = step.back() - '0';

            boxes[box].AddLens(label, focus);
        }
    }

    i64 power = 0;
    int boxNum = 0;
    for (const HashBox& box : boxes)
    {
        ++boxNum;

        for (int slot = 0; slot < isize(box.lenses); ++slot)
        {
            const Lens& lens = box.lenses[slot];
            power += boxNum * (slot + 1) * lens.focus;
        }
    }

    return power;
}


void run_day15()
{
    string sample = "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7";

    test(1320, day15(READ(sample)));
    gogogo(day15(LOAD(15)));

    test(145, day15_2(READ(sample)));
    gogogo(day15_2(LOAD(15)));
}