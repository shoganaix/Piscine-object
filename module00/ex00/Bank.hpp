/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bank.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 19:47:26 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 19:47:26 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BANK_HPP
# define BANK_HPP

# include <iosfwd>									// avoids <iostream>
# include <map>										// std::map, needed for holding accounts

/* ! Note: there is deliberately no Account.hpp, bonus asks account type to be INTERNAL to bank
 * A nested class must be declared inside the body of its enclosing class
 * -------------------------------------------------
 * Bank owns a set of accounts keyed by ID
 *   ------ WHY std::map AND NOT std::vector? ------
 * I first stored accounts in a vector and made the ID = pos in the vector
 * That made operator[] easier but when deleting an account, this would renumber every account after it. 
 * A map keyed by the ID, keeps them stable
 *  ---------------- BONUS -------------------------
 * Bonus forbids for or while loops inside operator[]
 *  A map lookup is a balanced binary tree descent performed inside the standard library SO method body contains no loop at all
*/


class Bank
	{
	public:

		// Nested account type: account declared inside Bank, public as a TYPE (user may name it but its DATA stays private)
		class Account
		{
		public:

			int	getId() const;
			int	getValue() const;

		private:
			// Private constructor
			Account(int p_id, int p_value);				

			// ! Note: copy constructor & copy assignment operator are deliberately not declared

			// Making them private would break account because standard containers are not friends of account
			// Leaving them undeclared keeps container working and still exposes no way to mutate an account

			int	_id;
			int	_value;

			// Bank is the ONLY class allowed to mutate account
			friend class Bank;
		};


		explicit Bank(int p_liquidity);					// explicit forbids implicit int (no temporal obj)
		Bank(const Bank& p_other);						// Copy constructor -> DECLARED SO THAT A BANK CANNOT BE COPIED
		Bank& operator=(const Bank& p_other);			// Copy assignment -> DECLARED SO THAT A BANK CANNOT BE COPIED

														// ! Note: declaring these members private and never defining means a compile error

		int		operator[](int p_id) const;
		const Account&	getAccount(int p_id) const;
		int		getLiquidity() const;
		size_t	getNbAccounts() const;

		void	createAccount(int p_id, int p_value);
		void	deleteAccount(int p_id);
		void	deposit(int p_id, int p_amount);
		bool	giveLoan(int p_id, int p_amount);
		void	print(std::ostream& p_os) const;

	private:

		// because std::map::const_iterator cannot be reached unqualified from inside Bank: the member typedefs of the map are NOT inherited by the class that owns the map.
		typedef std::map<int, Account>::iterator		iterator;
		typedef std::map<int, Account>::const_iterator	const_iterator;

		iterator		findAccount(int p_id);
		const_iterator	findAccount(int p_id) const;
		void			checkAmount(int p_amount) const;

		int							_liquidity;			// banks money
		std::map<int, Account>		_accounts;			// accounts, keyed by id and stored by value (no raw pointer, no leak)
	};

std::ostream&	operator<<(std::ostream& p_os, const Bank::Account& p_account);		// stream insertion for account
std::ostream&	operator<<(std::ostream& p_os, const Bank& p_bank);					// stream insertion for bank

#endif
