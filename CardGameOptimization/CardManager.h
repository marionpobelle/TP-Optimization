#pragma once
#include <vector>
#include <algorithm>
#include "Card.h"

class CardManager
{
public:

    std::vector<Card> setList;

    CardManager();

    std::vector<Card> GenerateCards();

    void WriteSetList();
};