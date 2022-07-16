#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "card.h"

#define MAX_NAME_LEN 24
#define MIN_NAME_LEN 20 
#define MAX_BUFFER 101

#define EXPDATELEN 5

static char* extractName(void)
{
	printf("Please enter your name: ");

	char* cardName = calloc(MAX_BUFFER, sizeof(char));
	if (cardName == NULL)
		return NULL;

	if (scanf_s("%[^\n]s", cardName, (unsigned)MAX_BUFFER) != 1)
	{
		free(cardName);
		cardName = NULL;
	}

	return cardName;
	
}
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char* cardName = extractName();
	size_t cardNameLen = strlen(cardName);
	
	if (cardName == NULL || cardNameLen > MAX_NAME_LEN || cardNameLen < MIN_NAME_LEN)
		return WRONG_NAME;

	strcpy_s(cardData->cardHolderName, cardNameLen + 1, cardName);

	free(cardName);

	return OK;
}



static char* extractExpDate(void)
{
	printf("Please enter expiration date (MM/YY): ");

	char* cardExpDate = calloc(MAX_BUFFER, sizeof(char));
	if (cardExpDate == NULL)
		return NULL;

	if (scanf_s("%s", cardExpDate, (unsigned)MAX_BUFFER) != 1)
	{
		free(cardExpDate);
		cardExpDate = NULL;
	}

	return cardExpDate;

}
static uint8_t validateExpDate(char* expDate)
{
	if (expDate == NULL ||!isdigit(expDate[0]) || !isdigit(expDate[1]) || expDate[2] != '/'
		|| !isdigit(expDate[3]) || !isdigit(expDate[4]))
		return 0;

	return 1;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char* cardExpDate = extractExpDate();
	size_t cardExpDateLen = strlen(cardExpDate);

	if (cardExpDate == NULL || cardExpDateLen != EXPDATELEN || !validateExpDate(cardExpDate))
		return WRONG_EXP_DATE;

	strcpy_s(cardData->cardExpirationDate, cardExpDateLen + 1, cardExpDate);

	free(cardExpDate);

	return OK;

}



EN_cardError_t getCardPAN(ST_cardData_t* cardData);