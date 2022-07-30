#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

// Database vairables (database is stored in memory during the program)
ST_accountsDB_t AccountDB[1000] = {0};
int AccountNum = 0;

ST_transaction_t TransactionDB[1000] = { 0 };
int TransactionNum = 0;


/** @brief This function will load the accounts data from the Accounts DB
 *  @return If accounts data can't be loaded will return SAVING_FAILED, else will return OK.
 */
EN_serverError_t loadAccountDB()
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "r");
	if (file == 0)
	{
		return SAVING_FAILED;
	}

	int result;
	while ((result = fscanf_s(file, "Card Data : %19s , %f\n", AccountDB[AccountNum].primaryAccountNumber, (unsigned int)_countof(AccountDB[AccountNum].primaryAccountNumber),  &AccountDB[AccountNum].balance)) != EOF)
	{
		AccountNum++;
	}

	fclose(file);

	return SERVER_OK;
}


/** @brief This function will save the accounts data to the Accounts DB
 *  @return If accounts data can't be saved will return SAVING_FAILED, else will return OK.
 */
EN_serverError_t saveAccountDB()
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Accounts DB.txt", "w");
	if (file == 0)
	{
		return SAVING_FAILED;
	}

	int accountid = 0;
	while (accountid < AccountNum)
	{
		fprintf(file, "Card Data: %s, %f\n", AccountDB[accountid].primaryAccountNumber, AccountDB[accountid].balance);
		accountid++;
	}

	fclose(file);

	return SERVER_OK;
}


/** @brief This function will load the transactions data from the transactions DB
 *  @return If transaction can't be loaded will return SAVING_FAILED, else will return OK.
 */
EN_serverError_t loadTransactionDB()
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Transactions DB.txt", "r");
	if (file == 0)
	{
		return SAVING_FAILED;
	}

	int result = 0;

	while (result != EOF)
	{
		ST_cardData_t* card = &TransactionDB[TransactionNum].cardHolderData;
		ST_terminalData_t* terminal = &TransactionDB[TransactionNum].terminalData;

		fscanf_s(file, "Transaction Data: %d, %d, ", &TransactionDB[TransactionNum].transactionSequenceNumber, &TransactionDB[TransactionNum].transState);
		fscanf_s(file, "Card Data: %[^,] ,  %[^,] ,  %[^,] , ",	card->primaryAccountNumber, (unsigned int)_countof(card->primaryAccountNumber),
																				card->cardHolderName, (unsigned int)_countof(card->cardHolderName),
																				card->cardExpirationDate, (unsigned int)_countof(card->cardExpirationDate));

		result = fscanf_s(file, "Terminal Data: %f,  %[^,] , %f\n",	&terminal->maxTransAmount,
																					terminal->transactionDate, (unsigned int)_countof(terminal->transactionDate),
																					&terminal->transAmount);
		if(result != EOF)
			TransactionNum++;

	}
	
	fclose(file);

	return SERVER_OK;
}


/** @brief This function will save the transactions data to the transactions DB
 *  @return If transaction can't be saved will return SAVING_FAILED, else will return OK.
 */
EN_serverError_t saveTransactionDB()
{
	FILE* file = NULL;
	fopen_s(&file, "./DB/Transactions DB.txt", "w");
	if (file == 0)
	{
		return SAVING_FAILED;
	}

	int transactionid = 0;
	while (transactionid < TransactionNum)
	{
		ST_cardData_t* card = &(TransactionDB[transactionid].cardHolderData);
		ST_terminalData_t* terminal = &(TransactionDB[transactionid].terminalData);

		fprintf(file, "Transaction Data: %d, %d, ", TransactionDB[transactionid].transactionSequenceNumber, TransactionDB[transactionid].transState);
		fprintf(file, "Card Data: %s, %s, %s, ", card->primaryAccountNumber, card->cardHolderName, card->cardExpirationDate);
		fprintf(file, "Terminal Data: %f, %s, %f\n", terminal->maxTransAmount, terminal->transactionDate, terminal->transAmount);
		transactionid++;
	}

	fclose(file);

	return SERVER_OK;
}



EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t state;

	loadAccountDB();
	loadTransactionDB();

	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
		state = DECLINED_STOLEN_CARD;
	else if (isAmountAvailable(transData) == LOW_BALANCE)
		state = DECLINED_INSUFFECIENT_FUND;
	else
		state = APPROVED;

	transData->transState = state;
	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;

	return state;

}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{

	for (int i = 0; i < AccountNum; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, AccountDB[i].primaryAccountNumber) == 0)
		{
			return SERVER_OK;
		}
	}

	return ACCOUNT_NOT_FOUND;

}

EN_serverError_t isAmountAvailable(ST_transaction_t* transData)
{
	ST_cardData_t* cardData = &transData->cardHolderData;

	// Check for the account balance in the AccountDB
	for (int i = 0; i < AccountNum; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, AccountDB[i].primaryAccountNumber) == 0)
		{

			if (transData->terminalData.transAmount > AccountDB[i].balance)
			{
				return LOW_BALANCE;
			}
			else
			{
				return SERVER_OK;
			}
		}
	}

	return ACCOUNT_NOT_FOUND;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	
	if (saveTransactionDB() == SAVING_FAILED)
		return SAVING_FAILED;

	if (saveAccountDB() == SAVING_FAILED)
		return SAVING_FAILED;

	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	// Retrieve the transaction from TransactionDB
	for (int i = 0; i < TransactionNum; i++)
	{
		if (transactionSequenceNumber == TransactionDB[i].transactionSequenceNumber)
		{
			*transData = TransactionDB[i];
			return SERVER_OK;
		}
	}

	return TRANSACTION_NOT_FOUND;

}