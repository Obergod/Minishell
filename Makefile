# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/30 18:16:42 by ufalzone          #+#    #+#              #
#    Updated: 2025/04/24 20:55:59 by ufalzone         ###   ########.fr        #
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
	@rm -f builtin_test
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(BOLD)$(GREEN)🌈 Tout est prêt pour une renaissance éclatante !$(RESET)"

# Recompile tout
re: fclean all

#Compile tous les tests disponibles
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
			$(shell find $(SRC_DIR)/exec -name "*.c") \
			$(shell find $(SRC_DIR)/parsing -name "*.c") \
			$(shell find $(SRC_DIR)/expansion -name "*.c") \
			$(shell find $(SRC_DIR)/builtin -name "*.c") \
			$(shell find $(SRC_DIR)/readline -name "*.c") \
			$(shell find $(SRC_DIR)/utils -name "*.c") \
			$(shell find $(SRC_DIR)/expansion/ugo_wildcard_2 -name "*.c") \
			-I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include -Wall -Wextra -Werror -g3 $(LDFLAGS) -L$(LIBFT_DIR) -lftfull || exit 1; \
	done
	@echo "$(BOLD)$(GREEN)✅ Tous les tests ont été compilés avec succès !$(RESET)"

# Teste les commandes builtin
test_builtin: $(LIBFT)
	@echo "$(BOLD)$(YELLOW)🧪 Test des commandes builtin en cours...$(RESET)"
	@if [ -f $(SRC_DIR)/builtin/main.c ]; then \
		echo "$(BOLD)$(CYAN)📝 Compilation des tests builtin...$(RESET)"; \
		$(CC) -o builtin_test $(SRC_DIR)/builtin/main.c \
			$(shell find $(SRC_DIR)/builtin -type f -name "*.c" ! -name "main.c") \
			$(shell find $(SRC_DIR)/garbage_collector -maxdepth 1 -name "*.c") \
			$(shell find $(SRC_DIR)/garbage_collector/utils -name "*.c") \
			$(shell find $(SRC_DIR)/env_parsing -name "*.c") \
			$(shell find $(SRC_DIR)/utils -name "*.c") \
			-g3\
			$(CFLAGS) $(LDFLAGS) -L$(LIBFT_DIR) -lftfull || exit 1; \
		echo "$(BOLD)$(GREEN)✅ Tests builtin compilés avec succès !$(RESET)"; \
		echo "$(BOLD)$(PURPLE)🚀 Exécution des tests builtin...$(RESET)"; \
		./builtin_test; \
		echo "$(BOLD)$(GREEN)✅ Tests builtin exécutés avec succès !$(RESET)"; \
	else \
		echo "$(BOLD)$(RED)❌ Fichier main.c introuvable dans $(SRC_DIR)/builtin/ !$(RESET)"; \
		echo "$(BOLD)$(YELLOW)ℹ️ Veuillez créer un fichier main.c dans le dossier builtin pour tester les commandes.$(RESET)"; \
	fi

# Compile le test AST
ast_test:
	@if [ -f tests/ast_test.c ]; then \
		echo "$(BOLD)$(YELLOW)🧪 Compilation des tests AST...$(RESET)"; \
		$(CC) -o ast_test tests/ast_test.c -I./tests -I$(LIBFT_DIR)/include $(CFLAGS) && \
		echo "$(BOLD)$(GREEN)✅ Tests AST compilés avec succès !$(RESET)" && \
		echo "$(BOLD)$(PURPLE)🚀 Exécution des tests AST...$(RESET)" && \
		./ast_test && \
		rm -f ast_test; \
	else \
		echo "$(BOLD)$(YELLOW)⚠️ Fichier tests/ast_test.c non trouvé, test AST ignoré$(RESET)"; \
	fi

-include $(DEP_FILES)
