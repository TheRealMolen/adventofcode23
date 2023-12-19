#include "pch.h"
#include "harness.h"

#include <execution>

//
// pt2 ideas
// 1. brute force
// 2. for N unwraps of S, assume `total = arr(S) * (max(arr(S?), arr(?S))**(N-1))
// 3. recursively iterate thru record & runs simultaneously
// 4. runlength encode inputs to reduce validation cost


struct Block
{
    u8 pre = 0;    // num ? before this run of ###
    u8 len = 0;    // num # in this block
    u8 post = 0;   // num ? following this block

    bool tied = false;  // if true, our post is the next block's pre


    void addSpring(char c)
    {
        if (c == '?')
        {
            if (len)
                ++post;
            else
                ++pre;
        }
        else
        {
            assert(c == '#');
            ++len;
        }
    }
};

vector<Block> decompose(const string& record)
{
    vector<Block> blocks;
    const char* recEnd = data(record) + size(record);
    
    bool terminated = false;
    for (const char* pc = data(record); pc != recEnd; /**/)
    {
        while (*pc == '.')
        {
            ++pc;
            terminated = true;
        }

        Block b;
        while (*pc && *pc != '.')
        {
            b.addSpring(*pc);
            ++pc;
        }
        blocks.push_back(b);
    }

    return blocks;
}


struct RunInfo
{
    u8 len = 0; // how long is this run?
    u8 minLenFromHere = 0; // what's the sum of all the subsequent runs and separators?
};


bool record_matches(const string& record, const vector<int>& runs)
{
    auto itRun = data(runs);
    auto itEndRuns = data(runs) + size(runs);

    int runLen = 0;
    for (char c : record)
    {
        if (c == '#')
            ++runLen;
        else if (runLen > 0)
        {
            if (itRun == itEndRuns)
                return false;
            if (*itRun != runLen)
                return false;

            ++itRun;
            runLen = 0;
        }
    }

    if (runLen > 0)
    {
        if (itRun == itEndRuns)
            return false;
        if (*itRun != runLen)
            return false;
        ++itRun;
    }

    if (itRun != itEndRuns)
        return false;

    return true;
}

#define CADBG_ENABLEx

#ifdef CADBG_ENABLE
struct CADbg
{
    const char* recordStart;
    string expanded;

    void expand(const char* recordCurr, char c)
    {
        auto offs = recordCurr - recordStart;
        expanded[offs] = c;
    }
};
#define CADBG_ARG_DEF   ,CADbg& dbg
#define CADBG_ARG       ,dbg
#define CADBG_EXPAND(c) dbg.expand(curr, c)
#define CADBG_FOUND     cout << format("found: {}", dbg.expanded) << endl
#define CADBG_INIT(rec) CADbg dbg{ rec.c_str(), rec };  \
                        cout << "starting hunt thru " << rec << endl
#define CADBG_EXPOL     execution::seq
#else
#define CADBG_ARG_DEF   
#define CADBG_ARG       
#define CADBG_EXPAND(c) do{}while(0)
#define CADBG_FOUND     do{}while(0)
#define CADBG_INIT(rec) do{}while(0)
#define CADBG_EXPOL     execution::par_unseq
#endif

i64 count_arrangements_inner(const char* curr, const char* recEnd, int runLenSoFar, const RunInfo* itRun, const RunInfo* itRunEnd CADBG_ARG_DEF)
{
    const char c = *curr;
    if (!c)
    {
        if (itRun == itRunEnd)
        {
            CADBG_FOUND;
            return 1;
        }
        if (itRun->len == runLenSoFar && itRun+1 == itRunEnd)
        {
            CADBG_FOUND;
            return 1;
        }

        return 0;
    }

    if (itRun != itRunEnd)
    {
        if (runLenSoFar > itRun->len)
            return 0;

        const auto recLenLeft = recEnd - curr;
        if (recLenLeft + runLenSoFar < itRun->minLenFromHere)
            return 0;
    }

    if (c == '#')
    {
        if (itRun == itRunEnd)
            return 0;

        ++curr;
        ++runLenSoFar;
        while (*curr && *curr != '.' && runLenSoFar < itRun->len)
        {
            CADBG_EXPAND('#');
            ++curr;
            ++runLenSoFar;
        }

        return count_arrangements_inner(curr, recEnd, runLenSoFar, itRun, itRunEnd CADBG_ARG);
    }

    if (c == '.')
    {
        ++curr;
        while (*curr && *curr == '.')
            ++curr;

        if (runLenSoFar == 0)
            return count_arrangements_inner(curr, recEnd, 0, itRun, itRunEnd CADBG_ARG);

        if (itRun != itRunEnd && runLenSoFar == itRun->len)
            return count_arrangements_inner(curr, recEnd, 0, itRun+1, itRunEnd CADBG_ARG);

        return 0;
    }

    // must be a ?; try both options
    i64 numArrangements = 0;

    if (itRun != itRunEnd)
    {
        // #
        CADBG_EXPAND('#');
        numArrangements += count_arrangements_inner(curr+1, recEnd, runLenSoFar+1, itRun, itRunEnd CADBG_ARG);
    }

    // .
    CADBG_EXPAND('.');
    if (itRun != itRunEnd && runLenSoFar == itRun->len)
        numArrangements += count_arrangements_inner(curr+1, recEnd, 0, itRun+1, itRunEnd CADBG_ARG);
    else if (runLenSoFar == 0)
        numArrangements += count_arrangements_inner(curr+1, recEnd, 0, itRun, itRunEnd CADBG_ARG);

    return numArrangements;
}


