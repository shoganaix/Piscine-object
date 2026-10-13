# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/28 17:40:49 by root              #+#    #+#              #
#    Updated: 2026/10/13 13:11:09 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# The modules of the piscine, each one owns the Makefiles of its own exercises
MODULES := module00 module01 module02

# Root Makefile doesnt build anything instead forwards each goal to the module
# Makefiles, which in turn forward it to their exercises. The chain is:
#     make (root)  ->  make (moduleXX)  ->  make (moduleXX/exYY)

# The loop stops at first failure, so a broken exercise is never hidden behind
all clean fclean re:
	@for mod in $(MODULES); do \
		echo "--- $$mod : $@ ---"; \
		$(MAKE) -C $$mod $@ || exit 1; \
	done

.PHONY: all clean fclean re
