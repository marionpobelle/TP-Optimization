// CardGameOptimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "CardManager.h"
#include "Player.h"
#include <time.h>
#include <algorithm>
#include "csvfile.h"
#include <numeric>

Player* player1 = new Player();
Player* player2 = new Player();

std::vector<Card> player1Board = std::vector<Card>();
std::vector<Card> player2Board = std::vector<Card>();

int BoardAttackSum(std::vector<Card> playerBoard);
int GetFirstProvocCardIndex(std::vector<Card> playerBoard);
bool CheckForProvoc(std::vector<Card> playerBoard);

void ResetGameWithoutDeckChange();
void ResetGameGlobal();

void WriteAmountOfTurnsPerGameHistogram(std::vector<int>& nbTurnData);
void WriteAmountOfTurnsPerIteHistogram(std::vector<int>& averageTurnData);
void WriteWinratePerAmountOfGames(std::vector<float>& winRateData, int nbGames);
void WriteWinrateRefPerIteration(std::vector<float>& winRateData, int nbIte);

int main()
{
    srand(time(NULL));
    std::vector<float> winrateRefData = std::vector<float>();
    std::vector<int> averageTurnPerIteData = std::vector<int>();

    for (int k= 0; k < 1000; k++) 
    {
        int player1Winrate = 0;
        std::vector<int> nbTurnPerGame = std::vector<int>();
        std::vector<float> winratePerAmountOfGames = std::vector<float>();
        for (int i = 0; i < 1000; i++)
        {
            int nbTurns = 0;
            while (player1->GetPV() > 0 && player2->GetPV() > 0)
            {
                if (i < 500)
                {
                    //PLAYER 1 TURN
                    int trampleDamage = 0;
                    player1->IncrementMana(1);
                    player1->IncrementHand();
                    Card cardToPlay = player1->PlayHigherCostCard();
                    if (cardToPlay.IsEqual(Card(0, 0, false, false, false, false))) 
                    {
                        //Do nothing
                    }
                    else 
                    {
                        player1Board.push_back(cardToPlay);
                        //PROVOC STUFF HAPPENS HERE
                        while (CheckForProvoc(player2Board) && player1Board.size() > 0)
                        {
                            int indexProvoc = GetFirstProvocCardIndex(player2Board);
                            int attackingCardIndex = 0;
                            while (!player2Board[indexProvoc].IsCardDead() && player1Board.size() > 0)
                            {
                                if (player1Board[attackingCardIndex].GetCardDistortion() == true && player2Board[indexProvoc].GetCardDistortion() == false) 
                                {
                                    attackingCardIndex++;
                                }
                                else
                                {
                                    if (player1Board[attackingCardIndex].GetCardATK() > player2Board[indexProvoc].GetCardDEF() && player1Board[attackingCardIndex].GetCardTrample())
                                    {
                                        trampleDamage = player1Board[attackingCardIndex].GetCardATK() - player2Board[indexProvoc].GetCardDEF();
                                        player1Board[attackingCardIndex].RemoveAttack();
                                    }
                                    //FIRST STRIKE STUFF HERE
                                    if (player1Board[attackingCardIndex].GetCardFirstStrike() && !player2Board[indexProvoc].GetCardFirstStrike())
                                    {
                                        player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        player1Board[attackingCardIndex].RemoveAttack();
                                        if (!player2Board[indexProvoc].IsCardDead())
                                        {
                                            player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        }
                                        if (player1Board[attackingCardIndex].IsCardDead())
                                        {
                                            player1Board.erase(player1Board.begin() + attackingCardIndex);
                                            trampleDamage = 0;
                                        }
                                        attackingCardIndex++;
                                    }
                                    else if (player2Board[indexProvoc].GetCardFirstStrike() && !player1Board[attackingCardIndex].GetCardFirstStrike())
                                    {
                                        player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        if (!player1Board[attackingCardIndex].IsCardDead())
                                        {
                                            player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        }
                                        attackingCardIndex++;
                                    }
                                    else
                                    {
                                        player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        player1Board[attackingCardIndex].RemoveAttack();
                                        if (player1Board[attackingCardIndex].IsCardDead()) player1Board.erase(player1Board.begin() + attackingCardIndex);
                                        attackingCardIndex++;
                                    }
                                    //
                                   
                                }
                                
                            }
                            if (player2Board[indexProvoc].IsCardDead()) player2Board.erase(player2Board.begin() + indexProvoc);
                        }
                        //
                    }
                    int player1InflictedDamage = BoardAttackSum(player1Board) + trampleDamage;
                    player2->DecreasePV(player1InflictedDamage);
                    if (player2->GetPV() <= 0) continue;
                    trampleDamage = 0;
                    //PLAYER 2 TURN
                    player2->IncrementMana(1);
                    player2->IncrementHand();
                    cardToPlay = player2->PlayHigherCostCard();
                    if (cardToPlay.IsEqual(Card(0, 0, false, false, false, false)))
                    {
                        //Do nothing
                    }
                    else
                    {
                        player2Board.push_back(cardToPlay);
                        //PROVOC STUFF HAPPENS HERE
                        while (CheckForProvoc(player1Board) && player2Board.size() > 0)
                        {
                            int indexProvoc = GetFirstProvocCardIndex(player1Board);
                            int attackingCardIndex = 0;
                            while (!player1Board[indexProvoc].IsCardDead() && player2Board.size() > 0)
                            {
                                if(player2Board[attackingCardIndex].GetCardDistortion() == true && player1Board[indexProvoc].GetCardDistortion() == false)
                                {
                                    attackingCardIndex++;
                                }
                                else 
                                {
                                    if (player2Board[attackingCardIndex].GetCardATK() > player1Board[indexProvoc].GetCardDEF() && player2Board[attackingCardIndex].GetCardTrample())
                                    {
                                        trampleDamage = player2Board[attackingCardIndex].GetCardATK() - player1Board[indexProvoc].GetCardDEF();
                                        player2Board[attackingCardIndex].RemoveAttack();
                                    }
                                    //FIRST STRIKE STUFF HERE
                                    if (player2Board[attackingCardIndex].GetCardFirstStrike() && !player1Board[indexProvoc].GetCardFirstStrike())
                                    {
                                        player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        player2Board[attackingCardIndex].RemoveAttack();
                                        if (!player1Board[indexProvoc].IsCardDead())
                                        {
                                            player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        }
                                        if (player2Board[attackingCardIndex].IsCardDead())
                                        {
                                            player2Board.erase(player2Board.begin() + attackingCardIndex);
                                            trampleDamage = 0;
                                        }
                                        attackingCardIndex++;
                                    }
                                    else if (player1Board[indexProvoc].GetCardFirstStrike() && !player2Board[attackingCardIndex].GetCardFirstStrike())
                                    {
                                        player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        if (!player2Board[attackingCardIndex].IsCardDead())
                                        {
                                            player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        }
                                        attackingCardIndex++;
                                    }
                                    else
                                    {
                                        player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        player2Board[attackingCardIndex].RemoveAttack();
                                        if (player2Board[attackingCardIndex].IsCardDead()) player2Board.erase(player2Board.begin() + attackingCardIndex);
                                        attackingCardIndex++;
                                    }
                                    //
                                }    
                            }
                            if (player1Board[indexProvoc].IsCardDead()) player1Board.erase(player1Board.begin() + indexProvoc);
                        }
                        //
                    }
                    int player2InflictedDamage = BoardAttackSum(player2Board) + trampleDamage;
                    player1->DecreasePV(player2InflictedDamage);
                    //Healing cards
                    for (Card card : player1Board) 
                    {
                        card.HealCard();
                    }
                    for (Card card : player2Board)
                    {
                        card.HealCard();
                    }
                    //Restoring attack for cards
                    for (Card card : player1Board)
                    {
                        card.RestoreAttack();
                    }
                    for (Card card : player2Board)
                    {
                        card.RestoreAttack();
                    }
                    trampleDamage = 0;
                }
                else if (i >= 500)
                {
                    //PLAYER 2 TURN
                    int trampleDamage = 0;
                    player2->IncrementMana(1);
                    player2->IncrementHand();
                    Card cardToPlay = player2->PlayHigherCostCard();
                    if (cardToPlay.IsEqual(Card(0, 0, false, false, false, false)))
                    {
                        //Do nothing
                    }
                    else
                    {
                        player2Board.push_back(cardToPlay);
                        //PROVOC STUFF HAPPENS HERE
                        while (CheckForProvoc(player1Board) && player2Board.size() > 0)
                        {
                            int indexProvoc = GetFirstProvocCardIndex(player1Board);
                            int attackingCardIndex = 0;
                            while (!player1Board[indexProvoc].IsCardDead() && player2Board.size() > 0)
                            {
                                if (player2Board[attackingCardIndex].GetCardDistortion() == true && player1Board[indexProvoc].GetCardDistortion() == false)
                                {
                                    attackingCardIndex++;
                                }
                                else
                                {
                                    if (player2Board[attackingCardIndex].GetCardATK() > player1Board[indexProvoc].GetCardDEF() && player2Board[attackingCardIndex].GetCardTrample())
                                    {
                                        trampleDamage = player2Board[attackingCardIndex].GetCardATK() - player1Board[indexProvoc].GetCardDEF();
                                        player2Board[attackingCardIndex].RemoveAttack();
                                    }
                                    //FIRST STRIKE STUFF HERE
                                    if (player2Board[attackingCardIndex].GetCardFirstStrike() && !player1Board[indexProvoc].GetCardFirstStrike())
                                    {
                                        player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        player2Board[attackingCardIndex].RemoveAttack();
                                        if (!player1Board[indexProvoc].IsCardDead())
                                        {
                                            player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        }
                                        if (player2Board[attackingCardIndex].IsCardDead())
                                        {
                                            player2Board.erase(player2Board.begin() + attackingCardIndex);
                                            trampleDamage = 0;
                                        }
                                        attackingCardIndex++;
                                    }
                                    else if (player1Board[indexProvoc].GetCardFirstStrike() && !player2Board[attackingCardIndex].GetCardFirstStrike())
                                    {
                                        player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        if (!player2Board[attackingCardIndex].IsCardDead())
                                        {
                                            player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        }
                                        attackingCardIndex++;
                                    }
                                    else
                                    {
                                        player1Board[indexProvoc].DamageCardCurrentDEF(player2Board[attackingCardIndex].GetCardATK());
                                        player2Board[attackingCardIndex].DamageCardCurrentDEF(player1Board[indexProvoc].GetCardATK());
                                        player2Board[attackingCardIndex].RemoveAttack();
                                        if (player2Board[attackingCardIndex].IsCardDead()) player2Board.erase(player2Board.begin() + attackingCardIndex);
                                        attackingCardIndex++;
                                    }
                                    //
                                }
                            }
                            if (player1Board[indexProvoc].IsCardDead()) player1Board.erase(player1Board.begin() + indexProvoc);
                        }
                        //
                    }
                    
                    int player2InflictedDamage = BoardAttackSum(player2Board) + trampleDamage;
                    player1->DecreasePV(player2InflictedDamage);
                    if (player1->GetPV() <= 0) continue;
                    trampleDamage = 0;
                    //PLAYER 1 TURN
                    player1->IncrementMana(1);
                    player1->IncrementHand();
                    cardToPlay = player1->PlayHigherCostCard();
                    if (cardToPlay.IsEqual(Card(0, 0, false, false, false, false)))
                    {
                        //Do nothing
                    }
                    else
                    {
                        player1Board.push_back(cardToPlay);
                        //PROVOC STUFF HAPPENS HERE
                        while (CheckForProvoc(player2Board) && player1Board.size() > 0)
                        {
                            int indexProvoc = GetFirstProvocCardIndex(player2Board);
                            int attackingCardIndex = 0;
                            while (!player2Board[indexProvoc].IsCardDead() && player1Board.size() > 0)
                            {
                                if(player1Board[attackingCardIndex].GetCardDistortion() == true && player2Board[indexProvoc].GetCardDistortion() == false)
                                {
                                    attackingCardIndex++;
                                }
                                else
                                {
                                    if (player1Board[attackingCardIndex].GetCardATK() > player2Board[indexProvoc].GetCardDEF() && player1Board[attackingCardIndex].GetCardTrample())
                                    {
                                        trampleDamage = player1Board[attackingCardIndex].GetCardATK() - player2Board[indexProvoc].GetCardDEF();
                                        player1Board[attackingCardIndex].RemoveAttack();
                                    }
                                    //FIRST STRIKE STUFF HERE
                                    if (player1Board[attackingCardIndex].GetCardFirstStrike() && !player2Board[indexProvoc].GetCardFirstStrike())
                                    {
                                        player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        player1Board[attackingCardIndex].RemoveAttack();
                                        if (!player2Board[indexProvoc].IsCardDead())
                                        {
                                            player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        }
                                        if (player1Board[attackingCardIndex].IsCardDead())
                                        {
                                            player1Board.erase(player1Board.begin() + attackingCardIndex);
                                            trampleDamage = 0;
                                        }
                                        attackingCardIndex++;
                                    }
                                    else if (player2Board[indexProvoc].GetCardFirstStrike() && !player1Board[attackingCardIndex].GetCardFirstStrike())
                                    {
                                        player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        if (!player1Board[attackingCardIndex].IsCardDead())
                                        {
                                            player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        }
                                        attackingCardIndex++;
                                    }
                                    else
                                    {
                                        player2Board[indexProvoc].DamageCardCurrentDEF(player1Board[attackingCardIndex].GetCardATK());
                                        player1Board[attackingCardIndex].DamageCardCurrentDEF(player2Board[indexProvoc].GetCardATK());
                                        player1Board[attackingCardIndex].RemoveAttack();
                                        if (player1Board[attackingCardIndex].IsCardDead()) player1Board.erase(player1Board.begin() + attackingCardIndex);
                                        attackingCardIndex++;
                                    }
                                    //

                                }

                            }
                            if (player2Board[indexProvoc].IsCardDead()) player2Board.erase(player2Board.begin() + indexProvoc);
                        }
                        //
                    }
                    int player1InflictedDamage = BoardAttackSum(player1Board) + trampleDamage;
                    player2->DecreasePV(player1InflictedDamage);
                    //Healing cards
                    for (Card card : player1Board)
                    {
                        card.HealCard();
                    }
                    for (Card card : player2Board)
                    {
                        card.HealCard();
                    }
                    //Restoring attack for cards
                    for (Card card : player1Board)
                    {
                        card.RestoreAttack();
                    }
                    for (Card card : player2Board)
                    {
                        card.RestoreAttack();
                    }
                }
                nbTurns++;
            }
            if (player1->GetPV() > 0 && player2->GetPV() <= 0)
            {
                player1Winrate++;
            }
            winratePerAmountOfGames.push_back((float)((player1Winrate / (i + 1.0f)) * 100.0f));
            nbTurnPerGame.push_back(nbTurns);
            ResetGameWithoutDeckChange();
            
        }
        //WriteAmountOfTurnsPerGameHistogram(nbTurnPerGame);
        //WriteWinratePerAmountOfGames(winratePerAmountOfGames, 1000);
        averageTurnPerIteData.push_back((ceil)(std::accumulate(nbTurnPerGame.begin(), nbTurnPerGame.end(), 0.0) / nbTurnPerGame.size()));
        nbTurnPerGame.clear();

        player1Winrate = (float)((player1Winrate / 1000.0f) * 100.0f);
        std::cout << "Iteration : " << k << " Player 1 winrate is : " << player1Winrate << std::endl;
        player1->currentWinrate = player1Winrate;
        ResetGameGlobal();
        winrateRefData.push_back((float)player1->referenceWinrate);
        
    }
    player1->WriteAmountOfCardsPerCostHistogram(player1->GetRefDeck(), "Amount of card per cost Deck2");
    player1->WriteAmountOfCardsPerAtkHistogram(player1->GetRefDeck(), "Amount of card per attack Deck2");
    player1->WriteAmountOfCardsPerDefHistogram(player1->GetRefDeck(), "Amount of card per def Deck2");
    player1->WriteAmountOfCardsPerAbilityHistogram(player1->GetRefDeck(), "Amount of card per ability Deck2");
    player1->WriteJSONDeckFile();
    WriteAmountOfTurnsPerIteHistogram(averageTurnPerIteData);
    WriteWinrateRefPerIteration(winrateRefData, 1000);
    std::cout << "Final player 1 reference winrate : " << player1->referenceWinrate << std::endl;
}

