/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shovel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 16:57:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shovel.hpp"

# include <iostream>		// std::cout, std::endl


// Shovel constructor (chains up to protected Tool constructor)
Shovel::Shovel(void)
{
	std::cout << "[Shovel  ] ctor   a shovel is leaning against the wall" << std::endl;
}

// Destructor (virtual) to silences -Wnon-virtual-dtor
Shovel::~Shovel()
{
	std::cout << "[Shovel  ] dtor   the shovel goes back to the shed" << std::endl;
}


/* _countUse() is PROTECTED, shovel may count its uses but can neither read nor write directly
 */
void	Shovel::use(void)
{
	_countUse();
	std::cout << "[Shovel  ] use    digs a hole, use #" << getNumberOfUses() << std::endl;
}

std::string	Shovel::getToolName(void) const
{
	return ("shovel");
}
