/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:59:20 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/18 16:48:24 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

int	g_exit_status = 0;

static char	*ft_add_readline(const char *prompt, char **stock)
{
	(*stock) = readline(prompt);
	if (*stock == NULL)
		return (NULL);
	return (*stock);
}

// Gestionnaire de signal
static void	handle_signal(int sig)
{
	if (sig == SIGINT) // Ctrl+C
	{
		printf("^C");
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGTERM) // Ctrl + D
	{
		printf("\nSignal de terminaison, il faut bien exit proprement.\n");
		exit(1);
	}
	else if (sig == SIGQUIT) // Ctrl+ "\"
	{
		printf("Ctrl + \\\n");
	}
}

int	init_minishell(t_minishell *minishell, char **envp)
{
	t_gc_head	*gc_head;

	gc_head = gc_init();
	minishell->gc = gc_head;

	// Initialize environment
	minishell->env = env_parsing(envp, minishell);
	if (!minishell->env) {
		printf("Error: Failed to parse environment\n"); //creer un environnement
															//PWD=/home/ufalzone/42cursus/Cercle-3/Minishell/off-minishell-github/test_dir/bin
															// SHLVL=1
															// _=/usr/bin/env
		gc_destroy(gc_head);
		return (1);
	}
	return (0);
}

void	print_token(t_token *token)
{
		t_token *current = token;
		printf("******/		tokenization		******/\n\n");
		while (current)
		{
			printf("str : %s\n", current->str);
			printf("t_type : %u\n", current->type);
			printf("state : %u\n\n", current->state);
			current = current->next;
		}
		printf("\n\n\n");

}

void test_ast(t_ast_node *ast)
{
	t_ast_node *tmp;
	char *temp;

	tmp = ast;
	while (tmp)
	{
		if (tmp->type == NODE_CMD)
			printf("left cmd :%s\n", tmp->cmd->command_raw);
		else
			printf("left operator: %i\n", tmp->type);
		if (tmp->right)
			temp = tmp->right->cmd->command_raw;
		tmp = tmp->left;
	}
	printf("%s\n", temp);
	// tmp = ast;
	// while (tmp)
	// {
	// 	if (tmp->type == NODE_CMD)
	// 		printf("right cmd :%s\n", tmp->cmd->command_raw);
	// 	else
	// 		printf("right operator: %i\n", tmp->type);
	// 	tmp = tmp->right;

	// }
}

void	print_cmd_list(t_cmd *cmd_head)
{
	t_cmd *current;
	t_redir *redir;
	int i;
	int cmd_num;

	current = cmd_head;
	cmd_num = 1;
	printf("******/		cmd_list		******/\n\n");
	while (current)
	{
		printf("Commande %d:\n", cmd_num);
		printf("  command_raw: %s\n", current->command_raw);
		printf("  arguments: ");
		i = 0;
		while (current->command && current->command[i])
		{
			printf("[%s] ", current->command[i]);
			i++;
		}
		printf("\n");
		printf("  arg_count: %zu\n", current->_arg_count);

		// Afficher le type d'opérateur logique
		printf("  operateur: ");
		if (current->logic_operator_type == NONE)
			printf("NONE");
		else if (current->logic_operator_type == PIPE)
			printf("PIPE |");
		else if (current->logic_operator_type == AND)
			printf("AND &&");
		else if (current->logic_operator_type == OR)
			printf("OR ||");
		else if (current->logic_operator_type == OPEN_PARENTHESIS)
			printf("OPEN_PARENTHESIS (");
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
			printf("CLOSE_PARENTHESIS )");
		printf("\n");

		// Afficher les redirections
		printf("  redirections: ");
		redir = current->redirs;
		if (!redir)
			printf("aucune\n");
		else
		{
			printf("\n");
			while (redir)
			{
				printf("    type: ");
				if (redir->type == REDIR_IN)
					printf("<");
				else if (redir->type == REDIR_OUT)
					printf(">");
				else if (redir->type == REDIR_APPEND)
					printf(">>");
				else if (redir->type == REDIR_HEREDOC)
					printf("<<");
				printf(", fichier/délimiteur: %s\n", redir->file_or_delimiter);
				redir = redir->next;
			}
		}

		printf("\n");
		current = current->next;
		cmd_num++;
	}
	printf("\n\n");
}

