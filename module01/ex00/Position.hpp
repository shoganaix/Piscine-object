/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Position.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/29 16:20:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSITION_HPP
# define POSITION_HPP

/* ! Note: the subject asks for a STRUCT
 * -----------------------------------------------------------------------
 * A struct has public members by default. Turning it into a class would 
 * only add private members and getters we don't need.
 */

struct Position
{
	int	x;
	int	y;
	int	z;

	Position(int p_x, int p_y, int p_z);
};

#endif
