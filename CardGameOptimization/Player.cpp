#include "Player.h"

Player::Player() 
{
	_mana = 0;
	_pv = 20;
	_baseDeck = std::vector<Card>();
	_deck = std::vector<Card>();
	_hand = std::vector<Card>();
	FillDeck();
	_baseDeck = _deck;
	WriteAmountOfCardsPerCostHistogram(_deck);
	FillHand();
}

std::vector<Card> Player::GetHand() 
{
	return _hand;
}

void Player::FillDeck() 
{
	CardManager* cardManager = new CardManager();
	while (_deck.size() < 30)
	{
		int randomIndexInSet = rand() % cardManager->setList.size();
		Card newCard = cardManager->setList[randomIndexInSet];
		int nbDuplicates = 0;
		for (int i = 0; i < _deck.size(); i++)
		{
			if (newCard.IsEqual(_deck[i])) {
				nbDuplicates++;
			}
		}
		if (nbDuplicates < 2)
		{
			_deck.push_back(newCard);
		}
		nbDuplicates = 0;
	}
}

void Player::FillHand() 
{
	for (int i = 0; i < 4; i++)
	{
		int randomIndexInSet = rand() % _deck.size();
		Card newCard = _deck[randomIndexInSet];
		_hand.push_back(newCard);
		_deck.erase(_deck.begin() + randomIndexInSet);
	}
}

void Player::IncrementHand()
{
	int randomIndexInSet = rand() % _deck.size();
	Card newCard = _deck[randomIndexInSet];
	_hand.push_back(newCard);
	_deck.erase(_deck.begin() + randomIndexInSet);
}

void Player::IncrementMana(int additionalMana) 
{
	_mana += additionalMana;
}

void Player::DecreasePV(int damage)
{
	_pv -= damage;
}

int Player::GetMana()
{
	return _mana;
}

int Player::GetPV()
{
	return _pv;
}

void Player::ResetPlayer() {
	_deck = _baseDeck;
	_mana = 0;
	_pv = 20;
	_hand.clear();
	FillHand();
}

Card Player::PlayHigherCostCard() {
	Card higherCostCard = Card(0,0);
	int indexHigherCostCard = -1;
	for (int i = 0; i < _hand.size(); i++) {
		if (_hand[i].GetCardCost() == higherCostCard.GetCardCost()) 
		{
			int randomIndex = rand();
			if (randomIndex == 0)
			{
				higherCostCard.CopyCard(_hand[i]);
				indexHigherCostCard = i;
			}
			else continue;
		}
		else if (_hand[i].GetCardCost() > higherCostCard.GetCardCost())
		{
			higherCostCard.CopyCard(_hand[i]);
			indexHigherCostCard = i;
		}
	}
	_hand.erase(_hand.begin() + indexHigherCostCard);
	return higherCostCard;
}

void Player::WriteAmountOfCardsPerCostHistogram(std::vector<Card> deckData) {
	csvfile csv("AmountOfCardsPerCostData.csv");
	csv << "Cost" << "Amount of cards" << endrow;
	std::vector<int> costForEachCard = std::vector<int>();
	for (int i = 0; i < deckData.size(); i++) {
		costForEachCard.push_back(deckData[i].GetCardCost());
	}
	int max_value = *max_element(costForEachCard.begin(), costForEachCard.end());
	std::vector<int> amountOfCards = std::vector<int>();
	for (int i = max_value; i >= 0; --i)
	{
		amountOfCards.push_back(std::count(costForEachCard.begin(), costForEachCard.end(), i));
	}
	std::reverse(amountOfCards.begin(), amountOfCards.end());
	for (int i = max_value; i >= 0; --i)
	{
		csv << i << amountOfCards[i] << endrow;
	}
}