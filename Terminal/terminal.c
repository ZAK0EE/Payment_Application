#include <stdio.h>
#include <string.h>
#include <time.h>


#include "terminal.h"




EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	// Get current system's time
	time_t now = time(0);

	// Failed to aquire curernt time
	if(now == (time_t)(-1))
		return WRONG_DATE;

	// Convert time into human readable format
	struct tm timeInfo;
	if (localtime_s(&timeInfo, &now) != 0)
		return WRONG_DATE;

	char timeString[50] = {0};
	if (sprintf_s(timeString, (unsigned)11, "%02d/%02d/%04d", timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900) == -1)
		return WRONG_DATE;

	if (strcpy_s(termData->transactionDate, (unsigned)11, timeString) != 0)
		return WRONG_DATE;

	return TERMINAL_OK;

}



EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	int cardMon, cardYr, transMon, transYr;
	int result = 0;
	
	result = sscanf_s(cardData.cardExpirationDate, "%d/%d", &cardMon, &cardYr);
	if (result != 2)
		return EXPIRED_CARD;
	cardYr += 2000; // cardYr is stored as YY

	result = sscanf_s(termData.transactionDate, "%*d/%d/%d", &transMon, &transYr);
	if(result != 2)
		return EXPIRED_CARD;

	// Check if card date is older than transaction date
	if ((cardYr < transYr) || ((cardYr == transYr) && (cardMon < transMon)))
		return EXPIRED_CARD;
	return TERMINAL_OK;
}



static float getAmount()
{
	float amount = 0;
	int result = 0;
	printf("Please enter amount: ");

	result = scanf_s("%f", &amount);
	if (result != 1)
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

	return TERMINAL_OK;
}



EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}



static float getMaxAmount()
{
	float maxAmount = 0;
	int result = 0;
	printf("Please enter max amount: ");

	result = scanf_s("%f", &maxAmount);
	if (result != 1)
	{
		return 0;
	}

	return maxAmount;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float maxAmount = getMaxAmount();
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;

	termData->maxTransAmount = maxAmount;

	return TERMINAL_OK;
}