#ifndef GRAPH_HPP									// Include guard: open
# define GRAPH_HPP									// Define the guard token

# include <iosfwd>									// Forward declarations only: avoids the heavy <iostream>
# include <vector>									// std::vector, the standard container holding the points
# include "Vector2.hpp"								// The point type stored in the graph

// ---------------------------------------------------------------------------- //
// Graph																			//
//																					//
// A graph of a given size, holding a set of points inside it.							//
//																					//
// ENCAPSULATION DECISION (the subject asks this question explicitly):				//
// The question is whether the size and the point list should be private or public.	//
//																					//
// BOTH ARE PRIVATE, and this is the most important decision in the exercise.		//
//																					//
// If the point list were public, a user could push a point outside the graph:		//
//     graph.getPoints().push_back(Vector2(99, 99));									//
// The Graph would happily store it, and display() would then walk a grid that		// does not contain that point, so the point would either be invisible or the		// render loop would index out of range and crash. The object would no longer		//
// hold a valid state, and only the Graph itself is able to prevent that.			//
//																					//
// The size is private too. A user may READ it through a const getter, but must		//
// never be able to RESIZE a graph that already holds points, because the points		//
// stored would immediately become out of bounds.									//
// ---------------------------------------------------------------------------- //

class Graph												// A grid of a given size holding a set of points
{														// Start of class body
public:													// ---- Accessible from the outside ----
	// --- Public interface -------------------------------------------------------- //

	Graph();											// Default constructor, an empty graph of size (0, 0)
	explicit Graph(const Vector2& p_size);			// Constructor from a size, initially empty

	static Graph	fromFile(const char* p_path, const Vector2& p_size);	// Bonus: builds a graph from a text file of points

	const Vector2&				getSize() const;		// Const getter, returns a REFERENCE and never a copy
	const std::vector<Vector2>&	getPoints() const;		// Const getter, returns a REFERENCE and never a copy
	size_t						getNbPoints() const;	// Const getter for the number of points

	void	addPoint(const Vector2& p_point);			// Adds a point, throws if it falls outside the graph
	void	addLine(const Vector2& p_from, const Vector2& p_to);	// Bonus: adds every point of a segment

	void	display(std::ostream& p_os) const;			// Renders the graph as ASCII art, read-only

private:												// ---- Forbidden from the outside ----
	// --- Private interface ------------------------------------------------------- //

	bool	hasPoint(float p_x, float p_y) const;		// Membership test, used by the render loop
	void	checkInside(const Vector2& p_point) const;	// Throws std::out_of_range if the point is not in [0, size)

	// --- Attributes -------------------------------------------------------------- //

	Vector2					_size;					// The dimensions of the graph, private and never resizable once built
	std::vector<Vector2>	_points;					// The points, private: addPoint is the only way in
};														// End of class body

#endif													// Include guard: close
