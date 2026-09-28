/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shovel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shovel.hpp"

# include <iostream>		// std::cout, std::endl


// Shovel constructor, it chains up to the protected Tool constructor
Shovel::Shovel(void)
{
	std::cout << "[Shovel  ] ctor   a shovel is leaning against the wall" << std::endl;
}

// Destructor, declared virtual in Shovel too even though Tool already is:
// it silences -Wnon-virtual-dtor and it is the derived part that must run
Shovel::~Shovel()
{
	std::cout << "[Shovel  ] dtor   the shovel goes back to the shed" << std::endl;
}

/* The shovel at work
 * _countUse() is PROTECTED in Tool, so the shovel may count its own uses but can
 * neither read the counter without the getter nor write it directly
 */
void	Shovel::use(void)
{
	_countUse();
	std::cout << "[Shovel  ] use    digs a hole, use #" << getNumberOfUses() << std::endl;
}

// Identity as data, this is what the Workshop filter compares
std::string	Shovel::getToolName(void) const
{
	return ("shovel");
}
