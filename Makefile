# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/30 18:16:42 by ufalzone          #+#    #+#              #
#    Updated: 2025/05/14 19:32:07 by ufalzone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Couleurs
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
CYAN = \033[36m
PURPLE = \033[35m
BOLD = \033[1m
RESET = \033[0m

# Nom de l'exécutable
NAME = minishell

# Répertoires
SRC_DIR = src
INCLUDE_DIR = includes
OBJ_DIR = obj
DEPFLAGS = -MMD -MP

# Libft
LIBFT_DIR = full_libft
LIBFT = $(LIBFT_DIR)/libftfull.a
LIBFT_URL = https://github.com/ufalzone/libft_up.git

# Fichiers sources
SRC_FILES = src/builtin/cd.c \
			src/builtin/echo.c \
			src/builtin/env.c \
			src/builtin/exit.c \
			src/builtin/export.c \
			src/builtin/pwd.c \
			src/builtin/unset.c \
			src/env_parsing/env_parsing_2.c \
			src/env_parsing/env_parsing.c \
			src/env_parsing/utils_env_parsing.c \
			src/exec/builtin.c \
			src/exec/exec.c \
			src/exec/exec_process.c \
			src/exec/exec_types.c \
			src/exec/exec_utils.c \
			src/exec/fd_utils.c \
			src/exec/file_utils.c \
			src/exec/heredoc.c \
			src/exec/pipes.c \
			src/exec/redirs.c \
			src/expansion/expansion.c \
			src/expansion/wildcard/newline_wildcard.c \
			src/expansion/wildcard/utils_wildcard.c \
			src/expansion/wildcard/wildcard.c \
			src/garbage_collector/garbage_collector_2.c \
			src/garbage_collector/garbage_collector.c \
			src/garbage_collector/utils/gc_calloc.c \
			src/garbage_collector/utils/gc_itoa.c \
			src/garbage_collector/utils/gc_split.c \
			src/garbage_collector/utils/gc_split_first_word.c \
			src/garbage_collector/utils/gc_strdup_array.c \
			src/garbage_collector/utils/gc_strdup.c \
			src/garbage_collector/utils/gc_strjoin.c \
			src/garbage_collector/utils/gc_strjoin_three.c \
			src/garbage_collector/utils/gc_strndup_array.c \
			src/garbage_collector/utils/gc_strndup.c \
			src/garbage_collector/utils/gc_substr.c \
			src/main/main.c \
			src/parsing/ast/ast.c \
			src/parsing/ast/ast_utils.c \
			src/parsing/ast/ast_visualizer.c \
			src/parsing/parsing.c \
			src/parsing/parsing_check.c \
			src/parsing/parsing_cmd.c \
			src/parsing/parsing_error.c \
			src/parsing/parsing_logic_redirs.c \
			src/readline/readline.c \
			src/readline/signals_2.c \
			src/readline/signals.c \
			src/token/operator.c \
			src/token/operator_utils.c \
			src/token/token.c \
			src/token/token_utils.c \
			src/utils/clean_exit.c

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)

# Flags de compilation
CC      = cc
CFLAGS  = -Wall -Wextra -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include
DEPFLAGS= -MMD -MP
LDFLAGS = -L$(LIBFT_DIR) -lreadline -lftfull

# Règles
.PHONY: all clean fclean re libft test

# Compilation principale
all: banner $(NAME)

# Bannière stylée
banner:
	@echo ""
	@echo "$(BOLD)$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(BOLD)$(PURPLE) ██▓███   ██▓▒███████▒▒███████▒  ██████  ██░ ██ ▓█████  ██▓     ██▓    $(RESET)"
	@echo "$(BOLD)$(PURPLE)▓██░  ██▒▓██▒▒ ▒ ▒ ▄▀░▒ ▒ ▒ ▄▀░▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    $(RESET)"
	@echo "$(BOLD)$(PURPLE)▓██░ ██▓▒▒██▒░ ▒ ▄▀▒░ ░ ▒ ▄▀▒░ ░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    $(RESET)"
	@echo "$(BOLD)$(PURPLE)▒██▄█▓▒ ▒░██░  ▄▀▒   ░  ▄▀▒   ░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    $(RESET)"
	@echo "$(BOLD)$(PURPLE)▒██▒ ░  ░░██░▒███████▒▒███████▒▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒$(RESET)"
	@echo "$(BOLD)$(PURPLE)▒▓▒░ ░  ░░▓  ░▒▒ ▓░▒░▒░▒▒ ▓░▒░▒▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░$(RESET)"
	@echo "$(BOLD)$(PURPLE)░▒ ░      ▒ ░░░▒ ▒ ░ ▒░░▒ ▒ ░ ▒░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░$(RESET)"
	@echo "$(BOLD)$(PURPLE)░░        ▒ ░░ ░ ░ ░ ░░ ░ ░ ░ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   $(RESET)"
	@echo "$(BOLD)$(PURPLE)          ░    ░ ░      ░ ░          ░   ░  ░  ░   ░  ░    ░  ░    ░  ░$(RESET)"
	@echo "$(BOLD)$(PURPLE)             ░        ░                                               $(RESET)"
	@echo "$(BOLD)$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo ""

# Libft: clone et compilation
libft: $(LIBFT)

$(LIBFT):
	@printf "$(YELLOW)Initialisation libft$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@printf " $(GREEN)OK$(RESET)\n"
	@cd $(LIBFT_DIR) 2>/dev/null || (printf "$(YELLOW)Clonage libft$(RESET)" && for i in 1 2 3; do printf "."; sleep 0.2; done && git clone --quiet $(LIBFT_URL) $(LIBFT_DIR) && printf " $(GREEN)OK$(RESET)\n")
	@printf "$(YELLOW)Compilation libft$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@printf " $(GREEN)OK$(RESET)\n"
	@$(MAKE) -s -C $(LIBFT_DIR)

# Création du dossier obj
$(OBJ_DIR):
	@printf "$(CYAN)Création dossier obj$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@mkdir -p $(OBJ_DIR)
	@printf " $(GREEN)OK$(RESET)\n"

# Création des sous-répertoires du dossier obj (même structure que src/)
$(OBJ_DIR)/%/:
	@mkdir -p $@

# Compilation des .c -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@printf "$(CYAN)Compilation $<$(RESET)"
	@for i in 1 2 3; do printf "."; done
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@
	@printf " $(GREEN)OK$(RESET)\n"

# Linking et message d'animation
$(NAME): libft $(OBJ_FILES)
	@printf "$(YELLOW)Linking$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.3; done
	@$(CC) $(CFLAGS) $(OBJ_FILES) -L$(LIBFT_DIR) -lftfull -lreadline -o $(NAME)
	@printf " $(GREEN)OK$(RESET)\n"

# Nettoyage des fichiers objets
clean:
	@printf "$(RED)Nettoyage obj$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@printf " $(GREEN)OK$(RESET)\n"

# Nettoyage complet
fclean: clean
	@printf "$(RED)Suppression binaire$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@rm -f $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@printf " $(GREEN)OK$(RESET)\n"

# Tout recompiler
re: fclean all

# Inclusion des dépendances
-include $(DEP_FILES)
