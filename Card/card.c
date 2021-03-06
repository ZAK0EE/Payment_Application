#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "card.h"

#define MAX_NAME_LEN 24
#define MIN_NAME_LEN 20 
#define MAX_BUFFER 101

#define EXPDATELEN 5

#define MAX_PAN_LEN 19
#define MIN_PAN_LEN 16

static char* extractName(void)
{
	printf("Please enter name: ");

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

static uint8_t validateName(char* cardName)
{
	size_t cardNameLen = strlen(cardName);
	uint8_t valid = 1;

	for (int i = 0; i < cardNameLen; i++)
	{
		if (!isalpha(cardName[i]) && cardName[i] != ' ')
		{
			valid = 0;
			break;
		}
	}

	return valid;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char* cardName = extractName();

	size_t cardNameLen;
	if(cardName != NULL)
		cardNameLen = strlen(cardName);
	
	if (cardName == NULL || cardNameLen > MAX_NAME_LEN || cardNameLen < MIN_NAME_LEN || !validateName(cardName))
	{
		free(cardName);
		return WRONG_NAME;
	}
		

	strcpy_s(cardData->cardHolderName, cardNameLen + 1, cardName);

	free(cardName);

	return CARD_OK;
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

	size_t cardExpDateLen;
	if(cardExpDate != NULL)
		cardExpDateLen = strlen(cardExpDate);

	if (cardExpDate == NULL || cardExpDateLen != EXPDATELEN || !validateExpDate(cardExpDate))
		return WRONG_EXP_DATE;

	strcpy_s(cardData->cardExpirationDate, cardExpDateLen + 1, cardExpDate);

	free(cardExpDate);

	return CARD_OK;
}



static char* extractPAN(void)
{
	printf("Please enter PAN: ");

	char* cardPAN = calloc(MAX_BUFFER, sizeof(char));
	if (cardPAN == NULL)
		return NULL;

	if (scanf_s("%s", cardPAN, (unsigned)MAX_BUFFER) != 1)
	{
		free(cardPAN);
		cardPAN = NULL;
	}

	return cardPAN;

}

static uint8_t validatePAN(char* cardPAN)
{
	size_t cardPANLen = strlen(cardPAN);
	uint8_t valid = 1;

	for (int i = 0; i < cardPANLen; i++)
	{
		if (!isalnum(cardPAN[i]))
		{
			valid = 0;
			break;
		}
	}

	return valid;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char* cardPAN = extractPAN();

	size_t cardPANLen;
	if(cardPAN != NULL)
		cardPANLen = strlen(cardPAN);

	if (cardPAN == NULL || cardPANLen > MAX_PAN_LEN || cardPANLen < MIN_PAN_LEN || !validatePAN(cardPAN))
		return WRONG_PAN;

	strcpy_s(cardData->primaryAccountNumber, cardPANLen + 1, cardPAN);

	free(cardPAN);

	return CARD_OK;
}