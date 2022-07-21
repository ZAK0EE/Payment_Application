#include <stdio.h>

#include "Card/card.h"
#include "Terminal/terminal.h"
int main()
{
	EN_terminalError_t result;
	ST_cardData_t cardData;
	ST_terminalData_t termData;

	while (1)
	{

		getCardExpiryDate(&cardData);
		getTransactionDate(&termData);

		result = isCardExpired(cardData, termData);

		//printf("%s\n", termData.transactionDate);

		switch (result)
		{
		case TERMINAL_OK:
			printf("TERMINAL_OK");
			break;
		case WRONG_DATE:
			printf("WRONG_DATE");
			break;
		case EXPIRED_CARD:
			printf("EXPIRED_CARD");
			break;
		case INVALID_CARD:
			printf("INVALID_CARD");
			break;
		case INVALID_AMOUNT:
			printf("INVALID_AMOUNT");
			break;
		case EXCEED_MAX_AMOUNT:
			printf("EXCEED_MAX_AMOUNT");
			break;
		case INVALID_MAX_AMOUNT:
			printf("INVALID_MAX_AMOUNT");
			break;
		}
		printf("\n");

		// flush input
		char c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

}
