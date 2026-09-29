/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hammer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 18:31:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hammer.hpp"

# include <iostream>		// std::cout, std::endl


// Hammer constructor (chains up to protected Tool constructor)
Hammer::Hammer(void)
{
	std::cout << "[Hammer  ] ctor   a hammer is hanging on the board" << std::endl;
}

// Destructor (virtual)
Hammer::~Hammer()
{
	std::cout << "[Hammer  ] dtor   the hammer goes back in the box" << std::endl;
}

void	Hammer::use(void)
{
	_countUse();
	std::cout << "[Hammer  ] use    breaks a rock, use #" << getNumberOfUses() << std::endl;
}

std::string	Hammer::getToolName(void) const
{
	return ("hammer");
}
