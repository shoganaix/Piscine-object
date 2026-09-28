/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Workshop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Workshop.hpp"

# include <iostream>		// std::cout, std::endl
# include <stdexcept>		// std::invalid_argument, used for error handling


// An open workshop, any worker may come in
Workshop::Workshop(const std::string& p_name) :_name(p_name), _requiredTool(), _workers()
{
	std::cout << "[Workshop] ctor   \"" << _name << "\" opens, any worker is welcome" << std::endl;
}

// BONUS 2, a workshop that demands a tool before letting anybody in
Workshop::Workshop(const std::string& p_name, const std::string& p_requiredTool)
	:_name(p_name), _requiredTool(p_requiredTool), _workers()
{
	// empty name -> throws, it would silently build an open workshop
	if (_requiredTool.empty())
		throw (std::invalid_argument("Workshop: the required tool cannot be an empty name"));

	std::cout << "[Workshop] ctor   \"" << _name << "\" opens, only a worker with a " << _requiredTool << " may come in" << std::endl;
}

/* ! Note: the copy constructor and the copy assignment are DECLARED in the header
* and deliberately NOT defined here, for the same reason as in Worker: cloning a
* workshop would duplicate a roster that the workers know nothing about, and the
* copies would immediately drift apart.
*/
// Workshop::Workshop(const Workshop& p_other) ;
// Workshop&	Workshop::operator=(const Workshop& p_other) ;


// Closes the workshop, the workers are NOT deleted, they are not its to destroy
Workshop::~Workshop()
{
	// The back references are cleared, otherwise a worker would keep pointing at a
	// closed workshop and work() would send him there forever
	for (size_t i = 0; i < _workers.size(); i++)
	{
		std::cout << "[Workshop] dtor   \"" << _name << "\" closes while " << _workers[i]->getLabel() << " is still inside" << std::endl;
		_workers[i]->_forgetWorkshop(this);
	}
}

// Const name getter, by reference so the caller cannot rename a workshop
const std::string&	Workshop::getName(void) const
{
	return (_name);
}

// Const required tool getter
const std::string&	Workshop::getRequiredTool(void) const
{
	return (_requiredTool);
}

// Number of workers CONST getter
size_t	Workshop::getNbWorkers(void) const
{
	return (_workers.size());
}

// Linear search helper, shared by hasWorker, releaseWorker and _checkWorker
bool	Workshop::_findWorker(const Worker* p_worker, size_t& p_index) const
{
	for (size_t i = 0; i < _workers.size(); i++)
	{
		if (_workers[i] == p_worker)
		{
			p_index = i;		// the caller needs the position, not only a yes or no
			return (true);
		}
	}
	return (false);				// p_index is left untouched, the caller must not use it
}

// Const membership test
bool	Workshop::hasWorker(const Worker* p_worker) const
{
	size_t	index;				// the position is of no interest here
	return (_findWorker(p_worker, index));
}


// BONUS 2, the filter that guards the door
bool	Workshop::_hasRequiredTool(const Worker& p_worker) const
{
	// an open workshop takes anybody
	if (_requiredTool.empty())
		return (true);
	return (p_worker.hasTool(_requiredTool));
}


/* ASSOCIATION, a worker signs up
 * - a null worker is a programming mistake -> throws
 * - already inside -> refused, a worker is never listed twice
 * - missing the required tool -> refused, that is BONUS 2
 */
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

	// no tool, no entry -> refused
	if (!_hasRequiredTool(*p_worker))
	{
		std::cout << "[Workshop] enrol  " << p_worker->getLabel() << " is turned away, \"" << _name
					<< "\" only takes workers with a " << _requiredTool << std::endl;
		return (false);
	}

	_workers.push_back(p_worker);
	// The back reference, this is what makes Worker::work() know where to go.
	// Both lists are appended here, in the same breath, so they can never disagree.
	p_worker->_workshops.push_back(this);		// private on Worker, Workshop is a friend

	std::cout << "[Workshop] enrol  " << p_worker->getLabel() << " joins \"" << _name
				<< "\", " << getNbWorkers() << " worker(s) inside" << std::endl;
	return (true);
}


/* ASSOCIATION, a worker signs out, on his own or because the workshop dropped him
 * Public on purpose: leaving is a legitimate move for both sides.
 */
bool	Workshop::releaseWorker(Worker* p_worker)
{
	size_t	index;

	// not inside -> nothing to do, releasing an absent worker leaves the state it was in
	if (!_findWorker(p_worker, index))
	{
		std::cout << "[Workshop] release " << p_worker->getLabel() << " was not inside \"" << _name << "\", nothing to do" << std::endl;
		return (false);
	}

	_workers.erase(_workers.begin() + index);
	p_worker->_forgetWorkshop(this);			// private on Worker, so a back reference is never left behind

	std::cout << "[Workshop] release " << p_worker->getLabel() << " leaves \"" << _name
				<< "\", " << getNbWorkers() << " worker(s) left" << std::endl;
	return (true);
}


/* BONUS 3, called by a worker every time his toolbox changes
 *
 * The edits are done DIRECTLY on both vectors instead of going through
 * releaseWorker(): the worker is halfway through Worker::_checkWorkshops() and is
 * iterating his own _workshops vector right now, so the removal has to be the
 * surgical one. releaseWorker() would do exactly the same two erasures, and it
 * would erase the element the caller is standing on.
 */
void	Workshop::_checkWorker(Worker& p_worker)
{
	size_t	index;

	// not registered here -> there is nothing to decide
	if (!_findWorker(&p_worker, index))
		return;

	// still carries what we asked for -> stays
	if (_hasRequiredTool(p_worker))
		return;

	// the tool is gone -> he leaves on his own, nobody had to ask
	_workers.erase(_workers.begin() + index);
	p_worker._forgetWorkshop(this);

	// ! Note: no pronoun in that message, the trace labels the workers by their
	// ! position and never says whether they are he or she
	std::cout << "[Workshop] check  " << p_worker.getLabel() << " no longer has a " << _requiredTool
				<< ", released from \"" << _name << "\"" << std::endl;
}


/* "launching the day for every worker registered inside it"
 * An empty workshop is not an error, a day with nobody in it is simply a day
 * where nothing happens.
 */
void	Workshop::executeWorkDay(void)
{
	std::cout << "[Workshop] day    \"" << _name << "\" opens its doors for " << getNbWorkers() << " worker(s)" << std::endl;

	// Index based and not an iterator: work() reaches back into the workshops,
	// and an index survives a change of size where an iterator would not
	for (size_t i = 0; i < _workers.size(); i++)
		_workers[i]->work();

	std::cout << "[Workshop] day    \"" << _name << "\" closes, the work day is over" << std::endl;
}
