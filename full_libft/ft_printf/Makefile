# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mafioron <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 18:30:26 by mafioron          #+#    #+#              #
#    Updated: 2024/11/19 14:01:37 by mafioron         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = src/ft_printf.c src/ft_printnbr_base.c src/ft_printptr.c src/ft_print_ut.c src/ft_printnbs.c \
      src/ft_printnbs.c src/ft_printunsigned.c


OBJ = $(SRC:.c=.o)

NAME = libftprintf.a

CFLAGS +=  -Werror -Wextra -Wall -I include

all:$(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)
fclean: 
	rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all clean fclean re
