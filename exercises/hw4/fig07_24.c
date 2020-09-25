// Fig. 7.24: fig07_24.c
// Card shuffling and dealing.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define HAND_SIZE 5

typedef struct card {
	int row;
	int column;	
} Card;

// prototypes
void shuffle(unsigned int wDeck[][FACES]); // shuffling modifies wDeck
void deal(unsigned int wDeck[][FACES], const char *wFace[], const char *wSuit[]); // dealing doesn't modify the arrays
bool cardsEqual(Card card1, Card card2);
bool hasHighCard(Card hand[HAND_SIZE]);
bool hasTwoPairs(Card hand[HAND_SIZE]);
bool hasThreeOfAKind(Card hand[HAND_SIZE]);
bool hasStraight(Card hand[HAND_SIZE]);
int indexOfMinFace(Card hand[HAND_SIZE]);
bool handContainsFace(Card hand[HAND_SIZE], int face);
bool hasFlush(Card hand[HAND_SIZE]);
bool hasFourOfAKind(Card hand[HAND_SIZE]);
bool hasStraightFlush(Card hand[HAND_SIZE]);

int main(void)
{
   // initialize deck array
   unsigned int deck[SUITS][FACES] = { 0 };

   srand(time(NULL)); // seed random-number generator

   shuffle(deck); // shuffle the deck

   // initialize suit array                       
   const char *suit[SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};
   
   // initialize face array                   
   const char *face[FACES] =
      {"Ace", "Deuce", "Three", "Four",         
       "Five", "Six", "Seven", "Eight",         
       "Nine", "Ten", "Jack", "Queen", "King"};

   deal(deck, face, suit); // deal the deck
} 

// shuffle cards in deck
void shuffle(unsigned int wDeck[][FACES])
{
   // for each of the cards, choose slot of deck randomly
   for (size_t card = 1; card <= CARDS; ++card) {
      size_t row; // row number
      size_t column; // column number

      // choose new random location until unoccupied slot found
      do {                                                        
         row = rand() % SUITS;                                   
         column = rand() % FACES;                             
      } while(wDeck[row][column] != 0); // end do...while

      // place card number in chosen slot of deck
      wDeck[row][column] = card;
   } 
}

// deal cards in deck
void deal(unsigned int wDeck[][FACES], const char *wFace[], const char *wSuit[]) {
   // deal each of the cards
   size_t card;
   Card hand[HAND_SIZE];
   for (int i = 0; i < HAND_SIZE; i++) {
      card = rand() % (CARDS + 1);
      // loop through rows of wDeck
      for (size_t row = 0; row < SUITS; ++row) {
         // loop through columns of wDeck for current row
         for (size_t column = 0; column < FACES; ++column) {
            // if slot contains current card, display card
            if (wDeck[row][column] == card) {
               printf("%-5s of %-8s\n", wFace[column], wSuit[row]); // Print them vertically stacked
	       Card thisCard = {row, column};
	       hand[i] = thisCard; 
            } 
         } 
      } 
   } 
   printf("\nHand contains High Card: %c\n", hasHighCard(hand) ? 'T' : 'F');
   printf("Hand contains Two Pairs: %c\n", hasTwoPairs(hand) ? 'T' : 'F');
   printf("Hand contains Three of a Kind: %c\n", hasThreeOfAKind(hand) ? 'T' : 'F');
   printf("Hand contains a Straight: %c\n", hasStraight(hand) ? 'T' : 'F');
   printf("Hand contains a Flush: %c\n", hasFlush(hand) ? 'T' : 'F');
   printf("Hand contains Four of a Kind: %c\n", hasFourOfAKind(hand) ? 'T' : 'F');
   printf("Hand contains a Straight Flush: %c\n", hasStraightFlush(hand) ? 'T' : 'F'); 
}

bool cardsEqual(Card card1, Card card2) {
	if (card1.row == card2.row && card1.column == card2.column) {
		return true;
	}
	return false;
}

bool hasHighCard(Card hand[HAND_SIZE]) {
	// Every hand has a High Card in it.
	return true;
}

bool hasTwoPairs(Card hand[HAND_SIZE]) {
	int firstPairI = -1;
	for (int i = 0; i < HAND_SIZE - 1; i++) {
		for (int j = i + 1; j < HAND_SIZE; j++) {
			if (cardsEqual(hand[i], hand[j])) {
				if (firstPairI == -1) {
					firstPairI = i;
				}
				else if (!cardsEqual(hand[j], hand[firstPairI])) {
					return true;
				}
			}
		}
	}
	return false;
}

bool hasThreeOfAKind(Card hand[HAND_SIZE]) {
	int copiesFound;
	for (int i = 0; i < HAND_SIZE - 1; i++) {
		copiesFound = 0;
		for (int j = i + 1; j < HAND_SIZE; j++) {
			if (cardsEqual(hand[i], hand[j])) {
				if (copiesFound == 1) {
					return true;
				}
				else {
					copiesFound += 1;
				}
			}
		}
	}
	return false;
}

bool hasStraight(Card hand[HAND_SIZE]) {
	int minFaceValue = hand[indexOfMinFace(hand)].column;
	for (int i = 1; i <= 5; i++) {
		if (!handContainsFace(hand, minFaceValue + i)) {
			return false;
		}
	}
	return true;	
}

int indexOfMinFace(Card hand[HAND_SIZE]) {
	int minIndex = 0;
	for (int i = 0; i < HAND_SIZE; i++) {
		if (hand[i].column < hand[minIndex].column) {
			minIndex = i;
		}
	}
	return minIndex;
}

bool handContainsFace(Card hand[HAND_SIZE], int face) {
	for (int i = 0; i < HAND_SIZE; i++) {
		if (hand[i].column == face) {
			return true;
		}
	}
	return false;
}

bool hasFlush(Card hand[HAND_SIZE]) {
	for (int i = 0; i < HAND_SIZE; i++) {
		if (hand[i].row != hand[0].row) {
			return false;
		} 
	}
	return true;
}

bool hasFourOfAKind(Card hand[HAND_SIZE]) {
	int copiesFound;
	for (int i = 0; i < HAND_SIZE - 1; i++) {
		copiesFound = 0;
		for (int j = i + 1; j < HAND_SIZE; j++) {
			if (cardsEqual(hand[i], hand[j])) {
				if (copiesFound == 2) {
					return true;
				}
				else {
					copiesFound += 1;
				}
			}
		}
	}
	return false;
}

bool hasStraightFlush(Card hand[HAND_SIZE]) {
	return (hasFlush(hand)) && (hasStraight(hand));
}
