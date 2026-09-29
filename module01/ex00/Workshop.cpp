/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Workshop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 18:19:33 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Workshop.hpp"

# include <iostream>		// std::cout, std::endl
# include <stdexcept>		// std::invalid_argument


Workshop::Workshop(const std::string& p_name) :_name(p_name), _requiredTool(), _workers()
{
	std::cout << "[Workshop] ctor   \"" << _name << "\" opens, any worker is welcome" << std::endl;
}

// BONUS: A workshop that demands a tool
Workshop::Workshop(const std::string& p_name, const std::string& p_requiredTool)
	:_name(p_name), _requiredTool(p_requiredTool), _workers()
{
	// empty name -> throws
	if (_requiredTool.empty())
		throw (std::invalid_argument("Workshop: the required tool cannot be an empty name"));

	std::cout << "[Workshop] ctor   \"" << _name << "\" opens, only a worker with a " << _requiredTool << " may come in" << std::endl;
}

/* ! Note: the copy constructor and the copy assignment are DECLARED but NOT defined
// Workshop::Workshop(const Workshop& p_other) ;
// Workshop&	Workshop::operator=(const Workshop& p_other) ;
*/

// Closes the workshop but the workers are NOT deleted (not virtual)
Workshop::~Workshop()
{
	// references are cleared
	for (size_t i = 0; i < _workers.size(); i++)
	{
		std::cout << "[Workshop] dtor   \"" << _name << "\" closes while " << _workers[i]->getLabel() << " is still inside" << std::endl;
		_workers[i]->_forgetWorkshop(this);
	}
}

// by reference so the caller cannot rename a workshop
const std::string&	Workshop::getName(void) const
{
	return (_name);
}

const std::string&	Workshop::getRequiredTool(void) const
{
	return (_requiredTool);
}

size_t	Workshop::getNbWorkers(void) const
{
	return (_workers.size());
}

bool	Workshop::_findWorker(const Worker* p_worker, size_t& p_index) const
{
	for (size_t i = 0; i < _workers.size(); i++)
	{
		if (_workers[i] == p_worker)
		{
			p_index = i;		// caller needs position too!
			return (true);
		}
	}
	return (false);
}

bool	Workshop::hasWorker(const Worker* p_worker) const
{
	size_t	index;
	return (_findWorker(p_worker, index));
}


bool	Workshop::_hasRequiredTool(const Worker& p_worker) const
{
	// an open workshop takes anybody
	if (_requiredTool.empty())
		return (true);
	return (p_worker.hasTool(_requiredTool));
}


// Workers sign up
bool	Workshop::enrolWorker(Worker* p_worker)
{
	// null pointer -> throws
	if (p_worker == 0)
		throw (std::invalid_argument("Workshop: cannot enrol a null worker"));

	// twice in a row -> refused
	if (hasWorker(p_worker))
	{
		std::cout << "[Workshop] enrol  " << p_worker->getLabel() << " is already on the roster of \"" << _name << "\"" << std::endl;
		return (false);
	}

	// no tool -> refused
	if (!_hasRequiredTool(*p_worker))
	{
		std::cout << "[Workshop] enrol  " << p_worker->getLabel() << " is turned away, \"" << _name
					<< "\" only takes workers with a " << _requiredTool << std::endl;
		return (false);
	}

	// Both worker and workshops are appended here so they can never disagree
	_workers.push_back(p_worker);
	p_worker->_workshops.push_back(this);		// private, Workshop is a friend

	std::cout << "[Workshop] enrol  " << p_worker->getLabel() << " joins \"" << _name
				<< "\", " << getNbWorkers() << " worker(s) inside" << std::endl;
	return (true);
}


/* A worker signs out (only modifies worker part)
 * - Public on purpose since leaving is a legitimate move
 */
bool	Workshop::releaseWorker(Worker* p_worker)
{
	size_t	index;

	// not inside -> refuses the does nothing
	if (!_findWorker(p_worker, index))
	{
		std::cout << "[Workshop] release " << p_worker->getLabel() << " was not inside \"" << _name << "\", nothing to do" << std::endl;
		return (false);
	}

	_workers.erase(_workers.begin() + index);
	p_worker->_forgetWorkshop(this);			// private method on Worker that removes back reference

	std::cout << "[Workshop] release " << p_worker->getLabel() << " leaves \"" << _name
				<< "\", " << getNbWorkers() << " worker(s) left" << std::endl;
	return (true);
}


/* BONUS: called by a worker every time his toolbox changes
 * The edits are done DIRECTLY on both vectors, releaseWorker() does too
 */
void	Workshop::_checkWorker(Worker& p_worker)
{
	size_t	index;

	// not registered -> does nothing
	if (!_findWorker(&p_worker, index))
		return;

	// still carries tool -> stays (does nothing)
	if (_hasRequiredTool(p_worker))
		return;

	// tool is gone -> leaves on its own
	_workers.erase(_workers.begin() + index);
	p_worker._forgetWorkshop(this);

	std::cout << "[Workshop] check  " << p_worker.getLabel() << " no longer has a " << _requiredTool
				<< ", released from \"" << _name << "\"" << std::endl;
}


/* "launchs" day for every worker registered 
 * An empty workshop is not an error, a day with nobody in it is simply a day
 */
void	Workshop::executeWorkDay(void)
{
	std::cout << "[Workshop] day    \"" << _name << "\" opens its doors for " << getNbWorkers() << " worker(s)" << std::endl;

	// Index safer than iterator: work() because index always survives a change of size
	for (size_t i = 0; i < _workers.size(); i++)
		_workers[i]->work();

	std::cout << "[Workshop] day    \"" << _name << "\" closes, the work day is over" << std::endl;
}
