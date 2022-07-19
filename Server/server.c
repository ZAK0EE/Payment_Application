

ST_accountsDB_t accounts[255];
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


EN_serverError_t isAmountAvailable(ST_transaction_t* transData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);