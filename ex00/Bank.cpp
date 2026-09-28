/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bank.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 18:30:16 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 19:12:15 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bank.hpp"

# include <ostream>		// stream operators (<<)
# include <stdexcept>	// std::out_of_range + std::invalid_argument, used for  error handling


// Private constructor of nested class, reachable only through friend class Bank
Bank::Account::Account(int p_id, int p_value) : _id(p_id),_value(p_value) {}

// Public const ID getter 
int	Bank::Account::getId() const
{
	return (_id);
}

// Public const balance getter
int	Bank::Account::getValue() const
{
	return (_value);
}

// Throws unless p_id addresses an existing account, and returns a mutable iterator.
Bank::iterator	Bank::findAccount(int p_id)
{
	iterator it = _accounts.find(p_id);
	// if ID doesnt exist -> throws
	if (it == _accounts.end())
		throw (std::out_of_range("Bank: unknown account id"));
	// Hand back pos it, caller will decide what to do with it
	return (it);
}

// Const version (read-only)
Bank::const_iterator	Bank::findAccount(int p_id) const
{
	const_iterator it = _accounts.find(p_id);
	if (it == _accounts.end())
		throw (std::out_of_range("Bank: unknown account id"));
	return (it);
}

/* p_amount != positive -> throws
* const needed to only inspect value, not touch the bank
*/
void	Bank::checkAmount(int p_amount) const
{
	// <= 0 money -> throws
	if (p_amount <= 0)
		throw (std::invalid_argument("Bank: amount must be strictly positive"));
}

// Constructor from initial liquidity
Bank::Bank(int p_liquidity) :_liquidity(p_liquidity),_accounts()
{
}

// Copy constructor
// (*p_other*) left unnamed to silence "-Wunused-parameter"
Bank::Bank(const Bank& /*p_other*/)
{
}

// Copy assignment
Bank&	Bank::operator=(const Bank& /*p_other*/)
{
	return (*this);	
}

// Returns const REFERENCE to account, NEVER a copy (returning a copy would let the caller work on a duplicate)
const Bank::Account&	Bank::getAccount(int p_id) const
{
	// findAccount points to a CONST account
	return (findAccount(p_id)->second);	
}

// BONUS: account lookup by id
int	Bank::operator[](int p_id) const
{
	return (getAccount(p_id).getValue());
}

// Const bank liquidity getter
int	Bank::getLiquidity() const
{
	return (_liquidity);
}

// Const number of accounts getter
size_t	Bank::getNbAccounts() const
{
	return (_accounts.size());
}

// Creates account with given ID and opening balance - NON const
void	Bank::createAccount(int p_id, int p_value)
{
	// <= 0 money -> throws
	if (p_value < 0)
		throw (std::invalid_argument("Bank: opening balance cannot be negative"));
	// <= 0 ID -> throws
	if (p_id < 0)
		throw (std::invalid_argument("Bank: account id cannot be negative"));
	// duplicated IDs -> throws
	if (_accounts.find(p_id) != _accounts.end())
		throw (std::invalid_argument("Bank: this account id is already taken"));
	/* insert() returns a pair
	* - second arg tells whether the insertion happened (true/false)
	* - ID was proven free just above so insertion always succeeds
	*/ 
	_accounts.insert(std::make_pair(p_id, Account(p_id, p_value)));
}

/* Deletes account (IDs stay untouched) - NON const
 *  - throws if account not found
 * 	- erases by key then tree rebalances itself w/no reindexing needed
*/
void	Bank::deleteAccount(int p_id)
{
	findAccount(p_id);
	_accounts.erase(p_id);							
}

/* ONLY way to add money to a client account (5% commission) - NON const
 * 	! Note: std::map::operator[] CANNOT be used because it would force account to be public default
*/
void	Bank::deposit(int p_id, int p_amount)
{
	checkAmount(p_amount);
	// Resolve the account once, this method mutates it
	iterator it = findAccount(p_id);
	const int commission = p_amount * 5 / 100;
	
	// clients
	it->second._value += p_amount - commission;
	// banks
	_liquidity += commission;
}

// Grants loan to account, but only if bank owns enough money - NON const
bool	Bank::giveLoan(int p_id, int p_amount)
{
	checkAmount(p_amount);
	iterator it = findAccount(p_id);
	// banks bankrupt -> doesnt loan
	if (p_amount > _liquidity)
		return (false);
	// banks
	_liquidity -= p_amount;
	// clients
	it->second._value += p_amount;
	return (true);
}

// Dumps the whole bank on the given stream
void	Bank::print(std::ostream& p_os) const
{
	p_os << "Bank informations :" << std::endl;
	p_os << "Liquidity : " << _liquidity << std::endl;

	/* Loop that walks map in ascending ID order
	 * 'const_iterator' because method is const & std::map::iterator not convertible to std::map::const_iterator
	*/
	for (const_iterator it = _accounts.begin(); it != _accounts.end(); ++it)
		// ! Account formatting to stream operator
		p_os << it->second << std::endl;
}

/* Formats an account as "[id] - [value]"
 * this is a free function, not a member -> no friend declaration needed
 *
 * Note: Remember to use const getters NEVER touch private members
*/
std::ostream&	operator<<(std::ostream& p_os, const Bank::Account& p_account)		
{	
	p_os << "[" << p_account.getId() << "]";
	p_os << " - [" << p_account.getValue() << "]";
	return (p_os);
}

// Formats a whole bank
std::ostream&	operator<<(std::ostream& p_os, const Bank& p_bank)
{
	// reuses prints logic
	p_bank.print(p_os);
	return (p_os);
}
