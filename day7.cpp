#include "pch.h"
#include "harness.h"
#include <assert.h>

namespace p1
{

array<int8_t, 128> init_card_values()
{
    array<int8_t, 128> cardValue;

    ranges::fill(cardValue, int8_t(0));
    cardValue['A'] = 14;
    cardValue['K'] = 13;
    cardValue['Q'] = 12;
    cardValue['J'] = 11;
    cardValue['T'] = 10;
    for (int i = '9'; i > '1'; --i)
        cardValue[i] = int8_t(i - '0');

    return cardValue;
}

const auto kCardValue = init_card_values();


struct Hand
{
    enum HandType
    {
        FiveOfKind,
        FourOfKind,
        FullHouse,
        ThreeOfKind,
        TwoPair,
        Pair,
        HighCard,

        HandType_Count,
    };

    static HandType Classify(const string& hand);

    Hand(const string& deal, int bet = 0) : mDeal(deal), mBet(bet), mType(Classify(deal)) { /**/ }

    bool operator<(const Hand& other) const
    {
        if (mType != other.mType)
            return mType > other.mType;

        for (size_t i = 0; i < size(mDeal); ++i)
        {
            const int ourCardScore = kCardValue[mDeal[i]];
            const int theirCardScore = kCardValue[other.mDeal[i]];
            if (ourCardScore != theirCardScore)
                return ourCardScore < theirCardScore;
        }

        return false;
    }

    string mDeal;
    int mBet;
    HandType mType;
};


Hand::HandType Hand::Classify(const string& hand)
{
    array<int8_t, 15> cardCount;
    ranges::fill(cardCount, int8_t(0));
    for (char c : hand)
        ++cardCount[kCardValue[c]];

    const auto largestCount = ranges::max(cardCount);
    if (largestCount == 5)
        return FiveOfKind;
    if (largestCount == 4)
        return FourOfKind;

    const auto numPairs = ranges::count(cardCount, 2);
    if (largestCount == 3)
    {
        if (numPairs == 1)
            return FullHouse;
        return ThreeOfKind;
    }

    if (numPairs == 2)
        return TwoPair;
    if (largestCount == 2)
        return Pair;

    return HighCard;
}

}

namespace p2
{

array<int8_t,128> init_card_values()
{
    array<int8_t, 128> cardValue;

    ranges::fill(cardValue, int8_t(0));
    cardValue['A'] = 14;
    cardValue['K'] = 13;
    cardValue['Q'] = 12;
    cardValue['T'] = 10;
    for (int i = '9'; i > '1'; --i)
        cardValue[i] = int8_t(i - '0');

    cardValue['J'] = 1;

    return cardValue;
}

const auto kCardValue = init_card_values();


struct Hand
{
    enum HandType
    {
        FiveOfKind,
        FourOfKind,
        FullHouse,
        ThreeOfKind,
        TwoPair,
        Pair,
        HighCard,

        HandType_Count,
    };

    static HandType Classify(const string& hand);

    Hand(const string& deal, int bet = 0) : mDeal(deal), mBet(bet), mType(Classify(deal)) { /**/ }

    bool operator<(const Hand& other) const
    {
        if (mType != other.mType)
            return mType > other.mType;

        for (size_t i = 0; i < size(mDeal); ++i)
        {
            const int ourCardScore = kCardValue[mDeal[i]];
            const int theirCardScore = kCardValue[other.mDeal[i]];
            if (ourCardScore != theirCardScore)
                return ourCardScore < theirCardScore;
        }

        return false;
    }

    string mDeal;
    int mBet;
    HandType mType;
};


Hand::HandType Hand::Classify(const string& hand)
{
    array<int8_t, 15> cardCount;
    ranges::fill(cardCount, int8_t(0));
    for (char c : hand)
    {
        if (c != 'J')
            ++cardCount[kCardValue[c]];
    }

    const auto numJokers = ranges::count(hand, 'J');

    const auto largestCount = ranges::max(cardCount);
    if (largestCount + numJokers == 5)
        return FiveOfKind;
    if (largestCount + numJokers == 4)
        return FourOfKind;

    const auto numPairs = ranges::count(cardCount, 2);
    if (largestCount == 3)
    {
        if (numPairs == 1 || numJokers > 0)
            return FullHouse;
        return ThreeOfKind;
    }
    if (largestCount == 2 && numJokers > 0)
    {
        if (numPairs == 2)
            return FullHouse;
        return ThreeOfKind;
    }
    if (largestCount == 1 && numJokers > 1)
        return ThreeOfKind;

    if (numPairs == 2)
        return TwoPair;
    if (largestCount+numJokers == 2)
        return Pair;

    assert(numJokers == 0);

    return HighCard;
}

}


template<typename HandType = p1::Hand>
int day7(const stringlist& input)
{
    vector<HandType> hands;
    for (auto& line : input)
    {
        auto parts = split(line, " ");
        string deal = parts[0];
        int bet = stoi(parts[1]);
        hands.emplace_back(deal, bet);
    }

    sort(begin(hands), end(hands));

    int result = 0;
    int rank = 1;
    for (const auto& hand : hands)
    {
        result += rank * hand.mBet;
        ++rank;
    }

    return result;
}


void run_day7()
{
    string sample =
R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";

    test(6440, day7(READ(sample)));
    gogogo(day7(LOAD(7)), 251806792);

    test(5905, day7<p2::Hand>(READ(sample)));
    gogogo(day7<p2::Hand>(LOAD(7)));
}