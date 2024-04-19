#include "Card.h"

Card::Card(int atk, int def, bool provoc, bool trample) {
    _atk = atk;
    _maxDef = def;
    _provoc = provoc;
    _trample = trample;
    _cost = ceil(((_atk + _maxDef) / 2.0f) + provoc * 1.5f + trample);
}

int Card::GetCardATK() {
    return _atk;
}

int Card::GetCardDEF() {
    return _maxDef;
}

int Card::GetCardCurrentDEF() {
    return _currentDef;
}

void Card::DamageCardCurrentDEF(int damage) {
    _currentDef -= damage;
}

void Card::HealCard() {
    _currentDef = _maxDef;
}

bool Card::IsCardDead() {
    return _currentDef <= 0;
}

float Card::GetCardCost() {
    return _cost;
}

bool Card::GetCardProvoc() {
    return _provoc;
}

bool Card::GetCardTrample() {
    return _trample;
}

bool Card::IsEqual(Card card) {
    return (_atk == card.GetCardATK() && _maxDef == card.GetCardDEF() && _cost == card.GetCardCost() && _provoc == card.GetCardProvoc() && _trample == card.GetCardTrample());
}

void Card::CopyCard(Card card) {
    _atk = card.GetCardATK();
    _maxDef = card.GetCardDEF();
    _cost = card.GetCardCost();
    _provoc = card.GetCardProvoc();
    _trample = card.GetCardTrample();
}