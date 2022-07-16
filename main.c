#include <stdio.h>
#include "card.h"
int main()
{
	ST_cardData_t x;
	if (getCardHolderName(&x) != WRONG_NAME)
		printf("%s", x.cardHolderName);
	else
		printf("WRONG NAME");

}