/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 19:47:19 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 20:12:27 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Graph.hpp"
# include "Vector2.hpp"

# include <exception>									// std::exception
# include <iostream>									// std::cout, std::cerr, std::endl


// // Everything the graph can refuse to do is reported by an exception (try-catch block)
int	main()
{
	try
	{

		std::cout << "=== The graph from the subject ===" << std::endl;
		Graph graph(Vector2(6, 6));						// A 6 by 6 grid, that is columns 0 to 5 and rows 0 to 5
		graph.addPoint(Vector2(0, 0));					// Point 0/0
		graph.addPoint(Vector2(2, 2));					// Point 2/2
		graph.addPoint(Vector2(4, 2));					// Point 4/2
		graph.addPoint(Vector2(2, 4));					// Point 2/4
		graph.display(std::cout);						// Render
		std::cout << "Points stored : " << graph.getNbPoints() << std::endl;	// 4

		std::cout << "Reading the graph back ===" << std::endl;
		// - 'getPoints' -> returns a const REFERENCE, reading through itsallowed
		// - 'size_t counter' -> matches size() + avoids signed/unsigned warnings
		for (size_t i = 0; i < graph.getPoints().size(); i++)
			std::cout << "point " << i << " is " << graph.getPoints()[i] << std::endl;
		std::cout << "Size of the graph : " << graph.getSize() << std::endl;

		std::cout << "=== Refused points ===" << std::endl;

		// column 6 does not exist
		try
		{
			graph.addPoint(Vector2(6, 0));
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		// 0 > coordinate
		try
		{
			graph.addPoint(Vector2(0, -1));
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught : " << e.what() << std::endl;
		}

		std::cout << "Still " << graph.getNbPoints() << " points" << std::endl;	// 4

		std::cout << "=== Bonus, a line ===" << std::endl;
		Graph lined(Vector2(8, 4));							// A 8 by 4 grid
		lined.addPoint(Vector2(0, 1));						// 1 anchor

		lined.addLine(Vector2(1, 1), Vector2(6, 1));		// Horizontal segment, column 1-6, row 1
		lined.addLine(Vector2(6, 1), Vector2(6, 3));		// Vertical segment, row 1-3, column 6
		lined.display(std::cout);							// Render
		std::cout << "Points after the line : " << lined.getNbPoints() << std::endl;	// 1 anchor + 6 cells + 3 cells

		std::cout << "=== Bonus, reading a file ===" << std::endl;
		// builds graphs from file
		Graph loaded = Graph::fromFile("points.txt", Vector2(10, 6));
		loaded.display(std::cout);
		std::cout << "Points loaded : " << loaded.getNbPoints() << std::endl;
	}
	// ANY exception not handled above									
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected error : " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
