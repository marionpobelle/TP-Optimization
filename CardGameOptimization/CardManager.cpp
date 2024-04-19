#include "CardManager.h"
#include "csvfile.h"

CardManager::CardManager()
{
    setList = GenerateCards();
    WriteSetList();
}

std::vector<Card> CardManager::GenerateCards()
{
    //Simple cards
    std::vector<Card> cards = std::vector<Card>();
    for (int atk = 0; atk <= 15; atk++)
    {
        for (int def = 1; def <= 16; def++)
        {
            Card card = Card(atk, def, 0);
            if (card.GetCardCost() <= 8)
            {
                cards.push_back(card);
            }
        }
    }
    //Provoc cards
    for (int atk = 0; atk <= 15; atk++)
    {
        for (int def = 1; def <= 16; def++)
        {
            Card card = Card(atk, def, 1);
            if (card.GetCardCost() <= 8)
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
    csv << "Cost" << "Attack" << "Defense" << "Provoc" << endrow;
    std::sort(setList.begin(), setList.end(), [](Card a, Card b) { return a.GetCardCost() < b.GetCardCost(); });
    for (int i = 0; i < setList.size(); i++)
    {
        csv << setList[i].GetCardCost() << setList[i].GetCardATK() << setList[i].GetCardDEF() << setList[i].GetCardProvoc() << endrow;
    }
}