#include "Card.h"

Card::Card(int atk, int def) {
    _atk = atk;
    _def = def;
    _cost = ceil((_atk + _def) / 2.0f);
}

int Card::GetCardATK() {
    return _atk;
}

int Card::GetCardDEF() {
    return _def;
}

int Card::GetCardCost() {
    return _cost;
}

bool Card::IsEqual(Card card) {
    return (_atk == card.GetCardATK() && _def == card.GetCardDEF() && _cost == card.GetCardCost());
}

void Card::CopyCard(Card card) {
    _atk = card.GetCardATK();
    _def = card.GetCardDEF();
    _cost = card.GetCardCost();
}