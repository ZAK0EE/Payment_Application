#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Card/card.h"

#define MAX_NAME_LEN 24
#define MIN_NAME_LEN 20 
#define MAX_BUFFER 101

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

	return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);