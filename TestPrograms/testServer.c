#include <stdio.h>

#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"

void printTransaction(ST_transaction_t* transData)
{
	ST_cardData_t* card = &transData->cardHolderData;
	ST_terminalData_t *terminal = &transData->terminalData;

	printf("Transaction Data: %d, %d, ", transData->transactionSequenceNumber, transData->transState);
	printf("Card Data: %s, %s, %s, ", card->primaryAccountNumber, card->cardHolderName, card->cardExpirationDate);
	printf("Terminal Data: %f, %s, %f\n", terminal->maxTransAmount, terminal->transactionDate, terminal->transAmount);

	return;
}

int main()
{
	EN_serverError_t result;
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;
	while (1)
	{
		getCardHolderName(&transData.cardHolderData);
		getCardExpiryDate(&transData.cardHolderData);
		getCardPAN(&transData.cardHolderData);

		getTransactionAmount(&transData.terminalData);
		setMaxAmount(&transData.terminalData);
		getTransactionDate(&transData.terminalData);
	
		result = saveTransaction(&transData);

		switch (result)
		{
		case SERVER_OK:
			printf("SERVER_OK");
			break;
		case SAVING_FAILED:
			printf("SAVING_FAILED");
			break;
		case TRANSACTION_NOT_FOUND:
			printf("TRANSACTION_NOT_FOUND");
			break;
		case ACCOUNT_NOT_FOUND:
			printf("ACCOUNT_NOT_FOUND");
			break;
		case LOW_BALANCE:
			printf("LOW_BALANCE");
			break;
		}
		printf("\n");

		// flush input
		char c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

}
