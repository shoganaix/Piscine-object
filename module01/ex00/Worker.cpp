/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Worker.hpp"
#include "Hammer.hpp"		// only needed here, so work() can reach for a hammer by type
#include "Workshop.hpp"	// a worker talks to the workshops it belongs to

# include <iostream>		// std::cout, std::endl
# include <sstream>			// std::ostringstream, to build the label
# include <stdexcept>		// std::invalid_argument, used for error handling


// COMPOSITION in action: the two members are built FIRST, then this body runs
Worker::Worker(const Position& p_coordonnee, const Statistic& p_stat)
	:_coordonnee(p_coordonnee), _stat(p_stat), _tools(), _workshops()
{
	std::cout << "[Worker  ] ctor   " << _label() << " is born, level " << _stat.level << ", " << _stat.exp << " exp" << std::endl;
}

/* ! Note: the copy constructor and the copy assignment are DECLARED in the header
* and deliberately NOT defined here.
* Declaring them without a body is the usual way to forbid an operation: the code
* still compiles, and the mistake only shows up as a link error naming the exact
* function that was wrongly used, which is far easier to diagnose than a silently
* wrong deep copy of a worker holding tools and workshop memberships.
*/
// Worker::Worker(const Worker& p_other) ;
// Worker&	Worker::operator=(const Worker& p_other) ;


// Const position getter, by reference so the caller cannot detach it
const Position&	Worker::getCoordonnee(void) const
{
	return (_coordonnee);
}

// Const statistic getter, by reference, same reason
const Statistic&	Worker::getStat(void) const
{
	return (_stat);
}

// The label that keeps the workers apart in the trace
std::string	Worker::getLabel(void) const
{
	return (_label());
}

// A worker is labelled by the place he stands, so no extra member is needed for it
std::string	Worker::_label(void) const
{
	std::ostringstream	oss;
	oss << "worker(" << _coordonnee.x << ", " << _coordonnee.y << ", " << _coordonnee.z << ")";
	return (oss.str());
}


/* ==========================================================================================
 * Destructor, the most commented method of the exercise
 * ==========================================================================================
 * Two lists of non owning pointers have to be cleaned up, and they are cleaned in
 * opposite ways.
 */
Worker::~Worker()
{
	std::cout << "[Worker  ] dtor   " << _label() << " is leaving" << std::endl;

	/* ---- 1. ASSOCIATION, the workshops must forget him ----
	 * A workshop holds a raw pointer to its workers, so without this the workshop
	 * would be left pointing at freed memory and its next executeWorkDay() would
	 * follow a dead pointer.
	 *
	 * BACKWARD iteration is not a style choice, it is a requirement: releaseWorker()
	 * erases this worker from the very _workshops vector being walked, and erasing
	 * the element a forward iterator is standing on is undefined behaviour.
	 */
	for (size_t i = _workshops.size(); i > 0; --i)
		_workshops[i - 1]->releaseWorker(this);

	/* ---- 2. AGGREGATION, the tools must NOT be deleted ----
	 * Nothing is deleted. That single absence is what separates aggregation from
	 * composition: the shovel belongs to whoever created it, so a worker may vanish
	 * while still holding one, and the tool stays perfectly usable.
	 *
	 * The back reference IS cleared though, otherwise the tool would keep pointing
	 * at a destroyed worker and the next giveTool() would ask a dead man to give
	 * the tool back.
	 */
	for (size_t i = 0; i < _tools.size(); i++)
	{
		std::cout << "[Worker  ] dtor   " << _label() << " leaves the " << _tools[i]->getToolName()
					<< " behind, it was never his to destroy" << std::endl;
		_tools[i]->_setHolder(0);
	}
}


// Number of tools CONST getter
size_t	Worker::getNbTools(void) const
{
	return (_tools.size());
}

// Name based lookup, this is what the Workshop filter calls
bool	Worker::hasTool(const std::string& p_toolName) const
{
	for (size_t i = 0; i < _tools.size(); i++)
	{
		if (_tools[i]->getToolName() == p_toolName)
			return (true);		// found it
	}
	return (false);				// walked the whole toolbox
}


// Is this worker inside that workshop
bool	Worker::isRegisteredTo(const Workshop* p_workshop) const
{
	for (size_t i = 0; i < _workshops.size(); i++)
	{
		if (_workshops[i] == p_workshop)
			return (true);
	}
	return (false);
}

// Number of workshops this worker belongs to, CONST getter
size_t	Worker::getNbWorkshops(void) const
{
	return (_workshops.size());
}


/* ------------------------------- AGGREGATION ------------------------------- */

