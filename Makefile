# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: root <root@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/28 17:40:49 by root              #+#    #+#              #
#    Updated: 2026/09/28 17:40:50 by root             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---- Configuration ---------------------------------------------------------- #

EXERCISES := ex00 ex01							# Every exercise of the module, in subject order

# ---- Rules ------------------------------------------------------------------ #

# The root Makefile does not build anything itself. It only forwards each goal to
# the Makefile of every exercise, which is where the real rules live. Each exercise
# keeps its own $(NAME), all, clean, fclean and re, as the subject requires.

# Forward a goal to every exercise. The loop stops at the first failure, so a broken
# exercise is never hidden behind a successful one.
all clean fclean re:
	@for dir in $(EXERCISES); do \
		echo "--- $$dir : $@ ---"; \
		$(MAKE) -C $$dir $@ || exit 1; \
	done

# Marks the targets that produce no file, so they always run
.PHONY: all clean fclean re						# Prevents make from skipping these on a second call
