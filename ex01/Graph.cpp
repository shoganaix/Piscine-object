#include "Graph.hpp"									// Own class declaration, always the first include

# include <cmath>									// std::fabs, to compare floats without writing a tolerance
# include <fstream>									// std::ifstream, to read a file of points
# include <ostream>									// std::ostream definition, needed by the render method
# include <sstream>									// std::istringstream, to parse one line at a time
# include <stdexcept>								// std::out_of_range, used by the validation

// ---------------------------------------------------------------------------- //
// Graph - construction															//
// ---------------------------------------------------------------------------- //

// Default constructor. Builds an empty graph of size (0, 0). Nothing can be added
// to it, because every point would be out of bounds.
Graph::Graph() :										// Same name as the class: this is a definition, not a new function
	_size(Vector2(0.0f, 0.0f)),							// Initialisation list: the graph starts empty
	_points()											// Initialisation list: no point stored yet
{														// Empty body: nothing more to do at construction time
}														// End of Graph::Graph

// Constructor from a size. A size must describe a real grid, so it is validated here.
Graph::Graph(const Vector2& p_size) :					// Same name as the class: this is a definition, not a new function
	_size(p_size),										// Initialisation list: the graph takes the requested size
	_points()											// Initialisation list: the graph starts without any point
{														// Open the body
	if (_size.getX() < 0.0f || _size.getY() < 0.0f)		// A negative dimension is meaningless, a grid cannot have one
		throw (std::invalid_argument("Graph: size cannot be negative"));	// Refuse to build an impossible graph
}														// End of Graph::Graph

// ---------------------------------------------------------------------------- //
// Graph - bonus: building a graph out of a text file								//
// ---------------------------------------------------------------------------- //

// Expected file format, one point per line, with "x y" coordinates:
//     # this is a comment, the whole line is skipped
//     0 0
//     2 2
//     4 2
// Blank lines are skipped, and anything after a '#' is ignored.
Graph	Graph::fromFile(const char* p_path, const Vector2& p_size)	// Static factory, so it can build the object it returns
{														// Open the body
	std::ifstream file(p_path);						// Try to open the file for reading
	if (!file.is_open())								// The file does not exist, or we may not read it
		throw (std::invalid_argument("Graph: cannot open the points file"));	// Refuse loudly, never return a half built graph
	Graph graph(p_size);							// Build an empty graph of the requested size, validated by the constructor
	std::string line;									// The current line, reused for every iteration
	while (std::getline(file, line))					// Read the file one line at a time
	{													// Open the loop body
		const std::string::size_type comment = line.find('#');	// Look for a comment marker on this line
		if (comment != std::string::npos)			// There is one, everything after it is a comment
			line.erase(comment);						// Drop the comment, keep only the coordinates
		std::istringstream parser(line);				// Build a stream over the remaining characters
		int x = 0;										// The x coordinate, defaulted so the test below works on a blank line
		int y = 0;										// The y coordinate, defaulted for the same reason
		if (!(parser >> x >> y))						// Reading failed, so the line was empty or malformed
			continue;									// Skip it silently, a stray blank line is not worth crashing over
		graph.addPoint(Vector2(static_cast<float>(x), static_cast<float>(y)));	// addPoint validates the bounds for us
	}													// Close the loop body
	return (graph);									// Return the finished graph by value, the caller owns it
}														// End of Graph::fromFile

// ---------------------------------------------------------------------------- //
// Graph - const getters															//
// ---------------------------------------------------------------------------- //

// Const getter for the size. It returns a REFERENCE on purpose: returning the
// Vector2 by value would let the caller work on a detached copy, and returning a
// mutable reference would let the caller resize the graph behind our back.
const Vector2&	Graph::getSize() const					// const because it only reads the graph
{														// Open the body
	return (_size);									// const reference, no copy, read-only
}														// End of Graph::getSize

// Const getter for the point list. Same reasoning as getSize: a REFERENCE, and a
// const one, so the caller can inspect the points but can never modify them.
const std::vector<Vector2>&	Graph::getPoints() const	// const because it only reads the graph
{														// Open the body
	return (_points);									// const reference, no copy, read-only
}														// End of Graph::getPoints

// Const getter for the number of points.
size_t	Graph::getNbPoints() const						// const because it only reads the graph
{														// Open the body
	return (_points.size());							// size() already returns a size_t, nothing to cast
}														// End of Graph::getNbPoints

// ---------------------------------------------------------------------------- //
// Graph - private helpers														//
// ---------------------------------------------------------------------------- //

// Membership test, used by the render loop to decide between X and a dot.
bool	Graph::hasPoint(float p_x, float p_y) const		// const because it only reads the graph
{														// Open the body
	for (size_t i = 0; i < _points.size(); i++)			// size_t counter, it matches size() and avoids a signed/unsigned warning
	{													// Open the loop body
		// std::fabs turns the subtraction into an absolute value, so this compares
		// the two floats without needing an epsilon, which are integral values
		// here anyway, they come from a float grid.
		if (std::fabs(_points[i].getX() - p_x) < 0.0001f
			&& std::fabs(_points[i].getY() - p_y) < 0.0001f)	// Both components must match
			return (true);								// Found it, stop searching
	}													// Close the loop body
	return (false);										// Went through every point without a match
}														// End of Graph::hasPoint

