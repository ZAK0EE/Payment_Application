#include <stdio.h>
#include <string.h>
#include <time.h>

//#include "../Card/card.h"
#include "terminal.h"



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
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);