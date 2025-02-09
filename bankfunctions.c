// Color code definitions
#define BLUE "\e[0;34m"
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define CYAN "\e[0;36m"

#define BGREEN "\e[1;32m"

#define BHGREEN "\e[1;92m"

#define COLOR_RESET "\e[0m"

// Constant definitions for easy updates
#define PIN_LENGTH 6
#define MAX_ATTEMPTS 3
#define DISPLAY_DELAY 50

// (Transaction types)
#define DEPOSIT 1
#define WITHDRAW 2
#define TRANSFER 3
#define PAYBILLS 4

// Function Prototypes
void displayDivider();
void printMoneyArt();
void displayEndScreen();
void clearInputBuffer();
void loadingSequence(char *strText, int nMilliseconds);
void getNumInput(int *nInput);
void getDecimalInput(float *fInput);
void getPinInput(int *nPinInput);
int generateRefNum();
int verifyAccountLogin(int nAccountNumber, int nPinNumber);
void displayMainMenu();
void highlightMainMenu();
int getMainMenuOption();
int changePin(int nCurrentPin, int* nPinNumber);
void checkBalance(int nAccountNumber, float fBalance);
void depositMoney(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount);
void withdrawMoney(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount);
void transferFunds(float *fBalance, int nUserAccountNumber, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount);
void payBills(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount);
void logTransaction(int nType, float fAmount, float fBalance, int nReferenceNum, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount);
void displayTransactions(int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int nTransactionCount);
void printReceipt(int nTransactionType, float fTransactionAmount, int nAccountNumber, char *strBillerName, float *fBalance, int nReferenceNum);

// This function is for displaying dividers
void displayDivider()
{
	printf("==============================================================================================\n");
}

