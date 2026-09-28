/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Position.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSITION_HPP
# define POSITION_HPP

/* ! Note: the subject asks for a STRUCTURE, and it is written as one on purpose
 * -----------------------------------------------------------------------
 * A struct has public members by default, which is exactly what a bag of three
 * coordinates is. Turning it into a class would only add private members and
 * getters for data that has no behaviour attached to it.
 * -----------------------------------------------------------------------
 * WHY THERE IS NO DEFAULT CONSTRUCTOR
 * Leaving "int x;" alone would allow "Position p;" and reading three garbage
 * values. A single explicit constructor means a position is ALWAYS built from
 * three known values, and the constructor is the one single place where they
 * get checked.
 * -----------------------------------------------------------------------
 * The validation lives in the constructor and the constructor throws, because a
 * negative coordinate is not a position, it is a mistake made at the call site.
 * Catching it three workshops later would only make the report harder to read.
*/

// ! COMPOSITION ingredient #1: a worker IS built out of one of these, by value,
// ! so it cannot be NULL, cannot be swapped under his feet, and dies with him
struct Position
{
	int	x;
	int	y;
	int	z;

	Position(int p_x, int p_y, int p_z);
};

#endif
