#include "Vector2.hpp"								// Own class declaration, always the first include

# include <ostream>									// std::ostream definition, needed by the stream operator

// ---------------------------------------------------------------------------- //
// Vector2 - construction															//
// ---------------------------------------------------------------------------- //

// Default constructor. Places the point at the origin, which is the only value that
// makes sense for a vector nobody has positioned yet.
Vector2::Vector2() :									// Same name as the class: this is a definition, not a new function
	_x(0.0f),											// Initialisation list: the x component starts at zero
	_y(0.0f)											// Initialisation list: the y component starts at zero
{														// Empty body: nothing more to do at construction time
}														// End of Vector2::Vector2

// Constructor from an explicit position.
Vector2::Vector2(float p_x, float p_y) :				// Same name as the class: this is a definition, not a new function
	_x(p_x),											// Initialisation list: the x component takes the parameter value
	_y(p_y)											// Initialisation list: the y component takes the parameter value
{														// Empty body: nothing more to do at construction time
}														// End of Vector2::Vector2

// ---------------------------------------------------------------------------- //
// Vector2 - getters and setters													//
// ---------------------------------------------------------------------------- //

// Const getter for the x component. The result is a plain float, which cannot be
// used to reach the attribute back, so returning it by value is safe.
float	Vector2::getX() const							// Trailing const applies to the method, not to the returned float
{														// Open the body
	return (_x);										// Return by value
}														// End of Vector2::getX

// Const getter for the y component.
float	Vector2::getY() const							// Trailing const applies to the method, not to the returned float
{														// Open the body
	return (_y);										// Return by value
}														// End of Vector2::getY

// Setter for the x component. This is the only public way to reach the attribute.
void	Vector2::setX(float p_x)						// Not const, because the method modifies the object
{														// Open the body
	_x = p_x;											// Assign through the private attribute, only this method may
}														// End of Vector2::setX

// Setter for the y component.
void	Vector2::setY(float p_y)						// Not const, because the method modifies the object
{														// Open the body
	_y = p_y;											// Assign through the private attribute, only this method may
}														// End of Vector2::setY

// ---------------------------------------------------------------------------- //
// Vector2 - operators																//
// ---------------------------------------------------------------------------- //

// Value equality: two points are equal when both components match.
bool	Vector2::operator==(const Vector2& p_other) const	// const because it only reads both operands
{														// Open the body
	// Reading p_other._x directly is legal here: a class may always read the
	// private members of another instance of its own type.
	return (_x == p_other._x && _y == p_other._y);	// Both comparisons must succeed
}														// End of Vector2::operator==

// Negation of operator==, provided so the caller never has to write !(a == b).
bool	Vector2::operator!=(const Vector2& p_other) const	// const because it only reads both operands
{														// Open the body
	return (!(*this == p_other));						// Delegate to operator==, single source of truth
}														// End of Vector2::operator!=

// Component wise addition, used by the line bonus to walk between two points.
Vector2	Vector2::operator+(const Vector2& p_other) const	// const because it only reads both operands
{														// Open the body
	return (Vector2(_x + p_other._x, _y + p_other._y));	// Build and return a brand new vector
}														// End of Vector2::operator+

// Component wise subtraction, used by the line bonus to measure a step.
Vector2	Vector2::operator-(const Vector2& p_other) const	// const because it only reads both operands
{														// Open the body
	return (Vector2(_x - p_other._x, _y - p_other._y));	// Build and return a brand new vector
}														// End of Vector2::operator-

// ---------------------------------------------------------------------------- //
// Stream operator																	//
// ---------------------------------------------------------------------------- //

// Prints the vector as "(x, y)", for example "(2, 4)".
std::ostream&	operator<<(std::ostream& p_os, const Vector2& p_vector)	// Free function, not a member
{														// Open the body
	p_os << "(" << p_vector.getX() << ", " << p_vector.getY() << ")";	// Use the const getters
	return (p_os);									// Returning the stream allows chaining, as the standard requires
}														// End of operator<< for Vector2
