/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tool.hpp"
#include "Worker.hpp"		// only needed here, to print who is holding the tool

# include <iostream>		// std::cout, std::endl


// Base constructor, only reachable from a derived class
Tool::Tool(void) :_numberOfUses(0), _holder(0)
{
	std::cout << "[Tool    ] ctor   a brand new tool, 0 use so far" << std::endl;
}

// VIRTUAL destructor
// Runs AFTER the derived destructor when a Tool* is deleted, which is why the
// trace always shows "Shovel dtor" first and "Tool dtor" last
Tool::~Tool()
{
	std::cout << "[Tool    ] dtor   a tool is put away, it was used " << _numberOfUses << " time(s)" << std::endl;
}

// The single writer of the counter, the derived use() already printed the action
void	Tool::_countUse(void)
{
	++_numberOfUses;
}

// Const counter getter
int	Tool::getNumberOfUses(void) const
{
	return (_numberOfUses);
}

// Who holds me right now, 0 when nobody does
Worker*	Tool::getHolder(void) const
{
	return (_holder);
}

// Private, only Worker may call it thanks to "friend class Worker"
void	Tool::_setHolder(Worker* p_holder)
{
	_holder = p_holder;
}