void ResetGameWithoutDeckChange() 
{
    player1->ResetPlayer();
    player2->ResetPlayer();
    player1Board.clear();
    player2Board.clear();
}

void ResetGameGlobal() 
{
    player1->ResetPlayerGlobal();
    player2->ResetPlayerGlobal();
    player1Board.clear();
    player2Board.clear();
}

int BoardAttackSum(std::vector<Card> playerBoard) {
    int res = 0;
    if (playerBoard.size() != 0)
    {
        for (int i = 0; i < playerBoard.size(); i++) {
            if (playerBoard[i].CanAttack()) {
                res += playerBoard[i].GetCardATK();
            }
        }
    }  
    return res;
}

int GetFirstProvocCardIndex(std::vector<Card> playerBoard) {
    for (int i = 0; i < playerBoard.size(); i++) {
        if (playerBoard[i].GetCardProvoc() == true) return i;
    }
    return -1;
}

bool CheckForProvoc(std::vector<Card> playerBoard) {
    for (int i = 0; i < playerBoard.size(); i++) {
        if (playerBoard[i].GetCardProvoc() == true) return true;
    }
    return false;
}

//EVERYTHING DATA RELATED//
void WriteAmountOfTurnsPerGameHistogram(std::vector<int>& nbTurnData) {
    csvfile csv("AmountOfGamesPerAmountOfTurnData.csv");
    csv << "Amount of turns" <<  "Amount of games" << endrow;

    int max_value = *max_element(nbTurnData.begin(), nbTurnData.end());
    std::vector<int> amountOfGames = std::vector<int>();
    for (int i = max_value; i >= 0; --i) 
    {
        amountOfGames.push_back(std::count(nbTurnData.begin(), nbTurnData.end(), i));
    }
    std::reverse(amountOfGames.begin(), amountOfGames.end());
    for (int i = max_value; i >= 0; --i)
    {
        csv << i + 1 << amountOfGames[i]<< endrow;
    }
}

