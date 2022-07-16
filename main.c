#include <stdio.h>
#include "Card/card.h"
int main()
{
	ST_cardData_t x;
	getCardHolderName(&x);
	printf("Hello, world %s\n", x.cardHolderName);
}