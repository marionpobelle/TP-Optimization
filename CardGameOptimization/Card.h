#pragma once
#include <math.h>
#include <iostream>

class Card
{
public:

    Card(int atk, int def, bool provoc, bool trample);

    int GetCardATK();

    int GetCardDEF();

    int GetCardCurrentDEF();

    void DamageCardCurrentDEF(int damage);

    void HealCard();

    bool IsCardDead();

    float GetCardCost();

    bool GetCardProvoc();

    bool GetCardTrample();

    bool IsEqual(Card card);

    void CopyCard(Card card);

private:
    float _cost;
    int _atk;
    int _maxDef;
    int _currentDef;
    bool _provoc;
    bool _trample;

};