// Throws unless p_point lies inside the grid, that is within [0, size) on both axes.
void	Graph::checkInside(const Vector2& p_point) const	// const because it only inspects the graph, it does not modify it
{														// Open the body
	if (p_point.getX() < 0.0f || p_point.getY() < 0.0f)	// Negative coordinates are outside every grid
		throw (std::out_of_range("Graph: point has negative coordinates"));	// Refuse the operation
	if (p_point.getX() >= _size.getX() || p_point.getY() >= _size.getY())	// The upper bound is EXCLUSIVE
		throw (std::out_of_range("Graph: point is outside the graph"));	// A graph of size 6 holds columns 0 to 5, never 6
}														// End of Graph::checkInside

// ---------------------------------------------------------------------------- //
// Graph - public interface														//
// ---------------------------------------------------------------------------- //

// Adds a point. This is the ONLY way to insert a point, and it validates first.
// Adding a point that is already there is a silent no-op, never an error and never a
// duplicate. A graph is a SET of points, not a list, and making addPoint idempotent is
// what lets two segments meet on a shared corner without storing that cell twice.
void	Graph::addPoint(const Vector2& p_point)			// Not const, because the method modifies the graph
{														// Open the body
	checkInside(p_point);								// Refuse a point that would corrupt the graph, throw before storing
	// The duplicate test makes addPoint O(n), which is the price of the set
	// semantics. For an ASCII grid n is at most a few thousand cells, so the cost
	// is irrelevant, whereas the render loop already does one lookup per cell.
	if (hasPoint(p_point.getX(), p_point.getY()))		// The point is already in the graph
		return;											// Do nothing, the graph already holds it
	_points.push_back(p_point);						// The point is new and inside the grid, so storing it is safe
}														// End of Graph::addPoint

// Bonus: adds every point of a segment from p_from to p_to. Reuses addPoint, so the
// bounds validation is applied to every single point of the line.
void	Graph::addLine(const Vector2& p_from, const Vector2& p_to)	// Not const, because the method modifies the graph
{														// Open the body
	// The segment must be axis aligned, otherwise it cannot be drawn on a grid made
	// of discrete cells. Rejecting anything else keeps the rule simple and honest.
	if (std::fabs(p_from.getX() - p_to.getX()) > 0.0001f
		&& std::fabs(p_from.getY() - p_to.getY()) > 0.0001f)	// The two endpoints differ on BOTH axes
		throw (std::invalid_argument("Graph: addLine expects an horizontal or vertical segment"));	// Refuse the diagonal
	checkInside(p_from);								// Validate the first endpoint
	checkInside(p_to);									// Validate the second endpoint
	const float dx = p_to.getX() - p_from.getX();		// Horizontal distance, positive, negative or zero
	const float dy = p_to.getY() - p_from.getY();		// Vertical distance
	const float steps = (std::fabs(dx) > std::fabs(dy) ? std::fabs(dx) : std::fabs(dy));	// Longest side, in cells
	for (float i = 0.0f; i <= steps; i += 1.0f)			// Walk every cell of the segment
	{													// Open the loop body
		// i / steps is the relative position along the segment, used to interpolate
		// the missing component. When steps is zero both endpoints are equal, and
		// the division below is guarded so we never divide by zero.
		const float ratio = (steps > 0.0f ? i / steps : 0.0f);	// Relative position, guarded against a zero length segment
		const float x = p_from.getX() + dx * ratio;		// Interpolated x, which stays constant on a vertical line
		const float y = p_from.getY() + dy * ratio;		// Interpolated y, which stays constant on an horizontal line
		addPoint(Vector2(x, y));							// addPoint validates again, so nothing out of bounds can slip in
	}													// Close the loop body
}														// End of Graph::addLine

// Renders the graph as ASCII art. Read-only, so it is a const method.
void	Graph::display(std::ostream& p_os) const			// const because it only reads the graph
{														// Open the body
	const int width = static_cast<int>(_size.getX());	// Graph width in cells, the size is a float but the grid is discrete
	const int height = static_cast<int>(_size.getY());	// Graph height in cells
														//
	// The expected layout, straight from the subject:									//
	//     >& 0 1 2 3 4 5																	//
	//     >& 0 X . . . . .																	//
	//     >& 2 . . X . X .																	//
	p_os << ">& ";										// The gutter every line starts with, as required by the subject
	for (int x = 0; x < width; x++)						// Walk every column index
	{													// Open the header loop
		if (x > 0)										// No separator before the very first index
			p_os << " ";								// One space between two column indices
		p_os << x;										// Print the column index
	}													// Close the header loop
	p_os << std::endl;									// End the header line
	for (int y = 0; y < height; y++)					// Walk every row, from the top down
	{													// Open the row loop
		p_os << ">& " << y;								// The gutter, then the row index
		for (int x = 0; x < width; x++)					// Walk every cell of that row
		{												// Open the cell loop
			// One X for a point, one dot for an empty cell. The ternary operator
			// keeps the two cases on a single readable line.
			p_os << " " << (hasPoint(static_cast<float>(x), static_cast<float>(y)) ? "X" : ".");	// Print the cell
		}												// Close the cell loop
		p_os << std::endl;								// End the row line
	}													// Close the row loop
}														// End of Graph::display
