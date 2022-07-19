#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"


ST_transaction_t transactions[255];


EN_transState_t recieveTransactionData(ST_transaction_t* transData);/*
{
	if (isValidAccount(&transData->cardHolderData) == DECLINED_STOLEN_CARD)
		return DECLINED_STOLEN_CARD;

	if (isAmountAvailable(transData) == LOW_BALANCE)
		return DECLINED_INSUFFECIENT_FUND;

	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;

	//UpdateBalance();

	return APPROVED;

}*/

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "r");
	if (file == 0)
		return INTERNAL_SERVER_ERROR;

	char buffer[30] = { 0 };

	// Checking for PAN in DB
	while (fscanf_s(file, "Card Data: %[^,] ,  %*f\n", buffer, (unsigned int)_countof(buffer)) != EOF)
	{
		if (strcmp(cardData->primaryAccountNumber, buffer) == 0)
			return SERVER_OK;
	}

	fclose(file);
	return DECLINED_STOLEN_CARD;

}

EN_serverError_t isAmountAvailable(ST_transaction_t* transData)
{
	ST_cardData_t* cardData = &transData->cardHolderData;

	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "r");
	if (file == 0)
		return INTERNAL_SERVER_ERROR;

	char buffer[30] = { 0 };
	float balance = 0;

	// Checking for PAN in DB
	while (fscanf_s(file, "Card Data: %[^,] ,  %f\n", buffer, (unsigned int)_countof(buffer), &balance) != EOF)
	{
		if (strcmp(cardData->primaryAccountNumber, buffer) == 0)
		{
			if (transData->terminalData.transAmount > balance)
				return LOW_BALANCE;
			else
				return SERVER_OK;
		}
	}

	fclose(file);

	return ACCOUNT_NOT_FOUND;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	ST_cardData_t* cardData = &transData->cardHolderData;

	FILE* file = NULL;
	fopen_s(&file, "./DB/Transactions DB.txt", "a+");
	if (file == 0)
		return SAVING_FAILED;

	int transNumber = 0;
	
	while (fscanf_s(file, "%*[^\n]\n") != EOF)
	{
		transNumber++;
	}

	transData->transactionSequenceNumber = transNumber;

	ST_cardData_t *card = &transData->cardHolderData;
	ST_terminalData_t* terminal = &transData->terminalData;

	fprintf(file, "Transaction Data: %d, %d, ", transData->transactionSequenceNumber, transData->transState);
	fprintf(file, "Card Data: %s, %s, %s, ", card->primaryAccountNumber, card->cardHolderName, card->cardExpirationDate);
	fprintf(file, "Terminal Data: %f, %s, %f\n", terminal->maxTransAmount, terminal->transactionDate, terminal->transAmount);
	

	printf("%d", transNumber);
	fclose(file);

}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);