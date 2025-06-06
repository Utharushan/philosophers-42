# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 11:56:22 by tuthayak          #+#    #+#              #
#    Updated: 2025/03/19 11:56:22 by tuthayak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c init.c routine.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
