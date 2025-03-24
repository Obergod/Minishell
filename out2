# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/30 18:16:42 by ufalzone          #+#    #+#              #
#    Updated: 2025/03/21 16:56:51 by ufalzone         ###   ########.fr        #
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

# Fichiers sources et objets
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)
DEP_FILES = $(OBJ_FILES:.o=.d) $(OBJ_FILES_BONUS:.o=.d)

# Flags de compilation
CC = cc
CFLAGS = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include -Wall -Wextra -Werror -O3 -g3
LDFLAGS = -lreadline

# Règles
.PHONY: all clean fclean re libft test

# Compilation principale
all: banner $(OBJ_DIR) libft $(NAME)
	@echo "$(BOLD)$(YELLOW)✨ $(GREEN)MINISHELL $(CYAN)PRÊT À L'EMPLOI ! $(YELLOW)PROFITEZ DE CETTE MERVEILLE TECHNOLOGIQUE ! ✨$(RESET)"

# Bannière stylée
banner:
	@echo "\n$(BOLD)$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(BOLD)$(PURPLE)                         ✧ MINISHELL EXTRAORDINAIRE ✧                         $(RESET)"
	@echo "$(BOLD)$(BLUE)                 LE SHELL ULTIME QUI REDÉFINIT L'EXCELLENCE                     $(RESET)"
	@echo "$(BOLD)$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"

# Compilation de la libft
libft:
	@echo "$(BOLD)$(YELLOW)🔮 Compilation de la libft légendaire...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(BOLD)$(GREEN)💎 Libft compilée avec brio ! Une merveille d'ingénierie !$(RESET)"

# Création du dossier obj et ses sous-dossiers
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJ_FILES))
	@echo "$(BOLD)$(BLUE)🏗️  Création des structures de dossiers ultra-optimisées...$(RESET)"

# Compile le projet minishell
$(NAME): $(OBJ_FILES) $(LIBFT)
	@printf "$(BOLD)$(GREEN)⚡ Compilation du légendaire MINISHELL en cours...$(RESET)                       \r"
	@sleep 0.5
	@printf "$(BOLD)$(CYAN)⚡ Optimisation du code à la perfection...$(RESET)                               \r"
	@sleep 0.5
	@printf "$(BOLD)$(PURPLE)⚡ Intégration des technologies de pointe...$(RESET)                           \r"
	@sleep 0.5
	@$(CC) $(OBJ_FILES) $(LIBFT) $(CFLAGS) $(LDFLAGS) -o $(NAME)
	@printf "\n$(BOLD)$(YELLOW)🚀 COMPILATION RÉUSSIE AVEC BRIO ! $(GREEN)$(NAME) $(YELLOW)EST MAINTENANT PRÊT À RÉVOLUTIONNER VOTRE MONDE !$(RESET)\n"
	@echo "$(BOLD)$(CYAN)🌟 Une expérience utilisateur inégalée vous attend...$(RESET)"

# Règle de compilation des fichiers objets
$(OBJ_DIR)/%.o: %.c
	@printf "$(BOLD)$(CYAN)🔧 Transformation magique de $<...$(RESET)\r"
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	@echo "$(BOLD)$(RED)🧹 Nettoyage cosmique des fichiers temporaires...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(BOLD)$(GREEN)✨ Espace nettoyé avec une précision chirurgicale !$(RESET)"

# Nettoyage complet
fclean: clean
	@echo "$(BOLD)$(RED)🗑️ Suppression stratégique de $(NAME) pour préparer une nouvelle ère...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(BOLD)$(GREEN)🌈 Tout est prêt pour une renaissance éclatante !$(RESET)"

# Recompile tout
re: fclean all

# Règle de test dynamique
# Usage: make test_X où X est le nom du fichier dans test_dir sans l'extension .c
test_%: $(LIBFT)
	@echo "$(BOLD)$(YELLOW)👾 Compilation du test $(GREEN)$*$(YELLOW) en cours...$(RESET)"
	@mkdir -p test_dir/bin
	@$(CC) -o test_dir/bin/$* test_dir/$*.c \
		$(shell find $(SRC_DIR)/token -name "*.c") \
		$(shell find $(SRC_DIR)/garbage_collector -name "*.c") \
		$(shell find $(SRC_DIR)/env_parsing -name "*.c") \
		$(shell find $(SRC_DIR)/parsing -name "*.c") \
		$(CFLAGS) $(LDFLAGS) -L$(LIBFT_DIR) -lftfull
	@echo "$(BOLD)$(GREEN)✅ Test $(CYAN)$*$(GREEN) compilé avec succès !$(RESET)"
	@echo "$(BOLD)$(PURPLE)🚀 Exécutez avec: ./test_dir/bin/$* <arguments>$(RESET)"

# Compile tous les tests disponibles
test: $(LIBFT)
	@echo "$(BOLD)$(YELLOW)🧪 Compilation de tous les tests disponibles...$(RESET)"
	@mkdir -p test_dir/bin
	@for test in $(wildcard test_dir/*.c); do \
		test_name=$$(basename $$test .c); \
		echo "$(BOLD)$(CYAN)📝 Compilation de $$test_name...$(RESET)"; \
		$(CC) -o test_dir/bin/$$test_name $$test \
			$(shell find $(SRC_DIR)/token -name "*.c") \
			$(shell find $(SRC_DIR)/garbage_collector -name "*.c") \
			$(shell find $(SRC_DIR)/env_parsing -name "*.c") \
			$(shell find $(SRC_DIR)/parsing -name "*.c") \
			$(CFLAGS) $(LDFLAGS) -L$(LIBFT_DIR) -lftfull || exit 1; \
	done
	@echo "$(BOLD)$(GREEN)✅ Tous les tests ont été compilés avec succès !$(RESET)"

-include $(DEP_FILES)