/* The worker BORROWS a tool
 * - a null tool is a programming mistake, not a runtime condition -> throws
 * - handing over the tool he already holds is a no-op
 * - handing it to somebody else TAKES IT AWAY from the previous holder
 */
void	Worker::giveTool(Tool* p_tool)
{
	// null pointer -> throws
	if (p_tool == 0)
		throw (std::invalid_argument("Worker: cannot be given a null tool"));

	// he already has it -> nothing to do, and above all do not push it twice
	if (p_tool->getHolder() == this)
	{
		std::cout << "[Worker  ] give   " << _label() << " already holds the " << p_tool->getToolName() << std::endl;
		return;
	}

	/* ---- "giving it to another worker removes it from the first" ----
	 * The tool knows who is holding it, so the hand over is resolved without any
	 * global registry of workers, which would have been the obvious but ugly
	 * alternative. Asking the previous holder to take it back is the ONLY thing
	 * that touches the previous toolbox, and takeTool() already erases the back
	 * reference, so both sides stay consistent.
	 */
	Worker*	p_previous = p_tool->getHolder();
	if (p_previous)
	{
		std::cout << "[Worker  ] give   the " << p_tool->getToolName() << " is taken away from " << p_previous->_label() << std::endl;
		p_previous->takeTool(p_tool);		// inside it, the victim is re-checked by his workshops
	}

	_tools.push_back(p_tool);
	p_tool->_setHolder(this);				// private on Tool, Worker is a friend
	std::cout << "[Worker  ] give   " << _label() << " holds " << getNbTools() << " tool(s)" << std::endl;

	_checkWorkshops();		// gaining a tool may also change what a workshop accepts
}


// The worker gives a tool back, returns false if he was not holding it
bool	Worker::takeTool(Tool* p_tool)
{
	for (size_t i = 0; i < _tools.size(); i++)
	{
		if (_tools[i] != p_tool)
			continue;			// not the one
		_tools.erase(_tools.begin() + i);
		p_tool->_setHolder(0);	// the tool is back on the shelf, nobody holds it any more
		std::cout << "[Worker  ] take   " << _label() << " put the " << p_tool->getToolName()
					<< " back, " << getNbTools() << " tool(s) left" << std::endl;
		_checkWorkshops();		// BONUS 3, losing a tool may cost him a workshop
		return (true);
	}
	std::cout << "[Worker  ] take   " << _label() << " was not holding that tool, refused" << std::endl;
	return (false);				// not an error, the state was simply already the requested one
}


/* ---------------------------- BONUS 3 ---------------------------- */

/* Every workshop this worker belongs to re-checks him after a toolbox change
 *
 * BACKWARD iteration is mandatory here, for the same reason as in the destructor:
 * a workshop that decides to release him calls _forgetWorkshop(), which erases
 * from the very vector being walked.
 */
void	Worker::_checkWorkshops(void)
{
	for (size_t i = _workshops.size(); i > 0; --i)
		_workshops[i - 1]->_checkWorker(*this);
}

// Private, only Workshop may call it, erases this worker from the workshop back reference
void	Worker::_forgetWorkshop(Workshop* p_workshop)
{
	for (std::vector<Workshop*>::iterator it = _workshops.begin(); it != _workshops.end(); ++it)
	{
		if (*it == p_workshop)
		{
			_workshops.erase(it);
			return;
		}
	}
}


/* ------------------------------ ASSOCIATION ------------------------------ */

/* "execute work IF he is registered to a workshop"
 * - no workshop -> refuses, returns false
 * - otherwise he does a job for each of them
 */
bool	Worker::work(void)
{
	// registered nowhere -> nothing to do
	if (_workshops.empty())
	{
		std::cout << "[Worker  ] work   " << _label() << " is signed up nowhere, he stays home" << std::endl;
		return (false);
	}

	std::cout << "[Worker  ] work   " << _label() << " is heading to " << getNbWorkshops() << " workshop(s)" << std::endl;

	/* The BONUS template in real use: ask the toolbox for a hammer BY TYPE.
	 * getTool<Hammer>() returns 0 when he owns none, which is exactly why the
	 * result is tested and not dereferenced straight away.
	 *
	 * The call below is a virtual dispatch through a Tool* the worker never
	 * declared, which is the whole point of the inheritance done in Tool.
	 */
	Hammer*	p_hammer = getTool<Hammer>();
	if (p_hammer == 0)
		std::cout << "[Worker  ] work   " << _label() << " has no hammer, he works with his bare hands" << std::endl;
	else
		p_hammer->use();

	return (true);
}
