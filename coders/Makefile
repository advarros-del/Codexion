# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adrian <adrian@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/30 11:27:05 by adrvarga          #+#    #+#              #
#    Updated: 2026/10/25 19:32:40 by adrian           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#Name of the file that the makfile it's going to create.
NAME = codexion

#compiler that you will going to use
CC = cc 

#Flags that the compiler have to be use.
CFLAGS = -Wextra -Werror -Wall -g 

#All files .c that you need
SRC = codexion.c\
	ft_atol.c\
	main.c\
	fill_struc.c\
	rutines.c\
	scheduler.c\
	compilation.c\
	
#Transform all the .c to .o
OBJS = $(SRC:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
