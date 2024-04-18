// CardGameOptimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "CardManager.h"
#include "Player.h"
#include <time.h>
#include "csvfile.h"

Player* player1 = new Player();
Player* player2 = new Player();

std::vector<Card> player1Board = std::vector<Card>();
std::vector<Card> player2Board = std::vector<Card>();

int BoardAttackSum(std::vector<Card> playerBoard);
void ResetGameWithoutDeckChange();
void WriteAmountOfTurnsPerGameHistogram(std::vector<int>& nbTurnData);
void WriteWinratePerAmountOfGames(std::vector<float>& winRateData, int nbGames);

int main()
{
    srand(time(NULL));
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
                player1->IncrementMana(1);
                player1->IncrementHand();
                player1Board.push_back(player1->PlayHigherCostCard());
                int player1InflictedDamage = BoardAttackSum(player1Board);
                player2->DecreasePV(player1InflictedDamage);
                if (player2->GetPV() <= 0) continue;

                //PLAYER 2 TURN
                player2->IncrementMana(1);
                player2->IncrementHand();
                player2Board.push_back(player2->PlayHigherCostCard());
                int player2InflictedDamage = BoardAttackSum(player2Board);
                player1->DecreasePV(player2InflictedDamage);
            }
            else if (i >= 500) 
            {
                //PLAYER 2 TURN
                player2->IncrementMana(1);
                player2->IncrementHand();
                player2Board.push_back(player2->PlayHigherCostCard());
                int player2InflictedDamage = BoardAttackSum(player2Board);
                player1->DecreasePV(player2InflictedDamage);
                if (player1->GetPV() <= 0) continue;

                //PLAYER 1 TURN
                player1->IncrementMana(1);
                player1->IncrementHand();
                player1Board.push_back(player1->PlayHigherCostCard());
                int player1InflictedDamage = BoardAttackSum(player1Board);
                player2->DecreasePV(player1InflictedDamage);       
            }
            nbTurns++;
        }
        if (player1->GetPV() > 0 && player2->GetPV() <= 0)
        {
            std::cout << "Game " << i+1 << " : Player 1 won !\n" << std::endl;
            player1Winrate++;
        }
        winratePerAmountOfGames.push_back((float)((player1Winrate / (i + 1.0f)) * 100.0f));
        nbTurnPerGame.push_back(nbTurns);
        ResetGameWithoutDeckChange();
    }
    std::cout << "All games were played ! Computing player 1 winrate...\n" << std::endl;
    player1Winrate = (float)((player1Winrate / 1000.0f) * 100.0f);
    std::cout << "Player 1 winrate is : " << player1Winrate << std::endl;
    std::cout << "Amount of turn per game :\n" << std::endl; 
    for (int j = 0; j < nbTurnPerGame.size(); j++)
    {
        std::cout << " Game " << j+1 << " nb turns : " << nbTurnPerGame[j] << std::endl;
    }
    WriteAmountOfTurnsPerGameHistogram(nbTurnPerGame);
    WriteWinratePerAmountOfGames(winratePerAmountOfGames, 1000);
}

void ResetGameWithoutDeckChange() 
{
    player1->ResetPlayer();
    player2->ResetPlayer();
    player1Board.clear();
    player2Board.clear();
}

int BoardAttackSum(std::vector<Card> playerBoard) {
    int res = 0;
    for (int i = 0; i < playerBoard.size(); i++) {
        res += playerBoard[i].GetCardATK();
    }
    return res;
}

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

void WriteWinratePerAmountOfGames(std::vector<float>& winRateData, int nbGames) {
    csvfile csv("WinratePerAmountOfGamesData.csv");
    csv <<  "Amount of games" << "WinRate" << endrow;
    for (int i = 0; i < nbGames; i++)
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
