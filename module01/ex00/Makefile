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

NAME     := ex00
CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98

SRCS     := main.cpp Tool.cpp Shovel.cpp Hammer.cpp Worker.cpp Workshop.cpp \
            Position.cpp Statistic.cpp
OBJS     := $(SRCS:.cpp=.o)
DEPS     := Tool.hpp Shovel.hpp Hammer.hpp Worker.hpp Workshop.hpp \
            Position.hpp Statistic.hpp

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
