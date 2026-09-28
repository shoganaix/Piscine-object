/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graph.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 21:41:32 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 21:46:26 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Graph.hpp"

# include <cmath>									// std::fabs (compares floats without writing a tolerance)
# include <limits>									// std::numeric_limits
# include <fstream>									// std::ifstream (read files)
# include <ostream>									// std::ostream
# include <sstream>									// std::istringstream
# include <stdexcept>								// std::out_of_range


Graph::Graph() :_size(Vector2(0.0f, 0.0f)), _points() {
}

Graph::Graph(const Vector2& p_size) :_size(p_size), _points()
{
	// < 0, throws
	if (_size.getX() < 0.0f || _size.getY() < 0.0f)
		throw (std::invalid_argument("Graph: size cannot be negative"));
	// > max int, throws
	if (p_size.getX() > static_cast<float>(std::numeric_limits<int>::max())
		|| p_size.getY() > static_cast<float>(std::numeric_limits<int>::max()))
		throw (std::out_of_range("Graph: size is too large"));
}


// Expected file format:
//	   One point per line, with "X Y" coordinates
//     # means there is a comment, the whole line will be skipped
// 		Blank lines are skipped, and anything after a '#' is ignored.
//     0 0
//     2 2
//     4 2

Graph	Graph::fromFile(const char* p_path, const Vector2& p_size)	// Static factory, so it can build the object it returns
{
	std::ifstream file(p_path);
	if (!file.is_open())
		throw (std::invalid_argument("Graph: cannot open the points file"));
	Graph graph(p_size);
	std::string line;
	while (std::getline(file, line))
	{
		const std::string::size_type comment = line.find('#');
		if (comment != std::string::npos)
			line.erase(comment);
		std::istringstream parser(line);
		int x = 0;
		int y = 0;
		if (!(parser >> x >> y))
			continue;
		graph.addPoint(Vector2(static_cast<float>(x), static_cast<float>(y)));	// addPoint validates the bounds for us
	}
	return (graph);
}


// size CONST getter 
// returns a REFERENCE on purpose: returning the
// Vector2 by value would let the caller work on a detached copy, and returning a
// mutable reference would let the caller resize the graph behind our back.
const Vector2&	Graph::getSize() const
{
	return (_size);
}

// points CONST getter
const std::vector<Vector2>&	Graph::getPoints() const
{
	return (_points);
}

// number of points CONST getter
size_t	Graph::getNbPoints() const
{
	return (_points.size());
}


// Membership test, used by render loop to decide between X and a dot.
bool	Graph::hasPoint(float p_x, float p_y) const
{
	for (size_t i = 0; i < _points.size(); i++)
	{
		// std::fabs turns the subtraction into an absolute value, so this compares
		// the two floats without needing an epsilon, which are integral values
		// here anyway, they come from a float grid.
		if (std::fabs(_points[i].getX() - p_x) < 0.0001f
			&& std::fabs(_points[i].getY() - p_y) < 0.0001f)	// Both components must match
			return (true);								// Found it, stop searching
	}													// Close the loop body
	return (false);										// Went through every point without a match
}

// p_point !lies inside the grid or !is within [0, size) on both axes -> throws
void	Graph::checkInside(const Vector2& p_point) const
{
	/* '-std=c++98' does not support std::isnan or std::isinf, so these checks are disabled
	* // NaN -> throws
	* if (std::isnan(p_point.getX()) || std::isnan(p_point.getY()))
	*	throw (std::out_of_range("Graph: point coordinates cannot be NaN"));
	* // infinite -> throws
	* if (std::isinf(p_point.getX()) || std::isinf(p_point.getY()))
	*	throw (std::out_of_range("Graph: point coordinates cannot be infinite"));
	*/
	// < 0 points -> throws
	if (p_point.getX() < 0.0f || p_point.getY() < 0.0f)
		throw (std::out_of_range("Graph: point has negative coordinates"));
	// A graph of size 6 holds columns 0 to 5, never 6
	if (p_point.getX() >= _size.getX() || p_point.getY() >= _size.getY())
		throw (std::out_of_range("Graph: point is outside the graph"));
}


// This is the ONLY way to insert a point
// Adding a point that is already there is a silent no-op (never error-never duplicate)
void	Graph::addPoint(const Vector2& p_point)
{
	checkInside(p_point);								// refuse invalid

	// The duplicate test makes addPoint O(n) (slower -> time increases linearly according to n) 
	// For an ASCII grid n is at most a few thousand cells, so cost is irrelevant

	if (hasPoint(p_point.getX(), p_point.getY()))		// The point is already in the graph
		return;											// do nothing
	_points.push_back(p_point);							// The point is new and inside the grid, so storing it is safe
}

// Bonus: Adds every point of a segment from p_from to p_to. Reuses addPoint

void	Graph::addLine(const Vector2& p_from, const Vector2& p_to)
{
	checkInside(p_from);								// Validate first endpoint
	checkInside(p_to);									// Validate second endpoint
	// The segment must be axis aligned (no diagonals)
	// If two points are different in both X and Y at the same time (using std::fabs with the 0.0001f tolerace) -> line is diagonal
	if (std::fabs(p_from.getX() - p_to.getX()) > 0.0001f && std::fabs(p_from.getY() - p_to.getY()) > 0.0001f)
		throw (std::invalid_argument("Graph: addLine expects an horizontal or vertical segment"));
	const float dx = p_to.getX() - p_from.getX();		// Horizontal distance
	const float dy = p_to.getY() - p_from.getY();		// Vertical distance
	const float steps = (std::fabs(dx) > std::fabs(dy) ? std::fabs(dx) : std::fabs(dy));	// ternary operator, returns longer distance (x or y) between points
	
	/* Walks every cell of the segment
	* 
	* EXAMPLE: Drawing a line 4 cells long (steps = 4):
	* - i = 0 -> ratio = 0/4 = 0.0  (0%   -> Start point)
	* - i = 2 -> ratio = 2/4 = 0.5  (50%  -> Mid point)
	* - i = 4 -> ratio = 4/4 = 1.0  (100% -> End point)
	* 
	*/

	const size_t intSteps = static_cast<size_t>(steps);
	for (size_t i = 0; i <= intSteps; ++i)
	{
		// i is the position along the segment
		// if steps = 0 -> both endpoints are equal (division is guarded so we never divide by zero)
		const float ratio = (steps > 0.0f ? static_cast<float>(i) / steps : 0.0f);	// 'ratio' is the percentage of path completed (from 0.0 to 1.0), guarded
		const float x = p_from.getX() + dx * ratio;									// Interpolated x
		const float y = p_from.getY() + dy * ratio;									// Interpolated y
		addPoint(Vector2(x, y));													// addPoint validates
	}
}

// Renders graph as ASCII art
void	Graph::display(std::ostream& p_os) const
{
	const int width = static_cast<int>(_size.getX());	// Graph width in cells
	const int height = static_cast<int>(_size.getY());	// Graph height in cells

	p_os << ">& ";										// every line starts with '>&'
	for (int x = 0; x < width; x++)						// walk every column
	{
		if (x > 0)										// No separator before the very first index
			p_os << " ";
		p_os << x;										// Print column index
	}
	p_os << std::endl;
	for (int y = 0; y < height; y++)					// walk every row
	{
		p_os << ">& " << y;
		for (int x = 0; x < width; x++)					// walk every cell
		{
			// One X for a point, one dot for an empty cell
			p_os << " " << (hasPoint(static_cast<float>(x), static_cast<float>(y)) ? "X" : ".");
		}
		p_os << std::endl;
	}
}
