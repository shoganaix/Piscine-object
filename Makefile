# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/28 17:40:49 by root              #+#    #+#              #
#    Updated: 2026/09/28 19:36:38 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXERCISES := ex00 ex01

# root Makefile doesnt build anything instead forwards each goal to other Makefiles

# loop stops at first failure, so a broken exercise is never hidden behind
all clean fclean re:
	@for dir in $(EXERCISES); do \
		echo "--- $$dir : $@ ---"; \
		$(MAKE) -C $$dir $@ || exit 1; \
	done

.PHONY: all clean fclean re
