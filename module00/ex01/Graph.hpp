/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graph.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 21:41:48 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 21:41:49 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_HPP
# define GRAPH_HPP

# include <iosfwd>									// avoids <iostream>
# include <vector>									// std::vector
# include "Vector2.hpp"

/* A graph holds a set of points inside it
 * ----------- ENCAPSULATION DECISION ----------
 * Both (size and points) are private. 
 * - If point list were public, user could push a point outside the graph (graph.getPoints()[0].setX(999)
 * - If size was public, user must never be able to resize graph
 */



class Graph
	{
	public:

		Graph();
		explicit Graph(const Vector2& p_size);

		static Graph	fromFile(const char* p_path, const Vector2& p_size);	// Bonus: builds a graph from an input file

		const Vector2&				getSize() const;		// CONST + returns a REFERENCE not a copy
		const std::vector<Vector2>&	getPoints() const;		// ...
		size_t						getNbPoints() const;

		void	addPoint(const Vector2& p_point);						// Adds a point
		void	addLine(const Vector2& p_from, const Vector2& p_to);	// Bonus: adds a line (points on segments)

		void	display(std::ostream& p_os) const;

	private:

		bool	hasPoint(float p_x, float p_y) const;
		void	checkInside(const Vector2& p_point) const;


		Vector2					_size;						// dimensions of the graph are private and never resizable once built (no setter)
		std::vector<Vector2>	_points;					// points are private so addPoint is the only way in
	};

#endif
