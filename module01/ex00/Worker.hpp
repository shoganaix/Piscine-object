/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_HPP
# define WORKER_HPP

# include "Position.hpp"		// COMPOSITION, stored BY VALUE
# include "Statistic.hpp"		// COMPOSITION, stored BY VALUE
# include "Tool.hpp"			// INHERITANCE + AGGREGATION, the worker only sees the base class

# include <string>				// std::string, tool names and the label used in the trace
# include <vector>				// std::vector, the toolbox and the workshop list


class Workshop;					// ! Note: forward declaration on purpose, a worker only stores POINTERS to workshops


/* ==========================================================================================
 * Worker is the class where the FOUR relationships of the subject meet:
 *
 *   COMPOSITION   _coordonnee / _stat   stored BY VALUE. A worker cannot exist without them,
 *                                      cannot be built without them, and they die with him.
 *
 *   AGGREGATION   _tools               stored BY POINTER, and the Worker destructor does NOT
 *                                      delete them. The tool outlives the worker on purpose.
 *
 *   INHERITANCE   the worker never sees Shovel or Hammer, only Tool. The polymorphism lives
 *                 in the tools, not here.
 *
 *   ASSOCIATION   _workshops           stored BY POINTER in both directions, a worker may be
 *                                      inside several workshops and may leave any time.
 *
 * WHY THE TOOLS AND THE WORKSHOPS ARE NOT const AND NOT COPIED
 * Both lists change during the life of a worker, so they are mutable containers of
 * non owning pointers. "Non owning" is the load bearing word: the worker never deletes
 * a tool, and a workshop never deletes a worker, so whoever created the object stays
 * responsible for it. There is exactly one owner per object in this exercise and it is
 * always main.
 * ==========================================================================================
 */
class Worker
{
	public:

		// The only constructor: no worker can be built without a position and a statistic
		Worker(const Position& p_coordonnee, const Statistic& p_stat);

		/* ! Note: copy constructor & copy assignment are DECLARED AND NEVER DEFINED
		* A worker has an identity. Cloning him would produce a second man with the same
		* coordinates, the same tools and the same workshop memberships, and nothing in
		* this exercise ever needs that, so the door is simply closed.
		* The two members are const, which already forbids any assignment.
		*/
		Worker(const Worker& p_other);
		Worker&	operator=(const Worker& p_other);

		/* The destructor is the proof of the aggregation
		* It releases the workshop memberships and clears the tool back references,
		* but it NEVER deletes a tool. A shovel that a dead worker was holding is still
		* sitting on the shelf afterwards, and the trace says so.
		*/
		~Worker();

		// Const getters, returned by CONST REFERENCE on purpose: returning a copy would
		// let the caller work on a detached position that is not the worker's
		const Position&	getCoordonnee(void) const;
		const Statistic&	getStat(void) const;

		// The label used in the trace, it keeps the workers apart without inventing a name
		std::string		getLabel(void) const;

		/* ------------------------------- AGGREGATION ------------------------------- */

		// The worker BORROWS a tool. The previous holder, if any, loses it first
		void			giveTool(Tool* p_tool);

		// The worker gives a tool back, returns false if he was not holding it
		bool			takeTool(Tool* p_tool);

		size_t			getNbTools(void) const;
		bool			hasTool(const std::string& p_toolName) const;

		/* ---------------------------- BONUS: typed lookup ----------------------------
		* "get the first tool of the right type, or nullptr otherwise"
		* dynamic_cast walks the vtable of every tool in the toolbox and returns 0 when
		* the type does not match, which is precisely the required behaviour, including
		* the case of a tool that is of a DERIVED type of the requested one.
		*
		* WHY THE BODY LIVES IN THE HEADER
		* A template is compiled where it is declared, and the compiler has no idea
		* what ToolType is until somebody calls getTool<Hammer>(). Putting the body in
		* the .cpp would make the linker fail the first time the bonus is used.
		*/
		template <typename ToolType>
	ToolType*		getTool(void)
		{
			// ! Note: no "typename" keyword, std::vector<Tool*>::iterator does not
			// ! depend on ToolType, and C++98 only allows it on dependent names
			for (std::vector<Tool*>::iterator it = _tools.begin(); it != _tools.end(); ++it)
			{
				// ! Note: tested and NOT dereferenced blindly, a miss returns 0
				ToolType*	p_typed = dynamic_cast<ToolType*>(*it);
				if (p_typed)
					return (p_typed);		// first match wins, that is the "first tool of the right type"
			}
			return (0);						// the toolbox holds no such tool
		}

		/* ------------------------------ ASSOCIATION ------------------------------ */

		bool			isRegisteredTo(const Workshop* p_workshop) const;
		size_t			getNbWorkshops(void) const;

		// Does the work IF the worker is registered to at least one workshop
		bool			work(void);

	private:

		/* ! Note: the two classes are friends of each other, and that is deliberate
		* ---------------------------------------------------------------------
		* Worker::_workshops and Workshop::_workers are two halves of the SAME
		* relationship, and the subject requires both sides to stay consistent:
		*   - a worker leaves a workshop   -> the worker erases its own back reference
		*   - a workshop drops a worker    -> the workshop erases its own back reference
		* Neither side may edit the other list with a public method, so each grants
		* the other friendship and the two edit their OWN vector. The lists can never
		* drift apart, and neither class is left with a public setter.
		*/
		friend class Workshop;

		// Private, reachable from Workshop, erases this worker from p_workshop's list
		void				_forgetWorkshop(Workshop* p_workshop);

		/* BONUS 3, called every time the toolbox changes
		* Asking every registered workshop to re-check this worker is what turns
		* "he lost his tool" into "he left the workshop that needed it", without the
		* worker having to know which workshops those are.
		*/
		void				_checkWorkshops(void);

		// A worker is labelled by his position, no extra member is needed for that
		std::string			_label(void) const;

		const Position				_coordonnee;	// COMPOSITION, by value, and const: a worker never changes place
		const Statistic			_stat;			// COMPOSITION, by value, and const: a worker never loses his level
		std::vector<Tool*>			_tools;		// AGGREGATION, non owning
		std::vector<Workshop*>		_workshops;		// ASSOCIATION, non owning
};

#endif
