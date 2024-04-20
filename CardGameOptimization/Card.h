#pragma once
#include <math.h>
#include <iostream>

class Card
{
public:

    Card(int atk, int def, bool provoc, bool trample, bool distortion);

    int GetCardATK();

    int GetCardDEF();

    int GetCardCurrentDEF();

    void DamageCardCurrentDEF(int damage);

    void HealCard();

    void RestoreAttack();

    void RemoveAttack();

    bool CanAttack();

    bool IsCardDead();

    float GetCardCost();

    bool GetCardProvoc();

    bool GetCardTrample();

    bool GetCardDistortion();

    bool IsEqual(Card card);

    void CopyCard(Card card);

    friend std::ostream& operator <<(std::ostream& r_os, Card const& r_a)
    {
        return r_os <<
            "{ "
            << "\"Cost\":" << r_a._cost << ","
            << "\"Attack\":" << r_a._atk << ","
            << "\"Defense\":" << r_a._maxDef << ","
            << "\"HasTaunt\":" << (r_a._provoc ? "true" : "false") << ","
            << "\"HasTrample\":" << (r_a._trample ? "true" : "false") << ","
            << "\"HasDistortion\":" << (r_a._distortion ? "true" : "false") << ","
            << "\"HasFirstStrike\":" << "false"
            << " }";
    }

private:
    float _cost;
    int _atk;
    int _maxDef;
    int _currentDef;
    bool _provoc;
    bool _trample;
    bool _distortion;

    bool _canAttack;

};