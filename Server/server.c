#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"


static EN_serverError_t updateBalance(ST_transaction_t* transData)
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "r+");
	if (file == 0)
	{
		return SAVING_FAILED;
	}


	char buffer[50] = {0};
	float balance = 0;
	while (fscanf_s(file, "Card Data: %[^,] , ", buffer, (unsigned int)_countof(buffer)) != EOF)
	{
		if (strcmp(transData->cardHolderData.primaryAccountNumber, buffer) == 0)
		{
			long pos = ftell(file);
			fscanf_s(file, "%f", &balance);

			fseek(file, pos, SEEK_SET);
			fprintf(file, "%f", balance - transData->terminalData.transAmount);

			break;
		}

		fscanf_s(file, "%*[^\n]\n");
	}

	fclose(file);

	return SERVER_OK;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t state;

	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
		state = DECLINED_STOLEN_CARD;
	else if (isAmountAvailable(transData) == LOW_BALANCE)
		state = DECLINED_INSUFFECIENT_FUND;
	else
		state = APPROVED;

	transData->transState = state;
	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;

	if (state == APPROVED)
		updateBalance(transData);

	return state;

}

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
		{
			fclose(file);
			return SERVER_OK;
		}
			
	}

	return ACCOUNT_NOT_FOUND;

}

EN_serverError_t isAmountAvailable(ST_transaction_t* transData)
{
	ST_cardData_t* cardData = &transData->cardHolderData;

	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "r");
	if (file == 0)
		return ACCOUNT_NOT_FOUND;

	char buffer[30] = { 0 };
	float balance = 0;

	// Checking for PAN in DB
	while (fscanf_s(file, "Card Data: %[^,] ,  %f\n", buffer, (unsigned int)_countof(buffer), &balance) != EOF)
	{
		if (strcmp(cardData->primaryAccountNumber, buffer) == 0)
		{
			if (transData->terminalData.transAmount > balance)
			{
				fclose(file);
				return LOW_BALANCE;			
			}
			else
			{
				fclose(file);
				return SERVER_OK;
			}
				
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
	

	fclose(file);

	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Transactions DB.txt", "r");
	if (file == 0)
		return TRANSACTION_NOT_FOUND;

	int transNumber = 0;
	int result = 0;
	while (transNumber++ < transactionSequenceNumber && (result = fscanf_s(file, "%*[^\n]\n")) != EOF);

	if (result == EOF)
	{
		fclose(file);
		return TRANSACTION_NOT_FOUND;
	}
		

	ST_cardData_t* card = &transData->cardHolderData;
	ST_terminalData_t* terminal = &transData->terminalData;

	fscanf_s(file, "Transaction Data: %d, %d, ", &transData->transactionSequenceNumber, &transData->transState);
	fscanf_s(file, "Card Data: %[^,] ,  %[^,] ,  %[^,] , ", card->primaryAccountNumber, (unsigned int)_countof(card->primaryAccountNumber),
															card->cardHolderName, (unsigned int)_countof(card->cardHolderName),
															card->cardExpirationDate, (unsigned int)_countof(card->cardExpirationDate));

	fscanf_s(file, "Terminal Data: %f,  %[^,] , %f\n", &terminal->maxTransAmount, 
													   terminal->transactionDate, (unsigned int)_countof(terminal->transactionDate),
													   &terminal->transAmount);

	fclose(file);

	return SERVER_OK;

}