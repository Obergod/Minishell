# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/30 18:16:42 by ufalzone          #+#    #+#              #
#    Updated: 2025/05/12 20:17:44 by ufalzone         ###   ########.fr        #
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

# Fichiers sources et objets
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)

# Flags de compilation
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include
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
