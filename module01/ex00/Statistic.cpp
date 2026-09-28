/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statistic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Statistic.hpp"

# include <iostream>		// std::cout, std::endl
# include <stdexcept>		// std::invalid_argument, used for error handling


/* The only way to build a Statistic
 * - a negative level or experience -> throws
 */
Statistic::Statistic(int p_level, int p_exp) :level(p_level), exp(p_exp)
{
	if (p_level < 0 || p_exp < 0)
		throw (std::invalid_argument("Statistic: level and exp cannot be negative"));

	std::cout << "[Statistic] ctor  born at level " << level << " with " << exp << " exp" << std::endl;
}
