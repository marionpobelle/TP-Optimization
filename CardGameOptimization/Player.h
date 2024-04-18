#pragma once
#include <list>
#include "Card.h"
#include "CardManager.h"
#include "csvfile.h"

class Player
{
public:

	Player();

	std::vector<Card> GetHand();

	void IncrementHand();

	void IncrementMana(int additionalMana);

	void DecreasePV(int damage);

	int GetMana();

	int GetPV();

	void ResetPlayer();

	Card PlayHigherCostCard();

private:
	int _mana;
	int _pv;

	std::vector<Card> _deck;
	std::vector<Card> _baseDeck;
	std::vector<Card> _hand;

	void FillDeck();

	void FillHand();

	void WriteAmountOfCardsPerCostHistogram(std::vector<Card> deckData);
};

