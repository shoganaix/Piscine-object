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
	// if ID doesnt exist -> abort
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

/* Throws unless p_amount is strictly positive.
* const because it only inspects the value, it does not touch the bank
*/
void	Bank::checkAmount(int p_amount) const
{
	if (p_amount <= 0)								// Zero or negative money is never a meaningful operation
		throw (std::invalid_argument("Bank: amount must be strictly positive"));	// Abort loudly, the caller sent garbage
}														// End of Bank::checkAmount

// ---------------------------------------------------------------------------- //
// Bank - public interface														//
// ---------------------------------------------------------------------------- //

// Constructor from the initial liquidity.
Bank::Bank(int p_liquidity) :						// Same name as the class: this is a definition, not a new function
	_liquidity(p_liquidity),							// Initialisation list: the bank starts with the given amount of money
	_accounts()										// Initialisation list: the map of accounts starts empty
{														// Empty body: nothing more to do at construction time
}														// End of Bank::Bank

// Copy constructor, declared but never defined. Copying a bank would duplicate its
// account ids, which the subject forbids, so the compiler must refuse to copy.
Bank::Bank(const Bank& /*p_other*/)					// The parameter is left unnamed to silence -Wunused-parameter
{														// Empty body: the function is never linkable, so it is never reached
}														// End of Bank::Bank

// Copy assignment, declared but never defined. Same reasoning as the copy constructor.
Bank&	Bank::operator=(const Bank& /*p_other*/)		// The parameter is left unnamed to silence -Wunused-parameter
{														// Empty body: the function is never linkable, so it is never reached
	return (*this);									// Required by the compiler, never actually executed
}														// End of Bank::operator=

// Returns a const REFERENCE to the account, never a copy. Returning a copy would let
// the caller work on a detached duplicate; a const reference cannot be mutated at all.
const Bank::Account&	Bank::getAccount(int p_id) const	// const because it only reads the bank
{														// Open the body
	// Calling findAccount here resolves to the const overload, because this
	// method is const. The result points to a const Account, which is exactly
	// what we want: the caller receives a read-only reference, never a copy.
	return (findAccount(p_id)->second);				// Return a const reference: no copy, and the caller cannot mutate it
}														// End of Bank::getAccount

// Bonus: account lookup by id, with no loop written by hand.
int	Bank::operator[](int p_id) const					// const because it only reads the bank
{														// Open the body
	return (getAccount(p_id).getValue());			// Delegate to getAccount, then to the account const getter
}														// End of Bank::operator[]

// Const getter for the bank liquidity.
int	Bank::getLiquidity() const						// const because it only reads the bank
{														// Open the body
	return (_liquidity);								// Return by value, safe for an int
}														// End of Bank::getLiquidity

// Const getter for the number of accounts.
size_t	Bank::getNbAccounts() const					// const because it only reads the bank
{														// Open the body
	return (_accounts.size());						// size() already returns a size_t, nothing to cast
}														// End of Bank::getNbAccounts

// Creates an account with the given id and opening balance.
void	Bank::createAccount(int p_id, int p_value)		// Not const: this method genuinely mutates the bank
{														// Open the body
	if (p_value < 0)									// A negative opening balance is illogical, reject it at the boundary
		throw (std::invalid_argument("Bank: opening balance cannot be negative"));	// Abort loudly
	if (p_id < 0)										// A negative id can never be valid, the bank only handles real identifiers
		throw (std::invalid_argument("Bank: account id cannot be negative"));	// Abort loudly
	if (_accounts.find(p_id) != _accounts.end())		// The client must never be able to open two accounts with the same id
		throw (std::invalid_argument("Bank: this account id is already taken"));	// Abort loudly, uniqueness is guaranteed here
	// insert() returns a pair where .second tells whether the insertion happened.
	// Because the id was proven free just above, the insertion always succeeds.
	_accounts.insert(std::make_pair(p_id, Account(p_id, p_value)));	// Store the account BY VALUE, keyed by its id
}														// End of Bank::createAccount

// Deletes an account. The ids of every other account stay untouched.
void	Bank::deleteAccount(int p_id)					// Not const: this method genuinely mutates the bank
{														// Open the body
	findAccount(p_id);								// Throws std::out_of_range if there is no such account
	_accounts.erase(p_id);							// Erase by key, the tree rebalances itself, no reindexing needed
}														// End of Bank::deleteAccount

// The ONLY way to add money to a client account. The bank keeps a 5% commission.
void	Bank::deposit(int p_id, int p_amount)			// Not const: this method genuinely mutates the bank
{														// Open the body
	checkAmount(p_amount);							// Reject zero and negative amounts
	iterator it = findAccount(p_id);					// Resolve the account once, this method mutates it
	const int commission = p_amount * 5 / 100;		// 5% with integer arithmetic, no floating point and no rounding surprise
	// std::map::operator[] CANNOT be used here: it would force Account to be
	// default constructible, which would mean giving it a public default
	// constructor. Going through the iterator avoids that entirely.
	it->second._value += p_amount - commission;		// The client receives the deposit minus the commission
	_liquidity += commission;						// The bank keeps the commission, this is the bank's income
}														// End of Bank::deposit

// Grants a loan to an account, but only if the bank owns enough money.
bool	Bank::giveLoan(int p_id, int p_amount)			// Not const: this method genuinely mutates the bank
{														// Open the body
	checkAmount(p_amount);							// Reject zero and negative amounts
	iterator it = findAccount(p_id);					// Resolve the account once; throws before any money moves
	if (p_amount > _liquidity)						// The bank would go bankrupt, refuse the operation
		return (false);								// Report the refusal by return value, this is an expected outcome not an error
	_liquidity -= p_amount;							// The money leaves the bank
	it->second._value += p_amount;					// And lands in the client account, a loan carries no commission
	return (true);									// Report the success by return value
}														// End of Bank::giveLoan

// Dumps the whole bank on the given stream.
void	Bank::print(std::ostream& p_os) const			// const because it only reads the bank
{														// Open the body
	p_os << "Bank informations :" << std::endl;		// Header, the same wording as the original subject code
	p_os << "Liquidity : " << _liquidity << std::endl;// Print the bank liquidity
	// const_iterator, because the method is const and std::map::iterator is not
	// convertible to std::map::const_iterator in that context.
	for (const_iterator it = _accounts.begin(); it != _accounts.end(); ++it)	// Walk the map in ascending id order
		p_os << it->second << std::endl;				// Delegate the account formatting to the account stream operator
}														// End of Bank::print

// ---------------------------------------------------------------------------- //
// Stream operators																	//
// ---------------------------------------------------------------------------- //

// Formats one account as "[id] - [value]", the exact format of the original subject code.
std::ostream&	operator<<(std::ostream& p_os, const Bank::Account& p_account)		// Free function, not a member, so no friend declaration is needed
{														// Open the body
	p_os << "[" << p_account.getId() << "]";			// Use the const getters, never touch the private members
	p_os << " - [" << p_account.getValue() << "]";	// Same access rule on the balance
	return (p_os);									// Returning the stream allows chaining, as the standard requires
}														// End of operator<< for Bank::Account

// Formats a whole bank, used by std::cout << bank.
std::ostream&	operator<<(std::ostream& p_os, const Bank& p_bank)					// Free function, takes the bank by const reference
{														// Open the body
	p_bank.print(p_os);								// Reuse the print method, no duplicated formatting logic
	return (p_os);									// Returning the stream allows chaining, as the standard requires
}														// End of operator<< for Bank
