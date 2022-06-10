# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/08 17:44:03 by anruland          #+#    #+#              #
#    Updated: 2022/06/10 19:20:09 by anruland         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -pthread

SRC		=	main.c \
			destruct.c \
			error.c \
			init.c \
			ft_atoi.c \
			ft_itoa.c \
			ft_bzero.c \
			ft_strlen.c \
			ft_printerror.c

all: $(NAME)

$(NAME): 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f ./*.o

fclean: clean
	rm -f ./$(NAME)

re:	fclean all
