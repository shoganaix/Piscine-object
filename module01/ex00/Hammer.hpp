/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hammer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HAMMER_HPP
# define HAMMER_HPP

# include "Tool.hpp"		// the abstract base, INHERITANCE


/* ! Note: Hammer exists only to prove that INHERITANCE is real
 * -----------------------------------------------------------------------
 * A class with a single concrete tool would never need a base class at all.
 * Two unrelated tools sharing one interface is exactly what forces the
 * abstraction: Worker stores a std::vector<Tool*>, and thanks to the virtual
 * use() that vector accepts a shovel and a hammer with no discrimination.
 * -----------------------------------------------------------------------
 * It also proves dynamic dispatch: the very same call, p_tool->use(), runs
 * Shovel::use() on a Shovel and Hammer::use() on a Hammer, and the caller
 * cannot tell the difference because it only knows about Tool.
*/
class Hammer : public Tool
{
	public:

		Hammer(void);
		virtual ~Hammer();

		// Both pure virtual functions of Tool, implemented here
		virtual void		use(void);
		virtual std::string	getToolName(void) const;
};

#endif
