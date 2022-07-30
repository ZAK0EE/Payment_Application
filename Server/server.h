#ifndef _SERVER_
#define _SERVER_

#include <inttypes.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;



typedef enum EN_serverError_t
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;


/** @brief This function will take all transaction data and validate its data.
 *			It checks the account details and amount availability.
 *  @param transData The user's transaction data.
 *  @return If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, 
 *			if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, else returns APPROVED.
 *	@note It will update the database with the new balance only when APPROVED, 
			and will save transactions in all cases except INTERNAL_SERVER_ERROR.
 */
EN_transState_t recieveTransactionData(ST_transaction_t* transData);


/** @brief This function will take card data and validate these data.
 *			It checks if the PAN exists or not in the server's database.
 *  @param cardData The user's card data.
 *  @return If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK.
 */
EN_serverError_t isValidAccount(ST_cardData_t* cardData);


/** @brief This function will take terminal data and validate these data.
 *			It checks if the transaction's amount is available or not.
 *  @param transData The user's transaction data.
 *  @return If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK
 *			You should deliver a maximum 2min video to discuss your implementation and run different test cases on this function.
 */
EN_serverError_t isAmountAvailable(ST_transaction_t* transData);


/** @brief This function will take all transaction data into the transactions database.
 * 			It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
 *  @param transData The user's transaction data.
 *  @return If transaction can't be saved will return SAVING_FAILED, else will return OK.
 *	@note If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
 */
EN_serverError_t saveTransaction(ST_transaction_t* transData);


/** @brief This function will take all transaction data into the transactions database.
 * 			It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
 *  @param transData The user's transaction data.
 *  @param transactionSequenceNumber the number of the current transaction.
 *  @return If transaction can't be saved will return SAVING_FAILED, else will return OK
 *	@note If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
 */
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif // _SERVER_