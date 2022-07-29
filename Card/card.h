#ifndef _CARD_
#define _CARD_

#include <inttypes.h>

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

/** @brief This function will ask for the cardholder's name and store it into card data.
 *  @param cardData The user's card data.
 *  @return If the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error, else return OK.
 *	@note Card holder name is 24 characters string max and 20 min.
 */
EN_cardError_t getCardHolderName(ST_cardData_t* cardData);


/** @brief This function will ask for the card expiry date and store it in card data.
 *  @param cardData The user's card data.
 *  @return If the card expiry date is NULL, less or more than 5 characters,
 *			 or has the wrong format will return WRONG_EXP_DATE error, else return OK.
 *	@note Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);


/** @brief This function will ask for the card's Primary Account Number and store it in card data.
 *  @param cardData The user's card data.
 *  @return If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.
 *	@note PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
 */
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

#endif // _CARD_