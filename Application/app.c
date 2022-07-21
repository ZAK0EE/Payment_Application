#include <stdio.h>

#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

void appStart(void)
{ 
	// *********************************************Card side*********************************************
	printf("[DEBUG]Card Module\n\n");

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
	printf("\n[DEBUG]Terminal Module\n\n");

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
		printf("[ERROR]EXCEED_MAX_AMOUNT\a\n");
		return;

	}

	// *********************************************Server side*********************************************
	printf("\n[DEBUG]Server Module\n\n");

	ST_transaction_t transData;
	transData.cardHolderData = cardData;
	transData.terminalData = termData;

	switch (recieveTransactionData(&transData))
	{
	case DECLINED_STOLEN_CARD:
		printf("[ERROR]DECLINED_STOLEN_CARD\a\n");
		break;
	case DECLINED_INSUFFECIENT_FUND:
		printf("[ERROR]DECLINED_INSUFFECIENT_FUND\a\n");
		break;
	case INTERNAL_SERVER_ERROR:
		printf("[ERROR]INTERNAL_SERVER_ERROR\a\n");
		break;
	default:
		printf("[INFO]Transaction is done successfully.\n");
		break;
	}


}
