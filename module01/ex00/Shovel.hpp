/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shovel.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHOVEL_HPP
# define SHOVEL_HPP

# include "Tool.hpp"		// the abstract base, INHERITANCE


/* ! Note: the subject mentions the Shovel twice, once in the aggregation part as
 * a plain structure, and once in the inheritance part as a class deriving from
 * Tool. Only the second version is written here, and it is the one that matters:
 * a shovel that is not a Tool could not be swapped for a hammer, and the whole
 * point of IV.3 is that both can sit in the same toolbox.
 * -----------------------------------------------------------------------
 * The shovel is stored BY A WORKER AS A POINTER, never by value. That is what
 * makes the relationship an aggregation: the worker borrows the tool, he does
 * not own it, so he is not allowed to destroy it. See Worker::giveTool().
*/
class Shovel : public Tool
{
	public:

		Shovel(void);
		virtual ~Shovel();

		// Both pure virtual functions of Tool, implemented here
		virtual void		use(void);
		virtual std::string	getToolName(void) const;
};

#endif