void WriteAmountOfTurnsPerIteHistogram(std::vector<int>& averageTurnData) {
    csvfile csv("AmountOfItePerAverageOfTurnData.csv");
    csv << "Average amount of turns" << "Amount of iteration" << endrow;
    int max_value = *max_element(averageTurnData.begin(), averageTurnData.end());
    std::vector<int> amountOfIte = std::vector<int>();
    for (int i = max_value; i >= 0; --i)
    {
        amountOfIte.push_back(std::count(averageTurnData.begin(), averageTurnData.end(), i));
    }
    std::reverse(amountOfIte.begin(), amountOfIte.end());
    for (int i = max_value; i >= 0; --i)
    {
        csv << i + 1 << amountOfIte[i] << endrow;
    }
}

void WriteWinratePerAmountOfGames(std::vector<float>& winRateData, int nbGames) {
    csvfile csv("WinratePerAmountOfGamesData.csv");
    csv <<  "Amount of games" << "WinRate" << endrow;
    for (int i = 0; i < nbGames; i++)
    {
        csv << i + 1 << winRateData[i] << endrow;
    }
}

void WriteWinrateRefPerIteration(std::vector<float>& winRateData, int nbIte) {
    csvfile csv("WinratePerAmountOfIteData.csv");
    csv << "Iterations" << "WinRate" << endrow;
    for (int i = 0; i < nbIte; i++)
    {
        csv << i + 1 << winRateData[i] << endrow;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
