/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Statistic.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATISTIC_HPP
# define STATISTIC_HPP

/* ! Note: same reasoning as Position, it is a plain STRUCTURE as the subject asks
 * -----------------------------------------------------------------------
 * A statistic is a value, not a behaviour. It is stored by value inside the
 * Worker, so a worker cannot lose his level and exp by accident, and cannot
 * share them with another worker.
 * -----------------------------------------------------------------------
 * WHY THE MEMBERS ARE NOT const
 * const would forbid the worker from ever leveling up. They are public and
 * writable on purpose: the point of this module is the RELATIONSHIP between
 * objects, not the protection of their fields, and the encapsulation that is
 * checked at the evaluation lives in Worker, Tool and Workshop.
 * -----------------------------------------------------------------------
 * The constructor still validates, so a level of -3 is rejected at the exact
 * moment it is typed instead of three workshops later.
*/

// ! COMPOSITION ingredient #2, stored by value next to the Position
struct Statistic
{
	int	level;
	int	exp;

	Statistic(int p_level, int p_exp);
};

#endif
