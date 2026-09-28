/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector2.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 19:47:34 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 19:47:35 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR2_HPP
# define VECTOR2_HPP

# include <iosfwd>	// avoids the heavy <iostream>


/* Vector 2 is a two dimensional vector holding an x and a y component, both of type float
 *
 *------------- ENCAPSULATION DECISION -------
 * 'x' & 'y' are private, why?
 * A standalone vector is harmless, a vextor INSIDE a Graph is not
 * The graph has a size, and only the points inside that size are legal
*/


class Vector2
{
public:


	Vector2();											// places point at origin (0, 0)
	Vector2(float p_x, float p_y);

	float	getX() const;								// Const getter x
	float	getY() const;								// Const getter y

	void	setX(float p_x);							// Setter for x
	void	setY(float p_y);							// Setter for y

	bool	operator==(const Vector2& p_other) const;	// =
	bool	operator!=(const Vector2& p_other) const;	// !=

	Vector2	operator+(const Vector2& p_other) const;	// +
	Vector2	operator-(const Vector2& p_other) const;	// -

private:

	// private attributes so nobody bypasses the setters
	float	_x;
	float	_y;	
};

std::ostream&	operator<<(std::ostream& p_os, const Vector2& p_vector);	// stream insertion -> prints "(x, y)"

#endif
