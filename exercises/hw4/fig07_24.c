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
bool hasHighCard(Card hand[HAND_SIZE]);
bool hasTwoPairs(Card hand[HAND_SIZE]);
bool hasThreeOfAKind(Card hand[HAND_SIZE]);
bool hasStraight(Card hand[HAND_SIZE]);
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
               printf("%5s of %-8s\n", wFace[column], wSuit[row]); // Print them vertically stacked
	       Card thisCard = {row, column};
	       hand[i] = thisCard; 
            } 
         } 
      } 
   } 
   printf("Hand contains High Card: %c", handHasHighCard(PLACEHOLDER) ? 'T' : 'F');
}

bool hasHighCard(Card hand[HAND_SIZE]) {
	// Every hand has a High Card in it.
	return true;
}

bool hasTwoPairs(Card hand[HAND_SIZE]) {
	for (int i = 0; i < HAND_SIZE; i++) {
		if (//TODO)
	}
}