// Parcours préfixe (racine, gauche, droite)
void parcours_prefixe(t_ast_node *node)
{
	if (!node)
		return;

	// Visite de la racine
	if (node->type == NODE_CMD)
		printf("Préfixe CMD: %s\n", node->cmd->command_raw);
	else if (node->type == NODE_PIPE)
		printf("Préfixe PIPE\n");
	else if (node->type == NODE_AND)
		printf("Préfixe AND\n");
	else if (node->type == NODE_OR)
		printf("Préfixe OR\n");

	// Visite du sous-arbre gauche
	parcours_prefixe(node->left);

	// Visite du sous-arbre droit
	parcours_prefixe(node->right);
}

// Parcours infixe (gauche, racine, droite)
void parcours_infixe(t_ast_node *node)
{
	if (!node)
		return;

	// Visite du sous-arbre gauche
	parcours_infixe(node->left);

	// Visite de la racine
	if (node->type == NODE_CMD)
		printf("Infixe CMD: %s\n", node->cmd->command_raw);
	else if (node->type == NODE_PIPE)
		printf("Infixe PIPE\n");
	else if (node->type == NODE_AND)
		printf("Infixe AND\n");
	else if (node->type == NODE_OR)
		printf("Infixe OR\n");

	// Visite du sous-arbre droit
	parcours_infixe(node->right);
}

// Parcours suffixe/postfixe (gauche, droite, racine)
void parcours_suffixe(t_ast_node *node)
{
	if (!node)
		return;

	// Visite du sous-arbre gauche
	parcours_suffixe(node->left);

	// Visite du sous-arbre droit
	parcours_suffixe(node->right);

	// Visite de la racine
	if (node->type == NODE_CMD)
		printf("Suffixe CMD: %s\n", node->cmd->command_raw);
	else if (node->type == NODE_PIPE)
		printf("Suffixe PIPE\n");
	else if (node->type == NODE_AND)
		printf("Suffixe AND\n");
	else if (node->type == NODE_OR)
		printf("Suffixe OR\n");
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_token 	*token;
	t_minishell	minishell;
	t_cmd		*cmd_head;
	t_ast_node *ast;
	t_ast_node *head;

	if (ac < 1)
		return (1);
	if (!av)
		return (1);
	if (init_minishell(&minishell, envp) == 1)
		return (1);
	rl_catch_signals = 0; // Desactive la gestion auto des signaux de readline
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (ft_add_readline(PROMPT, &input) != NULL)
	{
		token = tokenize(input, &minishell);
		if (token)
		{
			token = expand_vars(token, &minishell);
			//print_token(token);
		}
		else
			printf("Error: Failed to tokenize input\n");
		cmd_head = parsing(token, &minishell);

		// Appel de la fonction pour imprimer la liste de commandes
		//if (cmd_head)
			//print_cmd_list(cmd_head);
		if (!cmd_head)
			printf("Aucune commande valide n'a été trouvée.\n");

		ast = build_ast(cmd_head, &minishell);
		head = ast;
		// test_ast(ast);

//		visualize_ast(ast, 3);

		// Effectuer les trois types de parcours
/*		if (ast)
		{
			printf("\n--- Parcours Préfixe ---\n");
			parcours_prefixe(ast);

			printf("\n--- Parcours Infixe ---\n");
			parcours_infixe(ast);

			printf("\n--- Parcours Suffixe ---\n");
			parcours_suffixe(ast);
			printf("\n");
		}*/

		prefix_exec(ast, head, &minishell);
		if (*input)
			add_history(input);
		free(input);
	}
	rl_clear_history();
	handle_signal(SIGTERM);
	return (0);
}

//
