/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shovel.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 16:46:38 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHOVEL_HPP
# define SHOVEL_HPP

# include "Tool.hpp"


/* ! Note: 
 * The shovel is stored BY A WORKER AS A POINTER not by value. He does
 * not own it, so he is not allowed to destroy it
 */
class Shovel : public Tool
{
	public:

		Shovel(void);
		virtual ~Shovel();

		// Both pure virtual 
		virtual void		use(void);
		virtual std::string	getToolName(void) const;
};

#endif
