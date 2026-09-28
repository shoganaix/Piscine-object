#ifndef VECTOR2_HPP								// Include guard: open
# define VECTOR2_HPP								// Define the guard token

# include <iosfwd>									// Forward declarations only: avoids the heavy <iostream>

// ---------------------------------------------------------------------------- //
// Vector2																		//
//																					//
// A two dimensional vector holding an x and a y component, both of type float.		//
//																					//
// ENCAPSULATION DECISION (the subject asks this question explicitly):				//
// The question is whether _x and _y should be private or public.					//
//																					//
// They are PRIVATE, for two different reasons depending on where the object lives:	//
//																					//
//  - A standalone Vector2 is just a pair of coordinates. It carries no invariant,	//
//    so it does get public setters. Mutating a point in isolation is meaningless,	//
//    but mutating one is harmless, because the object stays valid either way.		//
//																					//
//  - Once a Vector2 lives INSIDE a Graph, the situation changes completely. The		//
//    graph has a size, and only the points inside that size are legal. If the user	//
//    could write graph.getPoints()[3].setX(99), they would silently corrupt the		//
//    graph, and the render loop would then read outside the array. That is exactly	//
//    the class of bug encapsulation exists to prevent, so Graph never hands out a		//
//    mutable Vector2, only a const reference.										//
// ---------------------------------------------------------------------------- //

class Vector2											// A point or a size in a 2D plane
{														// Start of class body
public:													// ---- Accessible from the outside ----
	// --- Public interface -------------------------------------------------------- //

	Vector2();										// Default constructor, places the point at the origin (0, 0)
	Vector2(float p_x, float p_y);					// Constructor from an explicit position

	float	getX() const;							// Const getter for the x component
	float	getY() const;							// Const getter for the y component

	void	setX(float p_x);							// Setter for the x component
	void	setY(float p_y);							// Setter for the y component

	bool	operator==(const Vector2& p_other) const;	// Value equality, both components must match
	bool	operator!=(const Vector2& p_other) const;	// Negation of operator==

	Vector2	operator+(const Vector2& p_other) const;	// Component wise addition, used by the line bonus
	Vector2	operator-(const Vector2& p_other) const;	// Component wise subtraction, used by the line bonus

private:												// ---- Forbidden from the outside ----
	// --- Attributes -------------------------------------------------------------- //

	float	_x;										// The x component, private so nobody bypasses the setter
	float	_y;										// The y component, private so nobody bypasses the setter
};														// End of class body

std::ostream&	operator<<(std::ostream& p_os, const Vector2& p_vector);	// Stream insertion, prints "(x, y)"

#endif													// Include guard: close
