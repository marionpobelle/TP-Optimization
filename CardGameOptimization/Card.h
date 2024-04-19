#pragma once
#include <math.h>
#include <iostream>

class Card
{
public:

    Card(int atk, int def, int provoc);

    int GetCardATK();

    int GetCardDEF();

    int GetCardCurrentDEF();

    void DamageCardCurrentDEF(int damage);

    void HealCard();

    bool IsCardDead();

    float GetCardCost();

    int GetCardProvoc();

    bool IsEqual(Card card);

    void CopyCard(Card card);

private:
    float _cost;
    int _atk;
    int _maxDef;
    int _currentDef;
    //0 is false, 1 is true
    int _provoc;

};