# TP - Optimization
## Summary

A deck optimization program made with C++.

## Development
### 1 - Implementing the basic rules
**-- Rules --**\
Two players are pitted against each other in a Magic: The Gathering type of card game.\
At this level of implementation, a card has several characteristics: \
- a cost 8 ≥ C ≥ 1\
- an attack value ATK ≥ 0\
- a defense value DEF ≥ 1\
The cost is calculated as follows: C = (A + D) / 2.

We create a set list of all possible combinations of features. Then, we draw randomly from the set list to form a deck for each player. If the same card is drawn more than twice, the third duplicate is discarded and another card is drawn. A deck is made up of 30 cards.

Each player has one resource: mana. Each player's mana rises incrementally at the beginning of each turn: the turn number corresponds to the amount of mana available to the player.
Each player has a hand formed from his deck. He starts with 4 cards in hand, then gains one more card each round.

**-- Sequence --**\
A round of play proceeds as follows:\
For each player:\
1- mana+1.\
2- draw a card from the deck, hand+1.\
3- player plays his highest-cost card according to available mana.
If he has more than one, he plays one of them at random. He places it on his board.\
4- All cards with ATK > 0 attack the enemy player's VPs.\

Each player plays the same number of turns: if player 1 kills player 2, player 2 takes his turn before the end of the game. 
A player is considered the winner if he is the only one with VP at the end of a turn. Ties are counted as defeats.\
We run 1000 games and calculate the winrate of player 1 vs. player 2.

### 2 - Implementing the Monte-Carlo optimization algorithm
**-- Sequence --**\
At this level of implementation, we're going to optimize player 1's deck by Monte Carlo optimization algorithm:\
We will run 1000 iterations of 1000 games. Player 1 now has a reference deck and a reference winrate, which are both initialized to player 1's deck and winrate respectively for iteration 1.\
- At the start of each iteration, a card in the reference deck of player 1 is swapped with another card from the set list, creating a different deck for the iteration.\
- At the end of each iteration, the winrate for the iteration is compared with the reference winrate. If the iteration winrate is higher than the reference winrate, it becomes the reference winrate and the deck used in that iteration becomes the reference deck. Otherwise, the current reference deck and winrate are retained.\
This makes it possible to study the impact of the deck's content on the player's winrate, while enabling him to optimize his deck as it is iterated.

### 3 - Implementing the special characteristics
**-- Taunt --**\
If an enemy creature has the taunt characteristic (TAUNT):\
Before attacking the enemy player, the first creature on the enemy board with TAUNT must be defeated. The attacking player's first card attacks this creature creature instead of the enemy player and loses DEF equal to the creature's ATK.
If the enemy creature with TAUNT not succumb to this first attack, the attacking player's next card attacks it in turn, and so on until it is defeated.

Any taunted attacking card whose DEF is less than or equal to the ATK of the ATK of the enemy creature with TAUNT is destroyed this turn. If it is not destroyed, it does not attack the player who used its turn on the enemy creature with TAUNT.
On the next turn, all creatures of both players recover their full DEF.\
With TAUNT a Boolean value of 0 if inactive, 1 if active, the cost formula becomes:\
C = (ATK + DEF) / 2 + 1.5 * TAUNT.

**-- Trample --**\
If an attacking creature has the trample characteristic (TRAMPLE):\
Two creatures clash if the defending creature has the TAUNT characteristic.
When two creatures clash, if the attacking creature has TRAMPLE and if its ATK is greater than the opposing creature's DEF, the rest of the calculation Trample.ATK - Opponent.DEF is added to the damage inflicted on the enemy this turn.\
With TRAMPLE a Boolean of value 0 if inactive, 1 if active, the cost formula
becomes:\
C = (ATK + DEF) / 2 + 1.5 * TAUNT + 1 * TRAMPLE.

**-- Distortion --**\
If an attacking creature has the DISTORTION characteristic:\
Two creatures clash if the defending creature has the TAUNT characteristic.
When two creatures clash, if the attacking creature has DISTORTION, it ignores its opponent's TAUNT and directly attacks the enemy player.
However, if the defending creature also possesses DISTORTION, the attacking creature still targets the opposing creature.
With DISTORTION a Boolean value of 0 if inactive, 1 if active, the cost formula becomes:\
C = (ATK + DEF)/2 + 1.5 * TAUNT + 1 * TRAMPLE + 1 * DISTORTION.

**-- First Strike --**\
If a creature has the first strike (FS) characteristic:\
Two creatures clash if the defending creature has TAUNT. When two creatures clash, if one of the creatures has FS, it attacks the opposing creature first. If it kills it, it suffers no damage. If both creatures have FS, they attack each other at the same time.\
With FS a Boolean value of 0 if inactive, 1 if active, the cost formula becomes:\
C = (ATK + DEF)/2 + 1.5 * TAUNT + 1 * TRAMPLE + 1 * DISTORTION + 1 * FS

## Features  

- [x] CARDS
- - [x] Base cards
- - [x] Base rules 

- [x] OPTIMIZATION
- - [x] Monte-Carlo
      
- [x] SPECIAL CHARACTERISTIC
- -  [x] Taunt
- -  [x] Trample
- -  [x] Distortion
- -  [x] First Strike
