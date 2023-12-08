#include "pch.h"
#include "harness.h"


int day8(const stringlist& input)
{
    string directions = input[0];
    unordered_map<string, pair<string, string>> nodes;

    for (auto itLine = begin(input) + 2; itLine != end(input); ++itLine)
    {
        string name = itLine->substr(0, 3);
        string l = itLine->substr(7, 3);
        string r = itLine->substr(12, 3);
        nodes.try_emplace(name, l, r);
    }

    string curr = "AAA";
    int steps = 0;
    auto itDir = cbegin(directions);
    do {
        ++steps;

        if (*itDir == 'L')
            curr = nodes[curr].first;
        else
            curr = nodes[curr].second;

        ++itDir;
        if (itDir == end(directions))
            itDir = begin(directions);

    } while (curr != "ZZZ");

    return steps;
}


i64 day8_2(const stringlist& input)
{
    string directions = input[0];
    unordered_map<string, pair<string, string>> nodes;
    vector<string> ghosts;

    for (auto itLine = begin(input) + 2; itLine != end(input); ++itLine)
    {
        string name = itLine->substr(0, 3);
        string l = itLine->substr(7, 3);
        string r = itLine->substr(12, 3);
        nodes.try_emplace(name, l, r);

        if (name[2] == 'A')
            ghosts.push_back(name);
    }

    auto getSeqLen = [&directions, &nodes](const string& start) -> i64
    {
        string curr = start;

        i64 steps = 0;
        auto itDir = cbegin(directions);
        do {
            ++steps;

            if (*itDir == 'L')
                curr = nodes[curr].first;
            else
                curr = nodes[curr].second;

            ++itDir;
            if (itDir == end(directions))
                itDir = begin(directions);

        } while (curr[2] != 'Z');

        return steps;
    };

    vector<i64> seqLen;
    ranges::transform(ghosts, back_inserter(seqLen), getSeqLen);
    
    i64 steps = accumulate(begin(seqLen), end(seqLen), 1ll, [](i64 v, i64 a) { return lcm(v, a); });
    return steps;
}


void run_day8()
{
    string sample =
R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";
    string sample2 =
R"(LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ))";

    test(2, day8(READ(sample)));
    test(6, day8(READ(sample2)));
    gogogo(day8(LOAD(8)));


    string sample3 =
R"(LR

11A = (11B, XXX)
11B = (XXX, 11Z)
11Z = (11B, XXX)
22A = (22B, XXX)
22B = (22C, 22C)
22C = (22Z, 22Z)
22Z = (22B, 22B)
XXX = (XXX, XXX))";

    test(6, day8_2(READ(sample3)));
    gogogo(day8_2(LOAD(8)));
}