/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Worker.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 17:53:45 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_HPP
# define WORKER_HPP

# include "Position.hpp"
# include "Statistic.hpp"
# include "Tool.hpp"

# include <string>				// std::string
# include <vector>				// std::vector for the toolbox and the workshop list


class Workshop;


/* Worker is the class where the FOUR relationships of the subject meet:
 *   COMPOSITION -> (pos,stats) A worker cannot exist without them, cannot be built without them, and they die with him
 *   AGGREGATION -> tools stored by pointer, and Worker destructor does NOT delete them
 *   INHERITANCE -> worker never sees Shovel or Hammer, ONLY Tool
 *   ASSOCIATION -> workshops stored by pointer in both directions, a worker may be inside several workshops and may leave any time
 *
 */
class Worker
{
	public:

		// ONE constructor, no worker can be built without a position and a statistic
		Worker(const Position& p_coordonnee, const Statistic& p_stat);

		// ! Note: copy constructor & copy assignment are DECLARED AND NEVER DEFINED
		Worker(const Worker& p_other);
		Worker&	operator=(const Worker& p_other);

		// Destructor releases the workshop and clears the tool back references but NEVER deletes
		~Worker();

		// Const getters, returned by CONST REFERENCE
		const Position&	getCoordonnee(void) const;
		const Statistic&	getStat(void) const;

		// The label used in the trace, it keeps the workers apart without inventing a name
		std::string		getLabel(void) const;

		void			giveTool(Tool* p_tool);
		bool			takeTool(Tool* p_tool);

		size_t			getNbTools(void) const;
		bool			hasTool(const std::string& p_toolName) const;

		/*BONUS: get the first tool of the right type, or nullptr otherwise
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

		bool			isRegisteredTo(const Workshop* p_workshop) const;
		size_t			getNbWorkshops(void) const;

		// Does work IF worker is registered to at least one workshop
		bool			work(void);

	private:

		/* ! Note: these two classes are friends of each other
		*   - a worker leaves a workshop   -> the worker erases its own back reference
		*   - a workshop drops a worker    -> the workshop erases its own back reference
		* Neither side may edit the other list with a public method, 
		* 	so each grants the other friendship and the two edit their OWN vector
		*/
		friend class Workshop;

		void				_forgetWorkshop(Workshop* p_workshop);

		/* BONUS: this one is called every time the toolbox changes
		* Asking every registered workshop to re-check workers is what turns
		* "he lost his tool" into "he left the workshop" without worker having to know which workshops that is
		*/
		void				_checkWorkshops(void);

		std::string			_label(void) const;

		const Position				_coordonnee;
		const Statistic			_stat;
		std::vector<Tool*>			_tools;
		std::vector<Workshop*>		_workshops;
};

#endif
