/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Workshop.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKSHOP_HPP
# define WORKSHOP_HPP

# include "Worker.hpp"		// ! Note: an include and not a forward declaration, because a worker
								// ! is stored BY POINTER, and executeWorkDay() calls Worker::work(),
								// ! which needs the complete type

# include <string>				// std::string, the workshop name and the tool it demands
# include <vector>				// std::vector, the worker list


/* ==========================================================================================
 * Workshop is the ASSOCIATION of the subject, and it is deliberately the weakest of
 * the four relationships:
 *
 *   - neither object contains the other, they only point at each other
 *   - a worker can be inside SEVERAL workshops at the same time
 *   - a workshop can hold any number of workers, including none at all
 *   - either side can walk away at any moment, and the other side is told
 *
 * A workshop NEVER deletes a worker and a worker NEVER deletes a workshop. Whoever
 * created the object owns it, and in this exercise that is always main.
 *
 * WHY BOTH SIDES KEEP A LIST
 * The subject asks for two things that need both halves: the worker must know it is
 * registered somewhere before work() agrees to do anything, and the workshop needs
 * its own roster to run a work day. Two vectors, each private, each edited only by
 * its owner, kept in sync by the mutual friendship declared in Worker.
 * ==========================================================================================
 */
class Workshop
{
	public:

		// A workshop open to any worker
		Workshop(const std::string& p_name);

		/* BONUS 2, a workshop that only accepts workers holding a given tool
		* Written as a second constructor rather than as a default argument, so that
		* an empty tool name is a mistake the compiler asks about rather than a
		* silently open workshop.
		*/
		Workshop(const std::string& p_name, const std::string& p_requiredTool);

		// Declared and never defined, a workshop has an identity too
		Workshop(const Workshop& p_other);
		Workshop&	operator=(const Workshop& p_other);

		~Workshop();

		// Const getters, returned by reference so the caller cannot rename a workshop
		const std::string&	getName(void) const;
		const std::string&	getRequiredTool(void) const;
		size_t				getNbWorkers(void) const;
		bool				hasWorker(const Worker* p_worker) const;

		/* ASSOCIATION, both directions are public because both are legitimate:
		* a workshop may call a worker in, and a worker may call the workshop out.
		*/
		bool	enrolWorker(Worker* p_worker);
		bool	releaseWorker(Worker* p_worker);

		// Runs the day for every worker currently registered
		void	executeWorkDay(void);

	private:

		// ! Note: see the mutual friendship note in Worker.hpp, the reason is symmetric
		friend class Worker;

		// Shared linear search, returns false and leaves p_index untouched on a miss
		bool	_findWorker(const Worker* p_worker, size_t& p_index) const;

		/* BONUS 2, the filter
		* Compared on the tool NAME and not on a typeid, for two reasons:
		*   - a Workshop holding a Tool* prototype would have to invent a throwaway
		*     instance of an ABSTRACT class, which cannot be built at all
		*   - RTTI is already used exactly where it earns its keep, inside
		*     Worker::getTool<T>(), where a type check is what the caller asked for
		*/
		bool	_hasRequiredTool(const Worker& p_worker) const;

		/* BONUS 3, private on purpose
		* A worker must be able to ASK whether it still belongs here, but must not be
		* able to tell the workshop to drop it by accident. Only Worker::_checkWorkshops()
		* reaches this, and only to re-evaluate a membership it already holds.
		*/
		void	_checkWorker(Worker& p_worker);

		std::string				_name;
		std::string				_requiredTool;		// empty means "any worker is welcome"
		std::vector<Worker*>		_workers;			// non owning, a worker may be in several of those
};

#endif
