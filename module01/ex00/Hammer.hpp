/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hammer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 16:14:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HAMMER_HPP
# define HAMMER_HPP

# include "Tool.hpp"		// the abstract base, INHERITANCE


/* ! Note: Hammer exists only to prove that INHERITANCE is real
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

		// Both pure virtual functions of Tool
		virtual void		use(void);
		virtual				std::string	getToolName(void) const;
};

#endif
