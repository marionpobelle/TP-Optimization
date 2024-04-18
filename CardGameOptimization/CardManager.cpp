#include "CardManager.h"
#include "csvfile.h"

CardManager::CardManager()
{
    setList = GenerateCards();
    WriteSetList();
}

std::vector<Card> CardManager::GenerateCards()
{
    std::vector<Card> cards = std::vector<Card>();
    for (int atk = 0; atk <= 12; atk++)
    {
        for (int def = 1; def <= 12; def++)
        {
            Card card = Card(atk, def);
            if (card.GetCardCost() <= 6)
            {
                cards.push_back(card);
            }
        }
    }
    return cards;
}

void CardManager::WriteSetList()
{
    csvfile csv("SetListData.csv");
    csv << "Cost" << "Attack" << "Defense" << endrow;
    std::sort(setList.begin(), setList.end(), [](Card a, Card b) { return a.GetCardCost() < b.GetCardCost(); });
    for (int i = 0; i < setList.size(); i++)
    {
        csv << setList[i].GetCardCost() << setList[i].GetCardATK() << setList[i].GetCardDEF() << endrow;
    }
}