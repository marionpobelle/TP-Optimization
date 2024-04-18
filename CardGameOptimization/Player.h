#pragma once
#include <list>
#include "Card.h"
#include "CardManager.h"
#include "csvfile.h"

class Player
{
public:

	float currentWinrate;
	float referenceWinrate;

	Player();

	std::vector<Card> GetHand();

	void IncrementHand();

	void IncrementMana(int additionalMana);

	void DecreasePV(int damage);

	int GetMana();

	int GetPV();

	std::vector<Card> GetRefDeck();

	void ResetPlayer();

	void ResetPlayerGlobal();

	Card PlayHigherCostCard();

	void WriteAmountOfCardsPerCostHistogram(std::vector<Card> deckData, std::string usedDeck);

private:
	int _mana;
	int _pv;

	std::vector<Card> _deck;
	std::vector<Card> _referenceDeck;
	std::vector<Card> _baseDeck;
	std::vector<Card> _hand;

	CardManager* cardManager = new CardManager();

	void FillDeck();

	void MonteCarlo();

	void FillHand();

	
};

