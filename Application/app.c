#include <stdio.h>

#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

void appStart(void)
{ 
	fflush(stdin); fflush(stdout);
	// *********************************************Card side*********************************************
	ST_cardData_t cardData;
	if (getCardHolderName(&cardData) == WRONG_NAME)
	{
		printf("[ERROR]WRONG_NAME\a\n");
		return;
	}
	
	if (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
	{
		printf("[ERROR]WRONG_EXP_DATE\a\n");
		return;
	}
	
	if (getCardPAN(&cardData) == WRONG_PAN)
	{
		printf("[ERROR]WRONG_PAN\a\n");
		return;
	}


	// *********************************************Terminal side*********************************************
	ST_terminalData_t termData;
	if (getTransactionDate(&termData) == WRONG_DATE)
	{
		printf("[ERROR]WRONG_DATE\a\n");
		return;
	}

	if (isCardExpired(cardData, termData) == EXPIRED_CARD)
	{
		printf("[ERROR]EXPIRED_CARD\a\n");
		return;
	}


	if(getTransactionAmount(&termData) == INVALID_AMOUNT)
	{
		printf("[ERROR]INVALID_AMOUNT\a\n");
		return;
	}

	if (setMaxAmount(&termData) == INVALID_MAX_AMOUNT)
	{
		printf("[ERROR]INVALID_MAX_AMOUNT\a\n");
		return;
	}

	if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
	{
		printf("[ERROR]EXPIRED_CARD\a\n");
		return;

	}

	// *********************************************Server *********************************************
	ST_transaction_t transData;
	transData.cardHolderData = cardData;
	transData.terminalData = termData;

	if (isValidAccount(&cardData) == ACCOUNT_NOT_FOUND)
	{
		printf("[ERROR]ACCOUNT_NOT_FOUND\a\n");
		return;
	}


	if (isAmountAvailable(&transData) == LOW_BALANCE)
	{
		printf("[ERROR]LOW_BALANCE\a\n");
		return;
	}


	if (recieveTransactionData(&transData) == INTERNAL_SERVER_ERROR)
	{
		printf("[ERROR]INTERNAL_SERVER_ERROR\a\n");
		return;
	}

}
