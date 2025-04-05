/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:59:20 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/04 16:49:06 by ufalzone         ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_token 	*token;
	t_minishell	minishell;
	t_cmd		*cmd_head;
	t_ast_node *ast;

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
			print_token(token);
		}
		else
			printf("Error: Failed to tokenize input\n");
		cmd_head = parsing(token, &minishell);
		ast = build_ast(cmd_head, &minishell);
		visualize_ast(ast, 3);
		if (*input)
			add_history(input);
		free(input);
	}
	rl_clear_history();
	handle_signal(SIGTERM);
	return (0);
}
