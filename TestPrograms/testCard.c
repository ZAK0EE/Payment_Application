#include <stdio.h>

#include "Card/card.h"

int main()
{
	EN_cardError_t result;
	ST_cardData_t cardData;

	while (1)
	{
		result = getCardPAN(&cardData);
	
		switch (result)
		{
		case CARD_OK:
			printf("CARD_OK");
			break;
		case WRONG_NAME:
			printf("WRONG_NAME");
			break;
		case WRONG_EXP_DATE:
			printf("WRONG_EXP_DATE");
			break;
		case WRONG_PAN:
			printf("WRONG_PAN");
			break;
		}
		printf("\n");

		// flush input
		char c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

}
