/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 17:12:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>			// std::string

class Worker;


/* This is the abstract base, meaning an object Tool cannot be instantiated 
 * 	- numberOfUses is PRIVATE: a shovel may increment the counter through
 *	countUse(), but it can never rewrite it or read without getters
*/
class Tool
{
	public:

		virtual			~Tool();

		virtual void		use(void) = 0;						// PURE VIRTUAL, never defined
		virtual std::string	getToolName(void) const = 0;		// PURE VIRTUAL, never defined

		int					getNumberOfUses(void) const;

		Worker*				getHolder(void) const;

	protected:

		// Protected and not private because ONLY a derived tool may be built, but ANYBODY may hold one
		Tool(void);

		void				_countUse(void);

	private:

		/* ! Note: copy constructor & copy assignment are DECLARED but NEVER DEFINED. 
		 * 	 Using either is a LINK error, not a compile error
		* 	A tool is unique because it is the very object a worker hold. A copy
		* would mean  a second shovel and a copy assignment would make two workers  hold the same  tool
		*/
		Tool(const Tool& p_other);
		Tool&	operator=(const Tool& p_other);

		// Only a worker may claim a tool, so nobody can make two workers hold the same by calling a setter
		friend class Worker;
		void				_setHolder(Worker* p_holder);

		int					_numberOfUses;						// private in the base, reachable only through _countUse()
		Worker*				_holder;							// allows us to change holder (borrow tool function)
};

#endif
