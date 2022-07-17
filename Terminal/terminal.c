#include <stdio.h>
#include <string.h>
#include <time.h>


#include "terminal.h"

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t now = time(0);
	if(now == (time_t)(-1))
		return WRONG_DATE;

	struct tm timeInfo;
	if (localtime_s(&timeInfo, &now) != 0)
		return WRONG_DATE;

	char timeString[50] = {0};
	if (sprintf_s(timeString, (unsigned)11, "%02d/%02d/%04d", timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900) == -1)
		return WRONG_DATE;

	if (strcpy_s(termData->transactionDate, (unsigned)11, timeString) != 0)
		return WRONG_DATE;

	return OK;

}



EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	int cardMon, cardYr, transMon, transYr;
	
	
	if (sscanf_s(cardData.cardExpirationDate, "%d/%d", &cardMon, &cardYr) != 2)
		return EXPIRED_CARD;
	cardYr += 2000; //cardYr is stored as YY

	if(sscanf_s(termData.transactionDate, "%d/%d/%d", &transMon, &transMon, &transYr) != 3)
		return EXPIRED_CARD;

	if (cardYr < transYr || (cardYr == transYr && cardMon < transMon))
		return EXPIRED_CARD;
	return OK;
}



static float getAmount()
{
	float amount = 0;
	printf("Please enter amount: ");
	if (scanf_s("%f", &amount) != 1)
	{
		return 0;
	}

	return amount;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount = getAmount();

	if (amount <= 0)
		return INVALID_AMOUNT;

	termData->transAmount = amount;

	return OK;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);