// This function is for displaying ASCII art of money on the screen
void printMoneyArt()
{
	printf(GREEN);
	printf("\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$                      $\n");
	printf("$     $$$$$$$$$$$$     $\n");
	printf("$     $          $     $\n");
	printf("$     $$$$$$$$$$$$     $\n");
	printf("$                      $\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("\n");
    printf(COLOR_RESET);
}

// This function is called everytime the program is exited or user logged out
void displayEndScreen()
{
	displayDivider();
	printf("\e[1mThank you for banking with ");
	printf(GREEN);
	printf("Animo Savings");
	printf(COLOR_RESET);
	printf("!\e[m\n");
	printMoneyArt();
	displayDivider();
}
// This function is for clearing the input buffer
void clearInputBuffer()
{
	int nReturn;
	// Loops until it reads \n or returns EOF (discarding any other char in the input buffer)
	while ((nReturn = getchar()) != '\n' && nReturn != EOF);
}

/* This function is for loading screens in between transactions
 Precondition: strText is a valid string
 			   nMilliseconds is a non-negative number
 @param *strText is a string that displays in the loading sequence
 @param nMilliseconds is the duration of the loading sequence in milliseconds
*/
void loadingSequence(char *strText, int nMilliseconds)
{
	printf("----------------------------\n");
	printf("%s", strText);
	for (int i = 0; i < 3; i++)
	{
		printf(".");
		Sleep(nMilliseconds / 3);
	}
	printf("\n");	
	system("cls");
}

/* This function makes sure that the user enters an integer
 Precondition: nInput is a valid pointer 
 @param *nInput is a pointer pointing to the pin input of the user
*/
void getNumInput(int *nInput)
{
	int nReturnValue = 0;
	char cExtraChar;
	int nErrorValue = -100;
	
	nReturnValue = scanf("%d", nInput); // Get user input
	
	if (nReturnValue == 1) // If the input is a number
	{
		// For decimal values
		cExtraChar = getchar(); // Check for extra characters
		if (cExtraChar != '\n') // If there's extra input (decimal values)
		{
			*nInput = nErrorValue; // Assign error value
			clearInputBuffer(); // Clear rest of input buffer
		}
	}
	else
	{
		*nInput = nErrorValue; // Assign error value
		clearInputBuffer(); // Clear input buffer if input is invalid
	}
}

void getDecimalInput(float *fInput)
{
	int nReturnValue = 0;
	char cExtraChar;
	float fErrorValue = -100;
	
	nReturnValue = scanf("%f", fInput); // Get user input
	
	if (nReturnValue == 1) // If the input is a number
	{
		// For decimal values
		cExtraChar = getchar(); // Check for extra characters
		if (!(cExtraChar == '.' || cExtraChar == '\n')) // If there's no decimal point or new line
		{
			*fInput = fErrorValue; // Assign error value
			clearInputBuffer(); // Clear rest of input buffer
		}
		
	}
	else
	{
		*fInput = fErrorValue; // Assign error value
		clearInputBuffer(); // Clear input buffer if input is invalid
	}

}

/* This function masks the user's pin input into asterisks
 Precondition: nPinInput is a valid pointer 
 @param *nPinInput is a pointer pointing to the pin input of the user
*/
void getPinInput(int *nPinInput)
{
    char cPin[PIN_LENGTH + 1]; // Pin takes 6 "inputs" (one extra space allocated for null character)
    int nCount = 0; // Input count
    int nIsEntered = 0; // Flag to check if the user pressed enter
    int isValid = 1; // Flag for validity check

    do
    {
        cPin[nCount] = getch(); // Get input for each pin character

        switch (cPin[nCount])
        {
            // Check for enter key
            case 13:
                nIsEntered = 1; // Set flag to 1 (entered)
                cPin[nCount] = '\0'; // Assign null character to current position
                break;
            // Check for backspace key
            case 8:
                if (nCount > 0) // If input is not at the beginning
                {
                    nCount -= 1; // Adjust for loop increment
                    printf("\b \b"); // Erase last asterisk
                }
                else
                {
                    nCount = -1; // Stay at the beginning (will be incremented to 0)
                }
                break;
            
            default:
                printf("*"); // Print asterisk in place of valid input
                nCount++;
                break;
        }

    } while (!nIsEntered); // Loop until enter is pressed

	for (int i = 0; i < nCount; i++)
    {
        if (!isdigit(cPin[i]))
        {
            isValid = 0; // Mark as invalid if non-digit character is detected
        }
    }
    
    // Validate if the entire input is valid
    if (isValid)
    {
        *nPinInput = atoi(cPin); // Convert entered "string" to integer and assign to nPinInput
    }
    else
    {
        *nPinInput = -1; // Assign an error value
    }

    printf("\n");
}
/* This function generates a reference number for each
	successful transaction
 @return nReferenceNumber
*/
int generateRefNum()
{
	int nReferenceNumber;
	
	// Seed rand with current time
	srand(time(NULL));
	
	// Set reference number as a random 8 digit number
	nReferenceNumber = (10000000 * (rand() % 10)) + rand() % 90000000 + (1100000 * (rand() % 10));
	
	return nReferenceNumber;
}
/* This function verifies user login credentials and 
 handles incorrect account number or pin inputs
 Precondition: Account Number is the user's ID number 
               Pin number is within 100000 - 999999 
 @param nAccountNumber is the Account Number of the user
 @param nPinNumber is the PIN of the user
 @return nLoginSuccesful
*/
int verifyAccountLogin(int nAccountNumber, int nPinNumber) 
{
    // Variables for Credentials input by the user
    int nAccountInput;
    int nPinInput;
    int nAttempts = 0;
    int nLoginSuccessful = 0; // 0 = not successful, 1 = successful, 2 = exit program

    // While user has not reached 3 attempts
    while (nAttempts < MAX_ATTEMPTS && !nLoginSuccessful) 
	{
		// Header
		printf("Welcome to ");
		printf(BHGREEN);
		printf("[Animo Savings]");
		printf(COLOR_RESET);
		printf("\n");
		
		Sleep(DISPLAY_DELAY);
		
		printf("Please Enter Your Login Credentials Below.\n");
		
		Sleep(DISPLAY_DELAY);
		
        // Ask for Login Credentials
        displayDivider();
        printf("+----------------+\n");
        printf("|    [0] Exit    |\n");
        printf("+----------------+\n");
        displayDivider();
        
        Sleep(DISPLAY_DELAY);
        
        printf("Enter your Account Number:\n> ");
       	getNumInput(&nAccountInput);
		
		// Only proceed if user didn't enter 0
		if (nAccountInput != 0)
		{
			// Get pin input
	        printf("Enter your 6-digit PIN:\n> ");
	        getPinInput(&nPinInput);
	        loadingSequence("Processing", 1000);
    	}
		
		// If the user entered 0
		if (nAccountInput == 0)
		{
			nLoginSuccessful = 2; // Set exited flag
		}
        // Check if Inputs match Credentials stored in the system
        else if (nAccountInput == nAccountNumber && nPinInput == nPinNumber) 
		{
            // Clear screen and print out welcome message if they match
            system("cls");
            printf(GREEN);
            printf("Welcome, %d!\n", nAccountNumber);
            printf(COLOR_RESET);
            nLoginSuccessful = 1; // Set successful flag
        } 
		else 
		{
            // If Credentials don't match, clear screen and print these messages out
            system("cls");
            nAttempts++;
            printf(RED);
            printf("Incorrect credentials.");
			printf(COLOR_RESET);
			printf(" %d attempts left.\n\a", MAX_ATTEMPTS - nAttempts);
        }
    }

    // Check if the login was not successful after 3 attempts
    if (!nLoginSuccessful) 
	{
        // Terminate transaction when user reaches 3 unsuccessful attempts
        printf("You have reached %d unsuccessful attempts, transaction will be terminated.\a\n", MAX_ATTEMPTS);
        nLoginSuccessful = 0; // Set failed flag
    }

    // Return whether login was successful or not
    return nLoginSuccessful;
}

/* This function displays the main menu
 Precondition: nDelay is a positive number 
 @param nDelay is the delay in which the display will appear on screen
*/
void displayMainMenu()
{
	displayDivider();
	
	Sleep(DISPLAY_DELAY);
	
	printf("Please select an option:\n\n");
	
	Sleep(DISPLAY_DELAY);
	
    printf("+--------------------------+   +--------------------------+\n");
    printf("|   [1] Change PIN         |   |   [2] Check Balance      |\n");
	printf("+--------------------------+   +--------------------------+\n");
	
	Sleep(DISPLAY_DELAY);
	
    printf("+--------------------------+   +--------------------------+\n");
    printf("|   [3] Deposit Money      |   |   [4] Withdraw Money     |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
	Sleep(DISPLAY_DELAY);
	
    printf("+--------------------------+   +--------------------------+\n");
    printf("|   [5] Transfer Funds     |   |   [6] Pay Bills          |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
	Sleep(DISPLAY_DELAY);
	
    printf("+--------------------------+   +--------------------------+\n");
    printf("|   [7] View Transactions  |   |   [0] Log Out            |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
    Sleep(DISPLAY_DELAY);
    
	printf("\n");
	    
    displayDivider();
}

/* This function is called after the user chooses an option,
 highlighting the option the user chose
 Precondition: nOption is a valid menu option
 @param nOption is the option the user chose
*/
void highlightMainMenu(int nOption)
{
	system("cls");
    displayDivider();
    
    printf("Please select an option:\n\n");
    
    /* Checks if the options in every line was chosen
     Highlight the option if it was chosen */
     
    // First line
    printf("+--------------------------+   +--------------------------+\n");
    // If the user chose change pin
    if (nOption == 1)
        printf("|" BHGREEN "   [1] Change PIN         " COLOR_RESET "|   |   [2] Check Balance      |\n");
    // If the user chose check balance
    else if (nOption == 2)
        printf("|   [1] Change PIN         |   |" BHGREEN "   [2] Check Balance      " COLOR_RESET "|\n");
    // If the user chose neither, print the normally
    else
        printf("|   [1] Change PIN         |   |   [2] Check Balance      |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
    
    // Second line
    printf("+--------------------------+   +--------------------------+\n");
    // If the user chose deposit money
    if (nOption == 3)
        printf("|" BHGREEN "   [3] Deposit Money      " COLOR_RESET "|   |   [4] Withdraw Money     |\n");
    // If the user chose withdraw money
    else if (nOption == 4)
        printf("|   [3] Deposit Money      |   |" BHGREEN "   [4] Withdraw Money     " COLOR_RESET "|\n");
    // If the user chose neither, print normally
    else
        printf("|   [3] Deposit Money      |   |   [4] Withdraw Money     |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
    
    // Third line
    printf("+--------------------------+   +--------------------------+\n");
    // If the user chose transfer funds
    if (nOption == 5)
        printf("|" BHGREEN "   [5] Transfer Funds     " COLOR_RESET "|   |   [6] Pay Bills          |\n");
    // If the user chose pay bills
    else if (nOption == 6)
        printf("|   [5] Transfer Funds     |   |" BHGREEN "   [6] Pay Bills          " COLOR_RESET "|\n");
    // If the user chose neither, print normally
    else
        printf("|   [5] Transfer Funds     |   |   [6] Pay Bills          |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
    
    // Fourth line
    printf("+--------------------------+   +--------------------------+\n");
    // If the user chose view transactions
    if (nOption == 7)
        printf("|" BHGREEN "   [7] View Transactions  " COLOR_RESET "|   |   [0] Log Out            |\n");
    // If the user chose to log out
    else if (nOption == 0)
        printf("|   [7] View Transactions  |   |" BHGREEN "   [0] Log Out            " COLOR_RESET "|\n");
    // If the user chose neither, print normally
    else
        printf("|   [7] View Transactions  |   |   [0] Log Out            |\n");
    printf("+--------------------------+   +--------------------------+\n");
    
    printf("\n");
        
    displayDivider();
}

/* This function displays the main menu and returns
   the option the user chooses
 Precondition: The option the user chooses is an integer from 0-7
 @return nOption
*/
int getMainMenuOption()
{
	
	int nOption;
	
	displayMainMenu();
	
	// Ask the user to choose an option
	printf("Enter Option Number:\n> ");
	getNumInput(&nOption);
	
	highlightMainMenu(nOption);
	loadingSequence("Processing", 2000);
	
	// Return the option chosen by the user
	return nOption;
	
}

/* This function allows the user 
   to change their PIN
 Precondition: nPinInput is between 100000 - 999999
 @param nCurrentPin is the current PIN of the user
 @param *nPinNumber is a pointer for the PIN of the user
 @return whether the pin change was successful (1) or not (0)
*/
int changePin(int nCurrentPin, int* nPinNumber) 
{
    system("cls");

    // Valid pin range
    const int nPinMin = 100000;
    const int nPinMax = 999999;

    int nPinInput = 0;
    int nNewPin = 0;
    int nAttempts = 0;
    int nContinueOperation = 1; // Flag to indicate if the operation should continue

    // While the user's input does not match their current pin
    while (nAttempts != MAX_ATTEMPTS && nCurrentPin != nPinInput && nContinueOperation) 
    {
        displayDivider();
        printf("Please enter your Current Pin:\n> ");
//        scanf("%d", &nPinInput);
//        clearInputBuffer(); // Clear input buffer
		getPinInput(&nPinInput);
		loadingSequence("Processing", 1000);
		
        // If the user's input does not match the current pin, prompt again
        if (nCurrentPin != nPinInput) 
        {
            system("cls");
            nAttempts++;
            printf(RED);
            printf("Entered PIN does not match Current PIN.\n");
            printf(COLOR_RESET);
            printf("Please try again. You have %d attempts left.\n", MAX_ATTEMPTS - nAttempts);
            printf("\n");
        }

        // Check if attempts reached 3
        if (nAttempts == MAX_ATTEMPTS) 
        {
            printf("You have reached %d unsuccessful attempts. Your account will be logged out.\n", MAX_ATTEMPTS);
            nContinueOperation = 0;  // Flag to indicate exit
        }
    }

    // Reset attempt counter when user successfully inputs Current Pin
    nAttempts = 0;

    // While New Pin is invalid
    while ((nNewPin < nPinMin || nNewPin > nPinMax || nNewPin == nCurrentPin) && nContinueOperation && nAttempts != MAX_ATTEMPTS) 
    {
        displayDivider();
        printf("NOTE: New Pin must be between %d - %d\n", nPinMin, nPinMax);
        displayDivider();

        printf("Enter your New Pin:\n> ");
//        scanf("%d", &nNewPin);
//        clearInputBuffer(); // Clear input buffer

		getPinInput(&nNewPin);
		loadingSequence("Processing", 1000);
		
        // Handle different invalid inputs
        if (nNewPin < nPinMin || nNewPin > nPinMax) 
        {
            system("cls");
            nAttempts++;
            printf(RED);
            printf("New Pin should be between %d and %d.\n", nPinMin, nPinMax);
            printf(COLOR_RESET);
            printf("Please try again. You have %d attempts left.\n", MAX_ATTEMPTS - nAttempts);
            printf("\n");
        } 
        else if (nNewPin == nCurrentPin) 
        {
            system("cls");
            nAttempts++;
            printf(RED);
            printf("New Pin should not match your Current Pin.\n");
            printf(COLOR_RESET);
            printf("Please try again. You have %d attempts left.\n", MAX_ATTEMPTS - nAttempts);
            printf("\n");
        }
    }

    // Check if attempts reached 3 and exit
    if (nAttempts == MAX_ATTEMPTS) 
    {
        system("cls");
        printf("You have reached %d unsuccessful attempts. Your account will be logged out.\n", MAX_ATTEMPTS);
        nContinueOperation = 0;  // Flag to indicate exit
    }

    // Reset attempt counter when user successfully inputs a valid New Pin
    nAttempts = 0;

    // While the user's input isn't the same as their new pin
    while (nPinInput != nNewPin && nContinueOperation) 
    {
        displayDivider();
        printf("Please re-enter your New Pin to confirm:\n> ");
//        scanf("%d", &nPinInput);
//        clearInputBuffer(); // Clear input buffer

		getPinInput(&nPinInput);

        // Handle invalid input if re-entered pin doesn't match
        if (nPinInput != nNewPin) 
        {
            system("cls");
            printf(RED);
            printf("Input does not match the New Pin.\n");
            printf(COLOR_RESET);
            printf("Please try again. You have %d attempts left.\n", MAX_ATTEMPTS - nAttempts);
            nAttempts++;
        }
    }

    // Check if attempts reached 3 and exit
    if (nAttempts == MAX_ATTEMPTS) 
    {
        system("cls");
        printf("You have reached %d unsuccessful attempts. Your account will be logged out.\n", MAX_ATTEMPTS);
        nContinueOperation = 0;  // Flag to indicate exit
    }

    // At the end of the function, return based on the nContinueOperation flag
    if (nContinueOperation) 
    {
        *nPinNumber = nNewPin;  // Set Current Pin to the New Pin
        loadingSequence("Processing", 1000);
		printf(GREEN);
        printf("Pin has been successfully changed. Please re-login.\n");
        printf(COLOR_RESET);
        displayDivider();
        system("pause");
        system("cls");
        return 1;  // Indicate success
    } 
    else 
    {
        return 0;  // Indicate failure (logged out)
    }
}


/* This function shows the user their
  current balance.
 Precondition: nAccountNumber is a valid account number & fBalance is a non-negative number
 @param nAccountNumber is the account number of the user
 @param fBalance is the current balance of the user
*/
void checkBalance(int nAccountNumber, float fBalance)
{
	
	loadingSequence("Fetching current balance", 3000);
	// Print out the Account Number and the Balance of the user
	system("cls");
	
	displayDivider();
	
	Sleep(DISPLAY_DELAY);
	
	printf("Account Number: %d\n", nAccountNumber);
	
	Sleep(DISPLAY_DELAY);
	
	printf("Your current balance is: P%.2f\n", fBalance);
	
	Sleep(DISPLAY_DELAY);
	
	displayDivider();
	
	Sleep(DISPLAY_DELAY);
	
	// Pause the screen and wait for the user to press any key
	system("pause");
	
	// Clear the screen after user presses a key
	system("cls");
	
}

/* This function allows the user to deposit money to
	their account
 Precondition: fBalance is a non-negative number
 @param *fBalance is a pointer for the balance of the user
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param *nTransactionCount is the number of transactions made
*/
void depositMoney(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount)
{
    system("cls");

    int nDenomination;
    int nNoOfBills;
    float fCurrentDeposit = 0;
    float fPreviousDeposit = 0;
    float fRunningDeposit = 0;
    int nReferenceNum = 0;
    char cChoice = 'Y'; // Initialized to Y to enter loop
    int nValidChoice = 0; // Flag for valid denomination selection
    int nValidBills = 0; // Flag for valid number of bills

    do 
    {
        // Set current deposit, denomination, and number of bills to 0
        fCurrentDeposit = 0;
        nDenomination = 0;
        nNoOfBills = 0;

        // Print the Running Deposit
        printf("Running Deposit: P%.2f\n", fRunningDeposit);

        displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
        // Print denomination options
        printf("Choose your deposit denomination: \n");
        
        printf("+----------------+   +----------------+   +-----------------+\n");
		printf("|    [1] P100    |   |    [2] P200    |   |   [0] Cancel    |\n");
		printf("+----------------+   +----------------+   +-----------------+\n");
		
		Sleep(DISPLAY_DELAY);
		
		printf("+----------------+   +----------------+\n");
		printf("|    [3] P500    |   |    [4] P1000   |\n");
		printf("+----------------+   +----------------+\n");

        displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
        // Scan for chosen option
        printf("Denomination:\n> ");
//        scanf("%d", &nDenomination);
//        clearInputBuffer(); // Clear input buffer

		getNumInput(&nDenomination);

        // Set the denomination to the equivalent of the user's choice
        switch (nDenomination)
        {
            case 1:
                nDenomination = 100;
                nValidChoice = 1; // Valid choice
                break;

            case 2:
                nDenomination = 200;
                nValidChoice = 1; // Valid choice
                break;

            case 3:
                nDenomination = 500;
                nValidChoice = 1; // Valid choice
                break;

            case 4:
                nDenomination = 1000;
                nValidChoice = 1; // Valid choice
                break;

            case 0:
            	fRunningDeposit = 0;
            	loadingSequence("Processing", 1000);
                printf(GREEN);
                printf("You have cancelled the transaction.\n");
                printf(COLOR_RESET);
                loadingSequence("Returning to main menu", 4000);
                nValidChoice = 2; // Flag to indicate cancellation
                break;

            default:
                system("cls");
                printf(RED);
                printf("Invalid Option. Please try again.\n");
                printf(COLOR_RESET);
                nValidChoice = 0; // Invalid choice
                continue;
        }

        // Proceed only if the choice is valid
        if (nValidChoice == 1) 
        {
            // Ask the user for a valid number of bills
            nValidBills = 0; // Reset valid bills flag

            while (!nValidBills) // Loop until a valid number of bills is provided
            {
                printf("Number of bills (up to 10 bills only):\n> ");
//                scanf("%d", &nNoOfBills);
//
//                clearInputBuffer(); // Clear input buffer

				getNumInput(&nNoOfBills);

                // If the number of bills is valid
                if (nNoOfBills <= 10 && nNoOfBills > 0)
                {
                    nValidBills = 1; // Set valid bills flag to 1
                }
                else
                {
                    system("cls");
                    printf(RED);
                    printf("Invalid Number of Bills. Please try again.\n"); // Print out error message
                    printf(COLOR_RESET);
                    printf("Denomination: %d\n", nDenomination); // Print out denomination for reference
                    displayDivider();
                }
            }

            fCurrentDeposit = nDenomination * nNoOfBills; // Set the Current Deposit
            fRunningDeposit = fPreviousDeposit + fCurrentDeposit; // Update Running Deposit
            fPreviousDeposit = fRunningDeposit; // Update Previous Deposit

            printf("Current Deposit: %.2f\n", fCurrentDeposit); // Show Current Deposit
            printf("Running Deposit: %.2f\n", fRunningDeposit); // Show Running Deposit

            // Ask the user if they wish to deposit more money
            do
            {
                printf("Do you wish to deposit more? Y/N:\n> ");	
                scanf(" %c", &cChoice);
                clearInputBuffer(); // Clear input buffer

                // Convert the choice to uppercase
                cChoice = toupper(cChoice);

                // If the choice is not Y or N
                if (cChoice != 'Y' && cChoice != 'N')
                {
                    system("cls");
                    printf(RED);
                    printf("Invalid Choice. Please try again.\n"); // Print out error message
                    printf(COLOR_RESET);
                }

            } while (cChoice != 'Y' && cChoice != 'N');
			
			loadingSequence("Processing", 1000);
        }

    } while (nValidChoice != 2 && cChoice == 'Y'); // Continue while the transaction is not cancelled and the choice is Y

    // Show the transaction result
    if (fRunningDeposit > 0 || nValidChoice == 1) // Check if there was a deposit or if the user cancelled the transaction
    {
    	// Update the balance after the deposits
    	*fBalance += fRunningDeposit; // Add the Running Deposit to the Balance
    	nReferenceNum = generateRefNum(); // Generate reference number
    	printf(GREEN);
        printf("You have successfully deposited P%.2f to your account.\n", fRunningDeposit); // Display total money deposited
        printf(COLOR_RESET);
        printf("Your new balance is P%.2f.\n", *fBalance); // Display new balance
        // Print out receipt
        printReceipt(DEPOSIT, fRunningDeposit, 0, NULL, fBalance, nReferenceNum);
        // Log the transaction
        logTransaction(DEPOSIT, fRunningDeposit, *fBalance, nReferenceNum, nTransactionType, fTransactionValues, nReferenceNums, nTransactionCount);   
    }
    
    return;
}

/* This function allows the user to withdraw money from
	their account
 Precondition: fBalance is a non-negative number
 @param *fBalance is a pointer for the balance of the user
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param *nTransactionCount is the number of transactions made
*/
void withdrawMoney(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount)
{
    int nWithdraw;
    int nReferenceNum;
    int nValidTransaction = 0; // Flag to indicate if the transaction is valid

    // Check immediately if balance is 0.00
    if (*fBalance <= 0)
    {
        // Cancel transaction
        system("cls");
        displayDivider();
        printf(RED);
        printf("Sorry, your balance is 0.00.\n");
        printf(COLOR_RESET);
        printf("Withdrawal will be cancelled.\n");
        loadingSequence("Returning to main menu", 4000);
        nValidTransaction = 2; // Indicate cancelled transaction
    }
    
	system("cls");
       
	while (nValidTransaction == 0)
	{
		do
		{
	        printf("Your current balance: P%.2f\n", *fBalance); // Print out current balance for reference
	        displayDivider();
	        
	        Sleep(DISPLAY_DELAY);
	        
	        printf("Please select from the options below:\n"); 
	        
	        Sleep(DISPLAY_DELAY);
	        
	        printf("+-----------------+   +-----------------+   +-----------------+   +-----------------+\n");
			printf("|   [1] P500      |   |   [2] P1000     |   |   [3] P2000     |   |   [4] P3000     |\n");
			printf("+-----------------+   +-----------------+   +-----------------+   +-----------------+\n");
			
			Sleep(DISPLAY_DELAY);
			
			printf("+-----------------+   +-----------------+   +-----------------+   +-----------------+\n");
			printf("|   [5] P4000     |   |   [6] P5000     |   |   [7] Custom    |   |   [0] Cancel    |\n");
			printf("+-----------------+   +-----------------+   +-----------------+   +-----------------+\n");
			
			displayDivider();
			
			Sleep(DISPLAY_DELAY);
		    // Ask user for the amount to withdraw
		    printf("> ");
	//	    scanf("%d", &nWithdraw);
	//	    clearInputBuffer(); // Clear input buffer
			
			getNumInput(&nWithdraw);
			loadingSequence("Processing", 1000);
			
			if (!(nWithdraw >= 1 && nWithdraw <= 7) && nWithdraw != 0)
			{
				system("cls");
				printf(RED);
				printf("Please input a valid option\n");
				printf(COLOR_RESET);
				displayDivider();
			}
			
		} while (!(nWithdraw >= 1 && nWithdraw <= 7) && nWithdraw != 0);
		
		// If the input was one of the options available
    	if (nWithdraw >= 1 && nWithdraw <= 7)
    	{
    		switch (nWithdraw)
    		{
    		case 1:
    			nWithdraw = 500;
    			break;
    		case 2:
    			nWithdraw = 1000;
    			break;
    		case 3:
    			nWithdraw = 2000;
    			break;
    		case 4:
    			nWithdraw = 3000;
    			break;
    		case 5:
    			nWithdraw = 4000;
    			break;
    		case 6:
    			nWithdraw = 5000;
    			break;
    		case 7:
    			system("cls");
    			printf("Withdrawal amounts should be in intervals of P100.00 only.\n");
    			
    			Sleep(DISPLAY_DELAY);
    			
			    displayDivider();
			    printf("+-----------------+\n");
    			printf("|   [0] Cancel    |\n");
    			printf("+-----------------+\n");
    			displayDivider();
    			
    			Sleep(DISPLAY_DELAY);
    			
    			printf("How much do you wish to withdraw?\n> ");
    			getNumInput(&nWithdraw);
    			loadingSequence("Processing", 1000);
    			break;
			}
		}
		
        // Check if the input is 0
        if (nWithdraw == 0)
        {
            system("cls");
            // Cancel transaction
            printf(GREEN);
            printf("You have cancelled the transaction.\n");
            printf(COLOR_RESET);
            loadingSequence("Returning to main menu", 4000);
            nValidTransaction = 2; // Indicate cancelled transaction
        }
        // If the withdrawal amount is greater than the balance
        else if (nWithdraw > *fBalance)
        {
            system("cls");
            printf(RED);
            printf("Sorry, you have insufficient balance. Your current balance is P%.2f.\n", *fBalance);
            printf(COLOR_RESET);
            printf("Please try again.\n");
            displayDivider();
        }
        // If the withdrawal amount is negative
        else if (nWithdraw < 0)
        {
            system("cls");
            printf(RED);
            printf("Withdrawal amounts must be positive integers\n");
            printf(COLOR_RESET);
            printf("Please try again.\n");
            displayDivider();
        }
        // If the withdrawal amount is not divisible by 100
        else if ((int)nWithdraw % 100 != 0)
        {
            system("cls");
            printf(RED);
            printf("Withdrawal amounts must be in intervals of P100.00\n");
            printf(COLOR_RESET);
            printf("Please try again.\n");
            displayDivider();
        }
        // If all checks are passed
        else
        {
        	nValidTransaction = 1; // Indicate valid transaction
		}
	}

    // Proceed with the withdrawal if nValidTransaction = 1
    if (nValidTransaction == 1)
    {
        system("cls");
        *fBalance -= nWithdraw; // Subtract the amount from the current balance
        nReferenceNum = generateRefNum(); // Generate reference number
        printf(GREEN);
        printf("P%.2f has been withdrawn from your account\n", (float)nWithdraw); // Print amount withdrawn from account
        printf(COLOR_RESET);
        printf("Your new balance is P%.2f\n", *fBalance); // Print new balance
        // Print out receipt
		printReceipt(WITHDRAW, nWithdraw, 0, NULL, fBalance, nReferenceNum);
        logTransaction(WITHDRAW, -nWithdraw, *fBalance, nReferenceNum, nTransactionType, fTransactionValues, nReferenceNums, nTransactionCount); // Log transaction
    }
    return;
}

/* This function allows the user to trasnfer money to
	another account
 Precondition: fBalance is a non-negative number
 @param *fBalance is a pointer for the balance of the user
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param *nTransactionCount is the number of transactions made
*/
void transferFunds(float *fBalance, int nUserAccountNumber, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount) 
{
	system("cls");
	
	// Valid account number range
	const int nAcctNumMin = 10000000;
	const int nAcctNumMax = 99999999;
	
	// Valid transfer amount range
	const float fTransferMin = 1;
	const float fTransferMax = 50000;
	
    int nAccountNumber = 0; // Initialized to an invalid value to enter the loop
    float fTransfer = -1.0;  // Initialized to an invalid value to enter the loop
    int nReferenceNum;
    int nTransactionCancelled = 0; // Flag to indicate if transaction is canceled

    // While account number is not valid, keep asking
    while (!nTransactionCancelled && (nAccountNumber < nAcctNumMin || nAccountNumber > nAcctNumMax) || nAccountNumber == nUserAccountNumber) 
	{
        printf("Account Number should be from 10000000 - 99999999.\n");
        
        Sleep(DISPLAY_DELAY);
        
        displayDivider();
        printf("+-----------------+\n");
		printf("|   [0] Cancel    |\n");
		printf("+-----------------+\n");
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
        // Ask for the account number of the recipient
        printf("Enter the Account Number of the recipient:\n> ");
        getNumInput(&nAccountNumber);
		
		loadingSequence("Processing", 1000);
		
        // If user cancels by entering 0
        if (nAccountNumber == 0) 
		{
            nTransactionCancelled = 1; // Set flag to 1 indicating cancellation
        } 
        // If account number is not within range
		else if (nAccountNumber < nAcctNumMin || nAccountNumber > nAcctNumMax) 
		{
            system("cls");
            printf(RED);
            printf("Invalid Account Number. Please try again.\n");
            printf(COLOR_RESET);
        }
        else if (nAccountNumber == nUserAccountNumber)
        {
        	system("cls");
            printf(RED);
            printf("Transfer to own account is not allowed. Please try again.\n");
            printf(COLOR_RESET);
		}
    }
	
    // While transaction has not been cancelled and transfer amount is greater than the current balance / not within range
    while (!nTransactionCancelled && (fTransfer > *fBalance || fTransfer < fTransferMin || fTransfer > fTransferMax)) 
	{
		system("cls");
		printf("Transfer amount should be between P1.00 - P50000.00\n");
		printf("Your current balance: %.2f.\n", *fBalance);
		Sleep(DISPLAY_DELAY);
		
		displayDivider();
		printf("+-----------------+\n");
		printf("|   [0] Cancel    |\n");
		printf("+-----------------+\n");
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
        // Ask for transfer amount
        printf("Enter amount to transfer:\n> ");
        scanf("%f", &fTransfer);

		loadingSequence("Processing", 1000);
		
        // If user cancels by entering 0
        if (fTransfer == 0) 
		{
            nTransactionCancelled = 1; // Set flag to 1 indicating cancellation
        } 
        // If the transfer amount is not within range
		else if (fTransfer < fTransferMin || fTransfer > fTransferMax) 
		{
            system("cls");
            printf(RED);
            printf("Transfer amount must be between P1.00 to P50000.00, please try again.\n");
            printf(COLOR_RESET);
        } 
        // If the transfer amount is greater than the current balance
		else if (fTransfer > *fBalance) 
		{
            system("cls");
            printf(RED);
            printf("You need more funds to make this transfer. Please try again.\n");
            printf(COLOR_RESET);
        }
    }
    
    // Only proceed if the transaction wasn't canceled
    if (!nTransactionCancelled) 
	{
        system("cls");
        displayDivider();
        *fBalance -= fTransfer; // Subtract the transfer amount from the balance
        nReferenceNum = generateRefNum(); // Generate reference number
        printf(GREEN);
        printf("P%.2f has been transferred to account number %d.\n", fTransfer, nAccountNumber);
        printf(COLOR_RESET);
        printf("Your new balance is P%.2f.\n", *fBalance);
        // Print out receipt
        printReceipt(TRANSFER, fTransfer, nAccountNumber, NULL, fBalance, nReferenceNum);
        logTransaction(TRANSFER, -fTransfer, *fBalance, nReferenceNum, nTransactionType, fTransactionValues, nReferenceNums, nTransactionCount); // Log the transaction
    } 
    // Otherwise, return to the main menu
	else 
	{
        system("cls");
        displayDivider();
        printf(GREEN);
        printf("You have cancelled the transaction.\n");
        printf(COLOR_RESET);
        loadingSequence("Returning to main menu", 4000);
    }
    
    return;
}

/* This function allows the user to pay bills to their
	chosen billers using their account
 Precondition: fBalance is a non-negative number
 @param *fBalance is a pointer for the balance of the user
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param *nTransactionCount is the number of transactions made
*/
void payBills(float *fBalance, int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int *nTransactionCount)
{
	system("cls");
	
	int nBiller = -1; // Initialized to invalid biller to enter loop
	char *strBillerName; // Biller's name
	float fPayment = 0; // Initialized to 0 to enter loop
	int nNumOfBillers = 5; // 5 billers default
	int nReferenceNum;
	int nTransactionCancelled = 0; // Flag to indicate if transaction is cancelled
	
	// While the transaction hasn't been cancelled and chosen biller is not in range
	while (!nTransactionCancelled && (nBiller < 0 || nBiller > nNumOfBillers))
	{
		displayDivider();
		printf("Available balance: P%.2f\n", *fBalance);
		printf("Please choose from the billers below.\n"); // Ask for the chosen biller
		
		Sleep(DISPLAY_DELAY);
		
		// Display list of options
		printf("[1] - Meralco\n");
		printf("[2] - Maynilad\n");
		printf("[3] - Converge ICT\n");
		printf("[4] - Smart Communications\n");
		printf("[5] - RCBC Credit\n");
		printf("[0] - Cancel\n");
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
		printf("Enter biller number:\n> ");
		getNumInput(&nBiller);
		
		loadingSequence("Processing", 1000);
		
		// Print out error message if biller is invalid
		if (nBiller < 0 || nBiller > nNumOfBillers)
		{
			system("cls");
			printf(RED);
			printf("The chosen biller does not exist. Please try again.\n");
			printf(COLOR_RESET);
		}
		// If user enters 0
		else if (nBiller == 0)
		{
			nTransactionCancelled = 1; // Set flag to 1 indicating cancellation
		}
		
	}
	
	// Set biller name to the choice of biller
	switch (nBiller)
	{
		// [1] - Meralco
		case 1:
			strBillerName = "Meralco";
			break;
		// [2] - Maynilad	
		case 2:
			strBillerName = "Maynilad";
			break;
		// [3] - Converge ICT
		case 3:
			strBillerName = "Converge ICT";
			break;
		// [4] - Smart Communications
		case 4:
			strBillerName = "Smart Communications";
			break;
		// [5] - RCBC Credit
		case 5:
			strBillerName = "RCBC Credit";
			break;
	}
		
	// While payment is not greater than 0 or payment is greater than the current balance
	while (!nTransactionCancelled && (fPayment <= 0 || fPayment > *fBalance))
	{
		printf("Payment must be a positive integer.\n");
		printf("Biller: %s\n", strBillerName);
		printf("Your current balance: P%.2f\n", *fBalance); // Print out current balance for reference
		
		Sleep(DISPLAY_DELAY);
		
		displayDivider();
	    printf("+-----------------+\n");
		printf("|   [0] Cancel    |\n");
		printf("+-----------------+\n");
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		// Ask for the amount to pay
		printf("Enter amount to pay:\n> ");
//		scanf("%f", &fPayment);
//		clearInputBuffer();

		getDecimalInput(&fPayment);
		
		loadingSequence("Processing", 1000);
		
		// If user cancelled the transaction
		if (fPayment == 0)
		{
			nTransactionCancelled = 1; // Set flag to 1 indicating cancellation
		}
		// If payment is greater than the current balance
		else if (fPayment > *fBalance)
		{
			// Print out error message and re-prompt
			system("cls");
			printf(RED);
			printf("You don't have enough funds to make this payment. Please try again.\n");
			printf(COLOR_RESET);
		}
		// If payment is negative
		else if (fPayment < 0)
		{
			// Print out error message and re-prompt
			system("cls");
			printf(RED);
			printf("Payment must be a positive integer. Please try again.\n");
			printf(COLOR_RESET);
		}
	}
	
	// Proceed with payment if transaction was never cancelled
	if (!nTransactionCancelled)
	{	
		system("cls");
		*fBalance -= fPayment; // Subtract payment from balance
		nReferenceNum = generateRefNum(); // Generate reference number
		printf(GREEN);
		printf("P%.2f has been paid to %s\n", fPayment, strBillerName); // Print payment message
		printf(COLOR_RESET);
		printf("Your new balance is P%.2f.\n", *fBalance); // Display new balance
		// Print out receipt
        printReceipt(PAYBILLS, fPayment, 0, strBillerName, fBalance, nReferenceNum);
		logTransaction(PAYBILLS, -fPayment, *fBalance, nReferenceNum, nTransactionType, fTransactionValues, nReferenceNums, nTransactionCount); // Log the transaction
	}
	// Otherwise, return to main menu
	else
	{
		system("cls");
        displayDivider();
        printf(GREEN);
        printf("You have cancelled the transaction.\n");
        printf(COLOR_RESET);
        loadingSequence("Returning to main menu", 4000);
	}
	
	return;
}

/* This function logs transactions by storing them into
	arrays and updates the transaction count
 Precondition: nType is an integer between 1-4
 			   fAmount is a positive or negative number
 			   fBalance is a non-negative number
 			   nReferenceNum is an 8 digit number
 @param nType corresponds to the type of the transaction
 @param fAmount is the amount added to or subtracted from the account
 @param fBalance is the running balance of the account after the transaction
 @param nReferenceNum is the reference number of the transaction
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param *nTransactionCount is the number of transactions made 
*/
void logTransaction(int nType, float fAmount, float fBalance, int nReferenceNum, int nTransactionType[], float fTransactionValues[][2], 
					int nReferenceNums[], int *nTransactionCount)
{
	const int nMaxTransactions = 5; // Maximum number of transactions to be displayed
	
	// Shift the elements of the array to the right
	for (int i = nMaxTransactions - 1; i > 0; i--)
	{
		nTransactionType[i] = nTransactionType[i - 1];
		fTransactionValues[i][0] = fTransactionValues[i - 1][0];
		fTransactionValues[i][1] = fTransactionValues[i - 1][1];
		nReferenceNums[i] = nReferenceNums[i - 1];
	}	
	
	// Add the most recent transaction to first index
	nTransactionType[0] = nType;
	fTransactionValues[0][0] = fAmount;
	fTransactionValues[0][1] = fBalance;
	nReferenceNums[0] = nReferenceNum;
	
	// If array is full
	if (*nTransactionCount >= nMaxTransactions) 
	{
		// Make sure transaction count stays at the number of max transactions
		*nTransactionCount = nMaxTransactions;
	}
	// Else if the array is not full yet
	else
	{
		// Increment transaction count by 1
		(*nTransactionCount)++;
	}
	
	return;
}

/* This function displays the last 5 transactions made by the user
 Precondition: nTransactionType & fTransactionValues have stored values
 @param nTransactionType[] is the array storing the types of the transactions
 @param fTransactionValues[][2] is the array storing the amount and running balance of each transaction
 @param nReferenceNums[] is the array that stores the reference numbers of transactions
 @param nTransactionCount is the current number of transactions made
*/
void displayTransactions(int nTransactionType[], float fTransactionValues[][2], int nReferenceNums[], int nTransactionCount)
{
	system("cls");
	
	loadingSequence("Fetching past transactions", 3000);
	
	// If transaction count is greater than 0
	if (nTransactionCount > 0)
	{

		// Print out the headers
		displayDivider();
		printf("| Transaction # | Transaction Type   | Amount       | Running Balance | Reference # |\n");
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
		// Print out transaction details
		for (int i = 0; i < nTransactionCount; i++)
		{
			// Print transaction number
			printf("|    %4d       | ", i + 1); // Current transaction will be i + 1 since i starts at 0
			// Check for the transaction type
			switch(nTransactionType[i])
			{
				// Print out transaction type
				case DEPOSIT: 
					printf("%-18s | ", "Deposit");
					break;
				case WITHDRAW:
					printf("%-18s | ", "Withdraw");
					break;
				case TRANSFER:
					printf("%-18s | ", "Transfer of Funds");
					break;
				case PAYBILLS:
					printf("%-18s | ", "Bill Payment");
					break;
				default:
					printf("%-18s | ", "Unknown");
	                break;
			}
			
			Sleep(DISPLAY_DELAY);
			
			// Print out the transaction amount
	        if (fTransactionValues[i][0] >= 0) // If the transaction amount is positive (or zero)
	        {
	        	printf(GREEN);
	            printf("+ P%9.2f", fabs(fTransactionValues[i][0])); // Using fabs to ensure no double sign
	            printf(COLOR_RESET);
	            printf(" |    ");
	        }
	        else // If the transaction amount is negative
			{
				printf(RED);
	            printf("- P%9.2f", fabs(fTransactionValues[i][0])); // Using fabs to ensure no double sign
	            printf(COLOR_RESET);
	            printf(" |    ");
			}
			
			Sleep(DISPLAY_DELAY);
			
	        // Print the running balance & reference number
	        printf("P%9.2f   | %10d  |", fTransactionValues[i][1], nReferenceNums[i]);
	        printf("\n");
		}
		displayDivider();
		
		Sleep(DISPLAY_DELAY);
		
		system("pause");
		system("cls");
		return;
	}
	// If there are no transactions made
	else
	{
		displayDivider();
		printf("There are no past transactions available to view at this time.\n");
		displayDivider();
		Sleep(2000);
		system("pause");
		system("cls");
		return;
	}
}

/* This function is for printing receipts after transactions are made
 Precondition: nTransactionType is an integer from 1-4
 			   fTransactionAmount and *fBalance is a non-negative number
 			   nAccountNumber is a number between 10000000 - 99999999
 @param nTransactionType represents the type of transaction that was made
 @param fTransactionAmount is the amount added or subtracted from the account during a transaction
 @param nAccountNumber is an account number where a transfer has been made
 @param *strBillerName is a string representing a biller's name
 @param *fBalance is the balance after the transaction was made
*/
void printReceipt(int nTransactionType, float fTransactionAmount, int nAccountNumber, char *strBillerName, float *fBalance, int nReferenceNum)
{
	displayDivider();
	printf("TRANSACTION RECEIPT\n");
	printf("\n");
	
	Sleep(DISPLAY_DELAY);
	
	// Print different formats based on transaction type
	switch (nTransactionType)
	{
		
		// Deposit
		case 1:
			printf("Transaction Type:   Deposit\n");
		    printf("Deposit Amount:     P %.2f\n", fTransactionAmount);
		    printf("Previous Balance:   P %.2f\n", *fBalance - fTransactionAmount);
			break;
			
		// Withdraw
		case 2:
			printf("Transaction Type:   Withdraw\n");
	        printf("Withdraw Amount:    P %.2f\n", fTransactionAmount);
	        printf("Previous Balance:   P %.2f\n", *fBalance + fTransactionAmount);
			break;
			
		// Transfer of Funds
		case 3:
			printf("Transaction Type:   Transfer of Funds\n");
	        printf("Transfer to:        %d\n", nAccountNumber);
	        printf("Transfer Amount:    P %.2f\n", fTransactionAmount);
	        printf("Previous Balance:   P %.2f\n", *fBalance + fTransactionAmount);
			break;
			
		// Bill Payment
		case 4:
			printf("Transaction Type:   Bill Payment\n");
	        printf("Payment to:         %s\n", strBillerName);
	        printf("Payment Amount:     P %.2f\n", fTransactionAmount);
	        printf("Previous Balance:   P %.2f\n", *fBalance + fTransactionAmount);
			break;	
	}
	
	Sleep(DISPLAY_DELAY);
	
	// Print new balance and reference number
	printf("New Balance:        P %.2f\n", *fBalance);
	printf("Reference Number:   %d\n", nReferenceNum);
	displayDivider();
	
	system("pause");
    system("cls");
}

