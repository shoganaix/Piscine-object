/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Position.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 16:17:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Position.hpp"

# include <iostream>		// std::cout, std::endl
# include <stdexcept>		// std::invalid_argument, used for error handling


/* The ONLY way to build a Position
 * - the three members are initialised first, then the body runs
 * - a negative coordinate -> throws
 */
Position::Position(int p_x, int p_y, int p_z) :x(p_x), y(p_y), z(p_z)
{
	if (p_x < 0 || p_y < 0 || p_z < 0)
		throw (std::invalid_argument("Position: coordinates cannot be negative"));

	std::cout << "[Position] ctor   a worker stands at (" << x << ", " << y << ", " << z << ")" << std::endl;
}
