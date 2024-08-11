# TP - Optimization
## Summary

A deck optimization program made with C++.

## Development
**1 - Implementing the basic rules**
-- Rules --\
Two players are pitted against each other in a Magic: The Gathering.\
At this level of implementation, a card has several characteristics: \
a cost 8 ≥ C ≥ 1\
an attack value ATK ≥ 0\
a defense value DEF ≥ 1\
The cost is calculated as follows: C = (A + D) / 2. \
We create a set list of all possible combinations of features.
combinations. Then, we draw randomly from the set list to form the deck for
each player's deck. If the same card is drawn more than twice, the third duplicate is discarded and another card is drawn. A deck is made up of 30
cards.
Each player has one resource: mana. Each player's mana
player's mana rises incrementally at the beginning of each turn: the turn number
corresponds to the amount of mana available to the player.
Each player has a hand formed from his deck. He starts with 4
cards in hand, then gains one more card each round.

-- Sequence of play --\
A round of play proceeds as follows:\
For each player:\
1- mana+1.\
2- draw a card from the deck, hand+1.\
3- player plays his highest-cost card according to available mana.
If he has more than one, he plays one of them at random. He places it on his board.\
4- All cards with ATK > 0 attack the enemy player's VPs.\
Each player plays the same number of turns: if player 1 kills player 2, player
player 2 takes his turn before the end of the game. 
A player is considered the winner if he is the only one with VP at the end of a turn.
turn. Ties are counted as defeats.
We run 1000 games and calculate the winrate of player 1 vs. player
2.

## Features  

- [x] CARDS
- - [x] Base cards
- - [x] Base rules 

- [x] OPTIMIZATION
- - [x] Monte-Carlo
      
- [x] SPECIAL CHARACTERISTIC
- -  [x] Taunt
- -  [x] Trample
- -  [x] Warp
- -  [x] First Strike
