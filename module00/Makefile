# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/28 22:26:21 by msoriano          #+#    #+#              #
#    Updated: 2026/09/28 22:26:21 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# The exercises of module 00, in the order the subject lists them
EXERCISES := ex00 ex01

# This Makefile builds nothing itself, it only forwards the goals down to each exercise

# The loop stops at the first failure, so a broken exercise is never hidden
# behind a later one that happens to succeed
all clean fclean re:
	@for dir in $(EXERCISES); do \
		echo "--- module00/$$dir : $@ ---"; \
		$(MAKE) -C $$dir $@ || exit 1; \
	done

# Building a single exercise without going through the whole module
ex00 ex01:
	@$(MAKE) -C $@ all

.PHONY: all clean fclean re ex00 ex01