string unwrapRecord(const string& ogRecord, int numUnwraps)
{
    string unwrapped;
    unwrapped.reserve(numUnwraps * (1 + size(ogRecord)));
    unwrapped = ogRecord;
    for (int i = 1; i < numUnwraps; ++i)
    {
        unwrapped += "?";
        unwrapped += ogRecord;
    }
    return unwrapped;
}

vector<RunInfo> unwrapRuns(const vector<RunInfo>& ogRuns, int numUnwraps)
{
    vector<RunInfo> unwrapped;
    unwrapped.reserve(numUnwraps * size(ogRuns));
    for (int i = 0; i < numUnwraps; ++i)
        ranges::copy(ogRuns, back_inserter(unwrapped));
    return unwrapped;
}


vector<RunInfo> loadRuns(const string& s)
{
    const vector<int> rawRuns = string_to_intlist<int>(s, ',');

    vector<RunInfo> runs;
    runs.reserve(size(rawRuns));

    for (int run : rawRuns)
        runs.emplace_back(u8(run), u8(0));

    u8 maxLen = 0;
    for (RunInfo& run : views::reverse(runs))
    {
        maxLen += run.len;
        run.minLenFromHere = maxLen;
        ++maxLen;
    }

    return runs;
}

string tidy_record(const string& s)
{
    string rec;
    for (char c : s)
    {
        if (c != '.')
            rec += c;
        else if (rec.empty() || rec.back() != '.')
            rec += c;
    }
    return rec;
}


template<int NumUnwraps=1>
i64 count_arrangements(const string& report)
{
    auto parts = split(report, " ");
    const string record = tidy_record(parts[0]);
    const vector<int> rawRuns = string_to_intlist<int>(parts[1], ',');
    const vector<RunInfo> runs = loadRuns(parts[1]);

    auto countForRecord = [](const string& rec, const auto& runs) -> i64
    {
        CADBG_INIT(rec);
        return count_arrangements_inner(rec.c_str(), rec.c_str() + rec.length(), 0, data(runs), data(runs) + size(runs) CADBG_ARG);
    };

    return countForRecord(unwrapRecord(record, NumUnwraps), unwrapRuns(runs, NumUnwraps));
    /*
    if constexpr (NumUnwraps == 1)
        return countForRecord(record, runs);
    else
    {
        i64 baseCount = countForRecord(record, runs);
        i64 oneUnwrapCount = countForRecord(unwrapRecord(record, 2), unwrapRuns(runs, 2));
        if (baseCount*baseCount == oneUnwrapCount)
        {
            i64 total = baseCount;
            for (int i = 0; i < NumUnwraps; ++i)
                total *= baseCount;
            return total;
        }

        return -1;
    }*/
}

i64 count_arrangements_og(const string& report)
{
    auto parts = split(report, " ");
    string record = parts[0];
    auto runs = string_to_intlist<int>(parts[1], ',');
    const char* recordBegin = data(record);
    const char* recordEnd = recordBegin + size(record);

    vector<u8> unknowns;
    for (u8 i = 0; i < size(record); ++i)
        if (record[i] == '?')
            unknowns.push_back(i);

    const u8* itBeginUnknowns = data(unknowns);
    auto itEndUnknowns = itBeginUnknowns + size(unknowns);

    int numArrangements = 0;
    int numPossibilities = 1 << isize(unknowns);
    string test = record;
    char* ptest = data(test);
    for (int p=0; p<numPossibilities; ++p)
    {
        copy(recordBegin, recordEnd, ptest);

        int mask = 1;
        for (auto itUnknown= itBeginUnknowns; itUnknown!= itEndUnknowns; ++itUnknown, mask <<= 1)
            ptest[*itUnknown] = (p & mask) ? '#' : '.';

        if (record_matches(test, runs))
            ++numArrangements;
    }

    return numArrangements;
}


i64 day12(const stringlist& input)
{
    return transform_reduce(CADBG_EXPOL, cbegin(input), cend(input), 0LL, plus{}, count_arrangements<1>);
}


i64 day12_2(const stringlist& input)
{
    return transform_reduce(CADBG_EXPOL, cbegin(input), cend(input), 0LL, plus{}, count_arrangements<5>);
}


void run_day12()
{
    string sample =
R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

    test(3, count_arrangements("???#?..??# 2,1,1"));
    test(1, count_arrangements("???.### 1,1,3"));
    test(4, count_arrangements(".??..??...?##. 1,1,3"));
    test(10, count_arrangements("?###???????? 3,2,1"));
    test(21, day12(READ(sample)));
    nononoD(day12(LOAD(12)), 7307ll);

    test(1, count_arrangements<5>("???.### 1,1,3"));
    test(16384, count_arrangements<5>(".??..??...?##. 1,1,3"));
    test(506250, count_arrangements<5>("?###???????? 3,2,1"));
    test(525152, day12_2(READ(sample)));
    skip("cos i'm stumped for now");
    //gogogo(day12_2(LOAD(12)));
}