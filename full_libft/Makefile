# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mafioron <mafioron@student.42.fr>          +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 18:22:50 by mafioron          #+#    #+#            #
#    Updated: 2024/12/03 18:22:53 by mafioron         ###   ########.fr      #
#                                                                              #
# **************************************************************************** #

NAME = libftfull.a
CFLAGS += -Wall -Wextra -Werror

GREEN = \033[32m
YELLOW = \033[33m
RESET = \033[0m
CLEAR = \033[2K\r

all: $(NAME)

$(NAME): libft printf gnl
	@printf "$(CLEAR)$(GREEN)Creating $(NAME)...$(RESET)"
	@cp libft/libft.a $(NAME)
	@(cd ft_printf && ar x libftprintf.a && ar q ../$(NAME) *.o)
	@(cd gnl && ar x libgnl.a && ar q ../$(NAME) *.o)
	@printf "$(CLEAR)$(GREEN)✓ $(NAME) successfully created!\n"

libft:
	@printf "$(YELLOW)Compiling libft..."
	@$(MAKE) -C libft re >/dev/null 2>&1
	@printf "$(CLEAR)$(GREEN)✓ libft compiled!\n"

printf:
	@printf "$(YELLOW)Compiling printf..."
	@$(MAKE) -C ft_printf re >/dev/null 2>&1
	@printf "$(CLEAR)$(GREEN)✓ printf compiled!\n"

gnl:
	@printf "$(YELLOW)Compiling gnl..."
	@$(MAKE) -C gnl re >/dev/null 2>&1
	@printf "$(CLEAR)$(GREEN)✓ gnl compiled!\n"

clean:
	@printf "$(YELLOW)Cleaning full_libft object files..."
	@$(MAKE) -C libft clean >/dev/null 2>&1
	@$(MAKE) -C ft_printf clean >/dev/null 2>&1
	@$(MAKE) -C gnl clean >/dev/null 2>&1
	@printf "$(CLEAR)$(GREEN)✓ full_libft object files cleaned!\n"

fclean: clean
	@printf "$(YELLOW)Removing full_libft executables..."
	@$(MAKE) -C libft fclean >/dev/null 2>&1
	@$(MAKE) -C ft_printf fclean >/dev/null 2>&1
	@$(MAKE) -C gnl fclean >/dev/null 2>&1
	@rm -f $(NAME)
	@printf "$(CLEAR)$(GREEN)✓ full_libft executables removed!\n"

re: fclean all

.PHONY: all clean fclean re libft printf gnl
