/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hammer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hammer.hpp"

# include <iostream>		// std::cout, std::endl


// Hammer constructor, it chains up to the protected Tool constructor
Hammer::Hammer(void)
{
	std::cout << "[Hammer  ] ctor   a hammer is hanging on the board" << std::endl;
}

// Destructor, declared virtual here too, see Shovel::~Shovel()
Hammer::~Hammer()
{
	std::cout << "[Hammer  ] dtor   the hammer goes back in the box" << std::endl;
}

// The hammer at work, same shape as Shovel::use() but a different job
void	Hammer::use(void)
{
	_countUse();
	std::cout << "[Hammer  ] use    breaks a rock, use #" << getNumberOfUses() << std::endl;
}

// Identity as data, this is what the Workshop filter compares
std::string	Hammer::getToolName(void) const
{
	return ("hammer");
}
