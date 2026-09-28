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

NAME     := ex01								# Name of the binary, the rule $(NAME) is required by the subject
CXX      := c++									# C++ compiler, mandated by the subject
CXXFLAGS := -Wall -Wextra -Werror -std=c++98	# Mandatory warnings, mandatory C++98 dialect

# ---- Sources ---------------------------------------------------------------- #

SRCS     := main.cpp Vector2.cpp Graph.cpp		# Every translation unit of the exercise
OBJS     := $(SRCS:.cpp=.o)						# Objects, derived from the sources by substitution
DEPS     := Graph.hpp Vector2.hpp					# Headers listed explicitly, so a header change forces a rebuild

# ---- Rules ------------------------------------------------------------------ #

# Default rule, runs when make is called with no argument
all: $(NAME)										# 'all' must build the binary, as required by the subject

# Link step. Depends on the objects, so make never relinks when they are up to date.
$(NAME): $(OBJS)									# The binary depends on every object
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@			# Link all the objects into the final executable

# Compile step. The header list in the recipe forces a rebuild when a header changes.
%.o: %.cpp $(DEPS)								# Pattern rule: any .cpp becomes its .o
	$(CXX) $(CXXFLAGS) -c $< -o $@				# $< is the first prerequisite, that is the source file

# Remove the object files, keep the binary
clean:											# 'clean' is required by the subject
	rm -f $(OBJS)									# Delete every object, -f so a missing file is not an error

# Remove the object files and the binary
fclean: clean										# 'fclean' is required by the subject, it extends clean
	rm -f $(NAME)									# Delete the executable

# Rebuild everything from scratch
re: fclean all									# 're' is required by the subject, it is fclean followed by all

# Marks the targets that produce no file, so they always run
.PHONY: all clean fclean re						# Prevents make from skipping these on a second call
