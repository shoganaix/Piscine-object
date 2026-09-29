/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Workshop.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 18:54:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKSHOP_HPP
# define WORKSHOP_HPP

# include "Worker.hpp"		

# include <string>				// std::string
# include <vector>				// std::vector (worker list)

class Workshop
{
	public:

		Workshop(const std::string& p_name);

		/* BONUS:, a workshop that only accepts workers holding a given tool
		* Written as a second constructor rather than on a default argument, so that
		* an empty tool name is a mistake the compiler can ask about 
		*/
		Workshop(const std::string& p_name, const std::string& p_requiredTool);

		// Declared and never defined
		Workshop(const Workshop& p_other);
		Workshop&	operator=(const Workshop& p_other);

		~Workshop();

		// Const getters, returned AGAIN by reference so the caller cannot rename a workshop
		const std::string&	getName(void) const;
		const std::string&	getRequiredTool(void) const;
		size_t				getNbWorkers(void) const;
		bool				hasWorker(const Worker* p_worker) const;

		// both directions are public so we can call them from main
		bool	enrolWorker(Worker* p_worker);
		bool	releaseWorker(Worker* p_worker);

		// Runs the day for every worker currently registered
		void	executeWorkDay(void);

	private:

		// mutual friendship
		friend class Worker;

		// Shared worker-workshop search, returns false and leaves p_index as it was
		bool	_findWorker(const Worker* p_worker, size_t& p_index) const;

		/* BONUS: Compared on the tool NAME and not on a typeid, for two reasons:
		*   -1. A Workshop holding a Tool* prototype would have to invent a throwaway
		*     instance of an ABSTRACT class which cannot be bdone
		*   -2. RTTI (Run-Time Type Information) is already used when they ask for an object
		*		and ist not necessary;
		*/
		bool	_hasRequiredTool(const Worker& p_worker) const;

		/* BONUS: A worker must be able to ASK whether it still belongs here, but must not be able to diretcly tell the workshop to drop him
		* Worker::_checkWorkshops() reaches this only to re-evaluate a membership it already holds
		*/
		void	_checkWorker(Worker& p_worker);

		std::string				_name;
		std::string				_requiredTool;
		std::vector<Worker*>		_workers;
};

#endif
