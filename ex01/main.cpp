#include "Graph.hpp"									// The class under demonstration, always the first include
# include "Vector2.hpp"								// The point type, needed to build the graph

# include <exception>								// std::exception, the base of every standard exception
# include <iostream>									// std::cout, std::cerr and std::endl

// ---------------------------------------------------------------------------- //
// main - demonstration of the encapsulated graph									//
// ---------------------------------------------------------------------------- //

// Everything the graph can refuse to do is reported by an exception, so the whole
// demonstration runs inside a try block and the catch at the bottom turns any
// failure into a clean error message instead of a crash.
int	main()												// The program entry point, returns int as the standard requires
{														// Open the body
	try												// Every operation below may throw, so guard the whole thing
	{													// Open the try block

		std::cout << "=== The graph from the subject ===" << std::endl;	// Section header
		Graph graph(Vector2(6, 6));						// A 6 by 6 grid, that is columns 0 to 5 and rows 0 to 5
		graph.addPoint(Vector2(0, 0));					// Point 0/0, taken from the subject
		graph.addPoint(Vector2(2, 2));					// Point 2/2, taken from the subject
		graph.addPoint(Vector2(4, 2));					// Point 4/2, taken from the subject
		graph.addPoint(Vector2(2, 4));					// Point 2/4, taken from the subject
		graph.display(std::cout);						// Render it, this must match the subject byte for byte
		std::cout << "Points stored : " << graph.getNbPoints() << std::endl;	// Four points, read through a const getter

		std::cout << "Reading the graph back ===" << std::endl;	// Section header
		// getPoints returns a const REFERENCE, never a copy. Reading through it
		// is allowed, and every Vector2 it exposes is itself read-only.
		for (size_t i = 0; i < graph.getPoints().size(); i++)	// size_t counter, it matches size() and avoids a signed/unsigned warning
			std::cout << "point " << i << " is " << graph.getPoints()[i] << std::endl;	// Print through the const getter
		std::cout << "Size of the graph : " << graph.getSize() << std::endl;	// Printed through the const getter

		std::cout << "=== Refused points ===" << std::endl;	// Section header

		try											// The graph must never crash, so guard each misuse on its own
		{												// Open the inner try block
			graph.addPoint(Vector2(6, 0));				// Column 6 does not exist, the upper bound is exclusive
		}												// Close the inner try block
		catch (const std::exception& e)					// Catch by const reference, as the standard requires
		{												// Open the inner catch block
			std::cout << "Caught : " << e.what() << std::endl;	// what() returns the message carried by the exception
		}												// Close the inner catch block

		try											// Second misuse, same pattern
		{												// Open the inner try block
			graph.addPoint(Vector2(0, -1));				// A negative coordinate is outside every grid
		}												// Close the inner try block
		catch (const std::exception& e)					// Catch by const reference
		{												// Open the inner catch block
			std::cout << "Caught : " << e.what() << std::endl;	// Print the message
		}												// Close the inner catch block

		std::cout << "Still " << graph.getNbPoints() << " points" << std::endl;	// Still four, nothing was corrupted

		std::cout << "=== Bonus, a line ===" << std::endl;	// Section header
		Graph lined(Vector2(8, 4));					// A wider and lower grid, to show a line
		lined.addPoint(Vector2(0, 1));					// One anchor point, so the line reads as a segment
		// A segment must be horizontal or vertical, because a grid is made of
		// discrete cells and a diagonal has no meaningful cells to fill. Two calls
		// therefore draw a corner: first right, then up.
		lined.addLine(Vector2(1, 1), Vector2(6, 1));		// Horizontal segment, from column 1 to column 6 on row 1
		lined.addLine(Vector2(6, 1), Vector2(6, 3));		// Vertical segment, from row 1 to row 3 on column 6
		lined.display(std::cout);						// Render it
		std::cout << "Points after the line : " << lined.getNbPoints() << std::endl;	// 1 anchor + 6 cells + 3 cells

		std::cout << "=== Bonus, reading a file ===" << std::endl;	// Section header
		// The file is a plain text list of "x y" pairs, '#' starts a comment and
		// blank lines are skipped. Every point still goes through addPoint, so the
		// bounds validation applies exactly as if the points had been typed in.
		Graph loaded = Graph::fromFile("points.txt", Vector2(10, 6));	// Build the graph straight from the file
		loaded.display(std::cout);						// Render it
		std::cout << "Points loaded : " << loaded.getNbPoints() << std::endl;	// How many the file contained
	}													// Close the outer try block
	catch (const std::exception& e)						// Last resort: any exception nobody handled above
	{													// Open the outer catch block
		std::cerr << "Unexpected error : " << e.what() << std::endl;	// Report it on the error stream
		return (1);										// Exit with a non-zero status, the standard signals failure with non-zero
	}													// Close the outer catch block
	return (0);											// Reaching this line means everything went fine
}														// End of main
