# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/28 17:40:49 by root              #+#    #+#              #
#    Updated: 2026/09/28 19:47:01 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME     := ex01
CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98

SRCS     := main.cpp Vector2.cpp Graph.cpp
OBJS     := $(SRCS:.cpp=.o)
DEPS     := Graph.hpp Vector2.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
