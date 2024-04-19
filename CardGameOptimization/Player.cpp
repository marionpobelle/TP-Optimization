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
	_referenceDeck = _deck;
	WriteAmountOfCardsPerCostHistogram(_deck, "Amount of card per cost Deck1");
	WriteAmountOfCardsPerAtkHistogram(_deck, "Amount of card per atk Deck1");
	WriteAmountOfCardsPerDefHistogram(_deck, "Amount of card per def Deck1");
	WriteAmountOfCardsPerAbilityHistogram(_deck, "Amount of card per ability Deck1");
	FillHand();
	currentWinrate = 0.0f;
	referenceWinrate = 0.0f;
}

std::vector<Card> Player::GetHand() 
{
	return _hand;
}

std::vector<Card> Player::GetRefDeck() 
{
	return _referenceDeck;
}

void Player::FillDeck() 
{
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

void Player::MonteCarlo()
{
	int randomIndexInDeck = rand() % _baseDeck.size();
	_baseDeck.erase(_baseDeck.begin() + randomIndexInDeck);
	while(_baseDeck.size() < 30) {
		int randomIndexInSet = rand() % cardManager->setList.size();
		Card newCard = cardManager->setList[randomIndexInSet];
		int nbDuplicates = 0;
		for (int i = 0; i < _baseDeck.size(); i++)
		{
			if (newCard.IsEqual(_baseDeck[i])) {
				nbDuplicates++;
			}
		}
		if (nbDuplicates < 2)
		{
			_baseDeck.push_back(newCard);
		}
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

void Player::ResetPlayerGlobal() {
	if (currentWinrate > referenceWinrate)
	{
		referenceWinrate = currentWinrate;
		_referenceDeck = _baseDeck;
	}
	else if (currentWinrate <= referenceWinrate)
	{
		_baseDeck = _referenceDeck;
	}
	MonteCarlo();
	_deck = _baseDeck;
	_mana = 0;
	_pv = 20;
	_hand.clear();
	FillHand();
}

Card Player::PlayHigherCostCard() {
	Card higherCostCard = Card(0,0,0);
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
		else if (_hand[i].GetCardCost() > higherCostCard.GetCardCost() && _hand[i].GetCardCost() <= _mana)
		{
			higherCostCard.CopyCard(_hand[i]);
			indexHigherCostCard = i;
		}
	}
	if(!higherCostCard.IsEqual(Card(0,0,0))) _hand.erase(_hand.begin() + indexHigherCostCard);
	return higherCostCard;
}

void Player::WriteAmountOfCardsPerCostHistogram(std::vector<Card> deckData, std::string usedDeck) {
	csvfile csv(usedDeck + ".csv");
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

void Player::WriteAmountOfCardsPerAtkHistogram(std::vector<Card> deckData, std::string usedDeck) {
	csvfile csv(usedDeck + ".csv");
	csv << "Attack" << "Amount of cards" << endrow;
	std::vector<int> attackForEachCard = std::vector<int>();
	for (int i = 0; i < deckData.size(); i++) {
		attackForEachCard.push_back(deckData[i].GetCardATK());
	}
	int max_value = *max_element(attackForEachCard.begin(), attackForEachCard.end());
	std::vector<int> amountOfCards = std::vector<int>();
	for (int i = max_value; i >= 0; --i)
	{
		amountOfCards.push_back(std::count(attackForEachCard.begin(), attackForEachCard.end(), i));
	}
	std::reverse(amountOfCards.begin(), amountOfCards.end());
	for (int i = max_value; i >= 0; --i)
	{
		csv << i << amountOfCards[i] << endrow;
	}
}

void Player::WriteAmountOfCardsPerDefHistogram(std::vector<Card> deckData, std::string usedDeck) {
	csvfile csv(usedDeck + ".csv");
	csv << "Def" << "Amount of cards" << endrow;
	std::vector<int> defForEachCard = std::vector<int>();
	for (int i = 0; i < deckData.size(); i++) {
		defForEachCard.push_back(deckData[i].GetCardDEF());
	}
	int max_value = *max_element(defForEachCard.begin(), defForEachCard.end());
	std::vector<int> amountOfCards = std::vector<int>();
	for (int i = max_value; i >= 0; --i)
	{
		amountOfCards.push_back(std::count(defForEachCard.begin(), defForEachCard.end(), i));
	}
	std::reverse(amountOfCards.begin(), amountOfCards.end());
	for (int i = max_value; i >= 0; --i)
	{
		csv << i << amountOfCards[i] << endrow;
	}
}

void Player::WriteAmountOfCardsPerAbilityHistogram(std::vector<Card> deckData, std::string usedDeck) {
	csvfile csv(usedDeck + ".csv");
	csv << " " << "Amount of cards" << endrow;
	int amountOfNoAbility = 0;
	int amountOfProvoc = 0;
	for (int i = 0; i < deckData.size(); i++)
	{
		if (deckData[i].GetCardProvoc() == 1) { // Later here || for each ability to check if ther eis at least one
			if (deckData[i].GetCardProvoc() == 1) amountOfProvoc++;
		}
		else
		{
			amountOfNoAbility++;
		}

	}
	csv << "No ability" << amountOfNoAbility << endrow;
	csv << "Provoc" << amountOfProvoc << endrow;
}