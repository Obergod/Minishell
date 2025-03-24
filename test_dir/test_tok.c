#include "../includes/minishell.h"
#include "../includes/garbage_collector.h"
#include "../includes/parsing.h"
#include "../includes/token.h"
#include "../includes/expand.h"
#include "../includes/env_parsing.h"
#include "../includes/ast.h"
#include "../src/ast_visualizer/ast_visualizer.h"
#include <stdio.h>

/* Global variable accessible to all files in the project */
int	g_exit_status = 0;

// Déclaration de la fonction build_ast
t_ast_node *build_ast(t_cmd *cmd, t_minishell *minishell);

// Fonction de débogage pour afficher un nœud AST
void debug_print_ast_node(t_ast_node *node, int level)
{
	if (!node)
	{
		printf("%*s[NULL NODE]\n", level * 2, "");
		return;
	}

	char *type_str = "UNKNOWN";
	switch(node->type)
	{
		case NODE_NONE: type_str = "NONE"; break;
		case NODE_CMD: type_str = "CMD"; break;
		case NODE_PIPE: type_str = "PIPE"; break;
		case NODE_AND: type_str = "AND"; break;
		case NODE_OR: type_str = "OR"; break;
		case NODE_OPEN_PARENTHESIS: type_str = "OPEN_PARENTHESIS"; break;
		case NODE_CLOSE_PARENTHESIS: type_str = "CLOSE_PARENTHESIS"; break;
	}

	printf("%*sNODE Type: %s (subshell: %d)\n", level * 2, "", type_str, node->subshell);

	if (node->cmd)
	{
		printf("%*s  CMD: ", level * 2, "");
		if (node->cmd->command_raw)
			printf("'%s' ", node->cmd->command_raw);
		else
			printf("(null raw) ");

		if (node->cmd->command)
		{
			printf("Args: ");
			for (int i = 0; node->cmd->command[i]; i++)
				printf("[%s] ", node->cmd->command[i]);
		}
		else
			printf("(null args)");

		printf("\n");
	}
	else
		printf("%*s  CMD: (null)\n", level * 2, "");

	if (node->left)
	{
		printf("%*sLEFT:\n", level * 2, "");
		debug_print_ast_node(node->left, level + 1);
	}

	if (node->right)
	{
		printf("%*sRIGHT:\n", level * 2, "");
		debug_print_ast_node(node->right, level + 1);
	}
}

// Fonction récursive pour déboguer l'AST complet
void debug_print_ast(t_ast_node *root)
{
	printf("---------- AST DEBUG DUMP ----------\n");
	if (!root)
		printf("AST is NULL\n");
	else
		debug_print_ast_node(root, 0);
	printf("------------------------------------\n");
}

int	main(int ac, char **av, char **envp)
{
	t_token *token;
	t_minishell minishell;
	t_gc_head *gc_head;
	t_cmd *cmd_head;
	t_ast_node *ast;

	if (ac < 2)
	{
		printf("Usage: %s \"input string\"\n", av[0]);
		return (1);
	}

	gc_head = gc_init();
	minishell.gc = gc_head;

	// Initialize environment
	minishell.env = env_parsing(envp, &minishell);
	if (!minishell.env) {
		printf("Error: Failed to parse environment\n");
		gc_destroy(gc_head);
		return (1);
	}

	token = tokenize(av[1], &minishell);
	if (token)
	{
		token = expand_vars(token, &minishell);

		// Print token information
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
	else
	{
		printf("Error: Failed to tokenize input\n");
	}
	cmd_head = parsing(token, &minishell);
	printf("/*******		parsing		********/\n\n");

	// Afficher les commandes
	t_cmd *cmd_debug = cmd_head;
	int cmd_count = 0;
	while (cmd_debug)
	{
		printf("Command %d:\n", cmd_count++);
		printf("  Command_raw: %s\n", cmd_debug->command_raw ? cmd_debug->command_raw : "(null)");

		printf("  Arguments: ");
		if (cmd_debug->command)
		{
			for (int i=0; cmd_debug->command[i]; i++)
				printf("[%s] ", cmd_debug->command[i]);
			printf("\n");
		}
		else
			printf("(null)\n");

		// Afficher les redirections
		printf("  Redirections:\n");
		t_redir *current_redir = cmd_debug->redirs;
		int redir_count = 0;
		while (current_redir)
		{
			printf("    [%d] ", redir_count++);
			switch (current_redir->type)
			{
				case REDIR_IN:
					printf("Type: < (REDIR_IN)");
					break;
				case REDIR_OUT:
					printf("Type: > (REDIR_OUT)");
					break;
				case REDIR_APPEND:
					printf("Type: >> (REDIR_APPEND)");
					break;
				case REDIR_HEREDOC:
					printf("Type: << (REDIR_HEREDOC)");
					break;
				default:
					printf("Type: UNKNOWN");
			}
			printf(", File/Delimiter: %s\n", current_redir->file_or_delimiter);
			current_redir = current_redir->next;
		}
		if (redir_count == 0)
			printf("    Aucune redirection\n");

		printf("  Logic operator type: %d\n", cmd_debug->logic_operator_type);
		printf("\n");
		cmd_debug = cmd_debug->next;
	}

	// Construire et visualiser l'AST
	printf("/*******		AST		********/\n\n");
	cmd_head = parsing(token, &minishell); // Recréer cmd_head car on l'a traversé complètement

	printf("Commandes avant de construire l'AST:\n");
	cmd_debug = cmd_head;
	cmd_count = 0;
	while (cmd_debug)
	{
		printf("  Cmd %d: %s (Op: %d)\n",
			cmd_count++,
			cmd_debug->command_raw ? cmd_debug->command_raw : "(null)",
			cmd_debug->logic_operator_type);
		cmd_debug = cmd_debug->next;
	}

	printf("\nConstruction de l'AST...\n");
	ast = build_ast(cmd_head, &minishell);
	printf("AST construit: %p\n", (void*)ast);

	if (ast)
	{
		// Afficher l'arbre construit pour débogage
		debug_print_ast(ast);

		printf("Génération de la visualisation de l'AST...\n");
		visualize_ast(ast, "ast_output");
		printf("Visualisation terminée. Vérifiez ast_output.png\n");
	}
	else
	{
		printf("Erreur: Impossible de construire l'AST\n");
	}

	gc_destroy(gc_head);
	return (0);
}
