/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 18:51:49 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 19:03:39 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bank.hpp"

# include <exception>								// std::exception,
# include <iostream>								// std::cout, std::cerr, std::endl
# include <stdexcept>								// std::out_of_range, std::invalid_argument


// Everything the bank can refuse to do is reported by an exception (try-catch block)
int	main()
{
	try												// Every operation below may throw, so guard the whole thing
	{

		std::cout << "=== Creation of the bank ===" << std::endl;
		// bank starts with 999
		Bank bank(999);	
		// bank has no account
		std::cout << bank << std::endl;


		std::cout << "=== Account creation ===" << std::endl;
	
		// Opens account number 0 with balance of 100
		bank.createAccount(0, 100);
		// Opens account number 1 with a balance of 100
		bank.createAccount(1, 100);	
		std::cout << bank << std::endl;

		std::cout << "=== Deposit, the bank keeps 5% ===" << std::endl;
		// account number 0 deposits 400, the bank skims 20 (total 380)
		bank.deposit(0, 400);
		std::cout << "Alice now owns : " << bank[0] << std::endl;				// 100 + 380 = 480
		std::cout << "Bank liquidity : " << bank.getLiquidity() << std::endl;	// 999 + 20 = 1019
		std::cout << "Alice id is still : " << bank.getAccount(0).getId() << std::endl;

		std::cout << "=== Loans ===" << std::endl;
		// account number 1 borrows 200, bank CAN afford it
		const bool granted = bank.giveLoan(1, 200);
		std::cout << "Loan of 200 granted : " << granted << std::endl;				// true
		std::cout << "Bob now owns : " << bank[1] << std::endl;						// 100 + 200 = 300
		// account number 1 borrows 100000, bank CANT afford it
		const bool refused = bank.giveLoan(1, 100000);
		std::cout << "Loan of 100000 granted : " << refused << std::endl;			// false
		std::cout << "Liquidity untouched : " << bank.getLiquidity() << std::endl;	// 819

		std::cout << "=== Account deletion ===" << std::endl;
		bank.deleteAccount(0);
		// No reindexing!
		std::cout << "Accounts left : " << bank.getNbAccounts() << std::endl;					// 1 account left
		std::cout << "Bob id after the deletion : " << bank.getAccount(1).getId() << std::endl;	// 1
		std::cout << "Bob now owns : " << bank[1] << std::endl;									// 300
		std::cout << bank << std::endl;

		std::cout << "=== Refused operations ===" << std::endl;	


		// Asking for an account number that was never opened
		try
		{
			bank[42];
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}


		// Negative deposit
		try
		{
			bank.deposit(0, -50);
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		// Opening an account number twice
		try											 
		{
			bank.createAccount(1, 500);
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		std::cout << "Bob was not altered : " << bank[1] << std::endl;							// 300, nothing changed
	}				
	// ANY exception no handled above									
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected error : " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
