#include "Bank.hpp"									// The class under demonstration, always the first include

# include <exception>								// std::exception, the base of every standard exception
# include <iostream>									// std::cout, std::cerr and std::endl
# include <stdexcept>								// std::out_of_range and std::invalid_argument

// ---------------------------------------------------------------------------- //
// main - demonstration of the encapsulated bank										//
// ---------------------------------------------------------------------------- //

// Everything the bank can refuse to do is reported by an exception, so the whole
// demonstration runs inside a try block and the catch at the bottom turns any
// failure into a clean error message instead of a crash.
int	main()												// The program entry point, returns int as the standard requires
{														// Open the body
	try												// Every operation below may throw, so guard the whole thing
	{													// Open the try block

		std::cout << "=== Creation of the bank ===" << std::endl;		// Section header
		Bank bank(999);									// The bank starts with 999 of its own money
		std::cout << bank << std::endl;					// Nothing to show yet, the bank has no account

		std::cout << "=== Account creation ===" << std::endl;		// Section header
		bank.createAccount(0, 100);						// Alice opens the account number 0 with a balance of 100
		bank.createAccount(1, 100);						// Bob opens the account number 1 with a balance of 100
		std::cout << bank << std::endl;					// Two accounts, keyed 0 and 1

		std::cout << "=== Deposit, the bank keeps 5% ===" << std::endl;	// Section header
		bank.deposit(0, 400);							// Alice deposits 400, the bank skims 20 and gives her 380
		std::cout << "Alice now owns : " << bank[0] << std::endl;		// 100 + 380 = 480, read through operator[]
		std::cout << "Bank liquidity : " << bank.getLiquidity() << std::endl;	// 999 + 20 = 1019
		std::cout << "Alice id is still : " << bank.getAccount(0).getId() << std::endl;	// Read through a const getter

		std::cout << "=== Loans ===" << std::endl;			// Section header
		const bool granted = bank.giveLoan(1, 200);		// Bob borrows 200, the bank can afford it
		std::cout << "Loan of 200 granted : " << granted << std::endl;	// Expected: true, printed as 1
		std::cout << "Bob now owns : " << bank[1] << std::endl;		// 100 + 200 = 300
		const bool refused = bank.giveLoan(1, 100000);	// The bank does not own that much, the loan is refused
		std::cout << "Loan of 100000 granted : " << refused << std::endl;	// Expected: false, printed as 0
		std::cout << "Liquidity untouched : " << bank.getLiquidity() << std::endl;	// Still 819, a refusal changes nothing

		std::cout << "=== Account deletion ===" << std::endl;	// Section header
		bank.deleteAccount(0);							// Alice closes her account number 0
		std::cout << "Accounts left : " << bank.getNbAccounts() << std::endl;	// Only Bob is left
		// This is the whole point of keying the map by id: Bob keeps the id 1
		// even though account 0 no longer exists. No reindexing, no surprise.
		std::cout << "Bob id after the deletion : " << bank.getAccount(1).getId() << std::endl;	// Still 1
		std::cout << "Bob now owns : " << bank[1] << std::endl;		// Still 300
		std::cout << bank << std::endl;					// Final state of the bank

		std::cout << "=== Refused operations ===" << std::endl;	// Section header

		try											// The bank must never crash, so guard each misuse on its own
		{												// Open the inner try block
			bank[42];									// Asking for an account number that was never opened
		}												// Close the inner try block
		catch (const std::exception& e)					// Catch by const reference, as the standard requires
		{												// Open the inner catch block
			std::cout << "Caught : " << e.what() << std::endl;	// what() returns the message carried by the exception
		}												// Close the inner catch block

		try											// Second misuse, same pattern
		{												// Open the inner try block
			bank.deposit(0, -50);						// A negative deposit is meaningless
		}												// Close the inner try block
		catch (const std::exception& e)					// Catch by const reference
		{												// Open the inner catch block
			std::cout << "Caught : " << e.what() << std::endl;	// Print the message
		}												// Close the inner catch block

		try											// Third misuse: opening an account number twice
		{												// Open the inner try block
			bank.createAccount(1, 500);					// Bob's account number 1 is already taken
		}												// Close the inner try block
		catch (const std::exception& e)					// Catch by const reference
		{												// Open the inner catch block
			std::cout << "Caught : " << e.what() << std::endl;	// Print the message
		}												// Close the inner catch block

		std::cout << "Bob was not altered : " << bank[1] << std::endl;	// Still 300, the rejected creation changed nothing
	}													// Close the outer try block
	catch (const std::exception& e)						// Last resort: any exception nobody handled above
	{													// Open the outer catch block
		std::cerr << "Unexpected error : " << e.what() << std::endl;	// Report it on the error stream
		return (1);										// Exit with a non-zero status, the standard signals failure with non-zero
	}													// Close the outer catch block
	return (0);											// Reaching this line means everything went fine
}														// End of main
