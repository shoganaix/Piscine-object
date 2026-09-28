/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 19:47:38 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 19:54:08 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Vector2.hpp"

# include <ostream>									// std::ostream

Vector2::Vector2() :_x(0.0f),_y(0.0f) {														
}

Vector2::Vector2(float p_x, float p_y) :_x(p_x),_y(p_y){
}

// CONST getter for x
float	Vector2::getX() const
{
	return (_x);
}

// CONST getter for y
float	Vector2::getY() const
{
	return (_y);
}

// Setter for x
void	Vector2::setX(float p_x)
{
	_x = p_x;
}

// Setter for y
void	Vector2::setY(float p_y)
{
	_y = p_y;
}

// =: two points are equal when both components match
// Note: reading is legal, a class may always read private members of another instance of its own type
bool	Vector2::operator==(const Vector2& p_other) const
{

	return (_x == p_other._x && _y == p_other._y);
}

// !=
bool	Vector2::operator!=(const Vector2& p_other) const
{
	return (!(*this == p_other));
}

// +: used by line bonus to walk between two points
Vector2	Vector2::operator+(const Vector2& p_other) const
{
	// builds and returns a new vector
	return (Vector2(_x + p_other._x, _y + p_other._y));
}

// -: used by line bonus to measure a step
Vector2	Vector2::operator-(const Vector2& p_other) const
{
	return (Vector2(_x - p_other._x, _y - p_other._y));
}

// Prints the vector as "(x, y)"
std::ostream&	operator<<(std::ostream& p_os, const Vector2& p_vector)
{
	p_os << "(" << p_vector.getX() << ", " << p_vector.getY() << ")";
	return (p_os);
}
