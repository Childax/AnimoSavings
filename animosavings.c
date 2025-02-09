/************************************************************
******
This is to certify that this project is my own work, based on 
my personal efforts in studying and applying the concepts 
learned. I have constructed the functions and their respective 
algorithms
and corresponding code by myself. The program was run, tested, 
and debugged by my own efforts. I further certify that I have 
not copied in part or whole or otherwise plagiarized the work 
of other students and/or persons. 
Richmond Jose G. Ramos, DLSU ID# 12414034
*************************************************************
*****/

/******************************************
Description: 
This program is designed to simulate the functionality of a Cash Deposit Machine (CDM) and an Automated Teller Machine (ATM), 
allowing users to perform various banking tasks. It starts by verifying the user’s identity through an account number and PIN, 
ensuring only authorized access. Once logged in, users can view their account balance, deposit money, withdraw funds, 
transfer money to other accounts, and pay bills. The program also tracks each transaction, recording the type of transaction,
the amount involved, and the updated balance. Users can also view a history of their past transactions, summarizing the details. 

 Programmed by: Richmond Jose G. Ramos S23B
 Last modified: November 24, 2024
 Version: 6.0
 
 [Acknowledgements: 
 https://stackoverflow.com/questions/7898215/how-can-i-clear-an-input-buffer-in-c - for clearing input buffer

 https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a - for color codes

 https://stackoverflow.com/questions/68151171/masking-password-input-in-windows-terminal-in-c - for masking input

 https://stackoverflow.com/questions/8132245/backspace-and-delete-issues-with-the-getch-function-in-dos - for deleting masked input
 ]
 
******************************************/

#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "bankfunctions.c"

int main()
{
	// Declaration of default values
	const int nAccountNumber = 12414034; // Account number never changes
	int nPinNumber = 123456;
	float fBalance = 1000;
	
	int nLoginFlag = 0; // Flag for checking if user exited the program during account login
	
	// Declaration of variables for logging transactions
	const int nMaxTransactions = 5; // Max transactions to be displayed
	int nTransactionCount = 0; // Keeps track of number of transactions
	int nTransactionType[nMaxTransactions]; // Array for storing transaction types
	float fTransactionValues[nMaxTransactions][2]; // 2D array for storing cash values for each transaction (Amount & Running Balance)
	int nReferenceNums[nMaxTransactions]; // Array for storing reference numbers
	
	// Call account login in while loop
	while ((nLoginFlag = verifyAccountLogin(nAccountNumber, nPinNumber)) != 0 && nLoginFlag != 2)
	{
		int nOption;
		// Loop while user hasn't chosen to log out
		while ((nOption = getMainMenuOption()) != 0)
		{
			// Handle different options
			switch (nOption)
			{
			// User chooses to change their Pin
			case 1:
				// Exit program when pin change is unsuccessful
				if (!changePin(nPinNumber, &nPinNumber))
				{
					return 2; // Return value 2 indicates termination by failed pin change
				}
				// Otherwise, prompt user to relogin 
				// If verifyAccountLogin returns false, exit the program
				else if (!verifyAccountLogin(nAccountNumber, nPinNumber))
				{
					return 3; // Return value 3 indicates termination by failed login after pin change
				}
				break;
			
			// User chooses to check their balance	
			case 2:
				checkBalance(nAccountNumber, fBalance);
				break;	
				
			// User chooses to deposit to their account
			case 3:
				depositMoney(&fBalance, nTransactionType, fTransactionValues, nReferenceNums, &nTransactionCount);
				break;
			
			// User chooses to withdraw money from their account
			case 4:
				withdrawMoney(&fBalance, nTransactionType, fTransactionValues, nReferenceNums, &nTransactionCount);
				break;
			
			// User chooses to transfer funds from their account to another
			case 5:
				transferFunds(&fBalance, nAccountNumber, nTransactionType, fTransactionValues, nReferenceNums, &nTransactionCount);
				break;
				
			// User chooses to pay bills	
			case 6:
				payBills(&fBalance, nTransactionType, fTransactionValues, nReferenceNums, &nTransactionCount);
				break;
			
			// User chooses to view transactions
			case 7:
				displayTransactions(nTransactionType, fTransactionValues, nReferenceNums, nTransactionCount);
				break;
			
			// User chooses an invalid option
			default:
                system("cls");
                printf(RED);
                printf("Invalid option. Please choose a valid menu option.\n");
                printf(COLOR_RESET);
                break;
			}
			
		}
		
		/* Print this out when user chooses to log out 
		(thus breaking out of the main menu while loop) */
		system("cls");	
		printf(GREEN);
		printf("You have logged out of your account.\n");
		printf(COLOR_RESET);
		loadingSequence("Logging you out", 5000);
	}
	
	// Handle different login flags
	switch (nLoginFlag)
	{
		// User exits program
		case 2:
			loadingSequence("Exiting Program", 5000);
			displayEndScreen();
			return 0; // Return value 0 indicates user exited the program
		// User fails 3 login attempts
		default:
			loadingSequence("Exiting Program", 5000);
			displayEndScreen();
			return 1; // Return value 1 indicates termination by failed login
			
	}
}