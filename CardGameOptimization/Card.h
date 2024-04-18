#pragma once
#include <math.h>
class Card
{
public:

    Card(int atk, int def);

    int GetCardATK();

    int GetCardDEF();

    int GetCardCost();

    bool IsEqual(Card card);

    void CopyCard(Card card);

private:
    int _cost;
    int _atk;
    int _def;
};