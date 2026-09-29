/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 17:42:20 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Worker.hpp"
#include "Hammer.hpp"
#include "Workshop.hpp"

# include <iostream>		// std::cout, std::endl
# include <sstream>			// std::ostringstream, to build the label
# include <stdexcept>		// std::invalid_argument


// Members are built first
Worker::Worker(const Position& p_coordonnee, const Statistic& p_stat):_coordonnee(p_coordonnee), _stat(p_stat), _tools(), _workshops()
{
	std::cout << "[Worker  ] ctor   " << _label() << " is born, level " << _stat.level << ", " << _stat.exp << " exp" << std::endl;
}

// ! Note: the copy constructor and the copy assignment are DECLARED BUT NOT defined 

const Position&	Worker::getCoordonnee(void) const
{
	return (_coordonnee);
}

const Statistic&	Worker::getStat(void) const
{
	return (_stat);
}

// label keeps workers apart in the trace
std::string	Worker::getLabel(void) const
{
	return (_label());
}

// A worker is labelled by the place he stands
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
			return (true);		// found
	}
	return (false);				// not found
}


bool	Worker::isRegisteredTo(const Workshop* p_workshop) const
{
	for (size_t i = 0; i < _workshops.size(); i++)
	{
		if (_workshops[i] == p_workshop)
			return (true);
	}
	return (false);
}

size_t	Worker::getNbWorkshops(void) const
{
	return (_workshops.size());
}

void	Worker::giveTool(Tool* p_tool)
{
	// null pointer -> throws
	if (p_tool == 0)
		throw (std::invalid_argument("Worker: cannot be given a null tool"));

	// he already has it -> does nothing
	if (p_tool->getHolder() == this)
	{
		std::cout << "[Worker  ] give   " << _label() << " already holds the " << p_tool->getToolName() << std::endl;
		return;
	}

	/* ---- "Giving it to another worker removes it from the first" ----
	 * The tool knows who is holding it! Asking the previous holder to take it back is the ONLY thing
	 * that touches the previous toolbox and takeTool() erases the reference
	 */
	Worker*	p_previous = p_tool->getHolder();
	if (p_previous)
	{
		std::cout << "[Worker  ] give   the " << p_tool->getToolName() << " is taken away from " << p_previous->_label() << std::endl;
		p_previous->takeTool(p_tool);
	}

	_tools.push_back(p_tool);
	p_tool->_setHolder(this);
	std::cout << "[Worker  ] give   " << _label() << " holds " << getNbTools() << " tool(s)" << std::endl;

	_checkWorkshops();		// gaining a tool may change workshops
}


// The worker gives a tool back, returns false if not holding it
bool	Worker::takeTool(Tool* p_tool)
{
	for (size_t i = 0; i < _tools.size(); i++)
	{
		if (_tools[i] != p_tool)
			continue;			// not the tool
		_tools.erase(_tools.begin() + i);
		p_tool->_setHolder(0);	// tool on shelf
		std::cout << "[Worker  ] take   " << _label() << " put the " << p_tool->getToolName()
					<< " back, " << getNbTools() << " tool(s) left" << std::endl;
		_checkWorkshops();		// BONUS:losing a tool may change workshops
		return (true);
	}
	std::cout << "[Worker  ] take   " << _label() << " was not holding that tool, refused" << std::endl;
	return (false);
}

/* Bonus: Every workshop re-checks  after a toolbox change
 * !Note: BACKWARD iteration is mandatory here! (since you are checking workers and pos may change)
 */
void	Worker::_checkWorkshops(void)
{
	for (size_t i = _workshops.size(); i > 0; --i)
		_workshops[i - 1]->_checkWorker(*this);
}

// Private. Erases this worker from the workshop reference so we just want worker class touching it
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

/* "execute work IF he is registered to a workshop"
 * - otherwise he does a job for each of them
 */
bool	Worker::work(void)
{
	// registered nowhere -> refuses then does nothing
	if (_workshops.empty())
	{
		std::cout << "[Worker  ] work   " << _label() << " is signed up nowhere, he stays home" << std::endl;
		return (false);
	}
	// otherwise, does a job for each of them
	std::cout << "[Worker  ] work   " << _label() << " is heading to " << getNbWorkshops() << " workshop(s)" << std::endl;

	/* BONUS: Ask the toolbox for a hammer BY TYPE.
	 * Note to remember: getTool<Hammer>() returns 0 when he owns none.
	 */
	Hammer*	p_hammer = getTool<Hammer>();
	if (p_hammer == 0)
		std::cout << "[Worker  ] work   " << _label() << " has no hammer, he works with his bare hands" << std::endl;
	else
		p_hammer->use();
	return (true);
}
