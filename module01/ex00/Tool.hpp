/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>			// std::string, the identity every tool answers to


class Worker;				// ! Note: forward declaration on purpose, Tool only ever stores a POINTER to a worker


/* ! Note: INHERITANCE, this is the abstract base class the subject asks for
 * =======================================================================
 * Tool cannot be instantiated, and that is the whole point: there is no such
 * thing as a plain tool, there are shovels and hammers.
 *
 * TWO CONCEPTS MUST NOT BE CONFUSED
 * - "abstract class" means Tool has at least one PURE VIRTUAL function, use().
 * - "polymorphic" means the vtable exists, which is what lets a Tool* point at
 *   a Shovel and still call the right code. Both are needed here, and both come
 *   from the same keyword.
 *
 * WHY THE DESTRUCTOR IS VIRTUAL, and this is the one that bites
 * If ~Tool() were not virtual, "delete aToolPointer" would run only the Tool
 * part and the Shovel part would be left undestroyed. Declaring it virtual
 * costs nothing and is mandatory the moment a class is meant to be held
 * through a pointer to its base.
 * =======================================================================
 * WHY _numberOfUses LIVES HERE AND NOT IN Shovel OR Hammer
 * The subject says "each tool must have a number of uses". Shovel and Hammer
 * share that counter and nothing else, so this is the one piece of state worth
 * factoring up. It is PRIVATE: a shovel may increment the counter through
 * _countUse(), but it can never rewrite it, and it can never read it back
 * without going through the const getter.
*/
class Tool
{
	public:

		virtual			~Tool();								// MUST be virtual, see the note above

		/* The two things every tool has to be able to do
		* use() does the work AND says which tool it is, in one call
		* getToolName() is that same identity as plain data, needed by the Workshop
		* filter so a tool can be recognised without being swung at something
		*/
		virtual void		use(void) = 0;						// PURE VIRTUAL, never defined
		virtual std::string	getToolName(void) const = 0;		// PURE VIRTUAL, never defined

		// Const getter, silent like every other const getter, printing here would flood the trace
		int					getNumberOfUses(void) const;

		/* AGGREGATION, seen from the tool side
		* The worker currently holding the tool, or 0 when the tool sits on the shelf.
		* This is what lets a hand-over be resolved without any global list of workers.
		*/
		Worker*				getHolder(void) const;

	protected:

		// Protected and not private: only a derived tool may be built, but anybody may hold one
		Tool(void);

		// The ONLY writer of _numberOfUses, called by every derived use()
		void				_countUse(void);

	private:

		/* ! Note: copy constructor & copy assignment are DECLARED AND NEVER DEFINED
		* -----------------------------------------
		* Using either is a LINK error, not a compile error, so nothing is hidden
		* from the reader of the header.
		* A tool has an IDENTITY: it is the very object the workers hold. A copy
		* would silently be a second shovel nobody asked for, and a copy assignment
		* would make two workers believe they hold the same physical tool.
		* -----------------------------------------
		* Contrast with the SHALLOW containers used in module 00: those store their
		* elements BY VALUE inside std::map and std::vector, and the standard
		* containers are not friends, so a private copy constructor would break
		* them. Here nothing is ever stored by value, so they can safely be closed.
		*/
		Tool(const Tool& p_other);
		Tool&	operator=(const Tool& p_other);

		// ! Note: a private method guarded by a friend, rather than a public setter
		// Only a worker may claim a tool, so nobody can make two workers believe
		// they hold the same shovel by calling a public setHolder()
		friend class Worker;
		void				_setHolder(Worker* p_holder);

		int					_numberOfUses;						// private in the base, reachable only through _countUse()
		Worker*				_holder;							// 0 = the tool is on the shelf, see the AGGREGATION note in Worker.hpp
};

#endif
