/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:21:46 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/19 15:52:15 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"
#include <unistd.h>

int	init_minishell(t_minishell *minishell, char **envp)
{
	t_gc_head	*gc_head;

	minishell->exit_status = 0;
	gc_head = gc_init();
	if (!gc_head)
	{
		printf("Error: Garbage not initialized.\n");
		return (1);
	}
	minishell->gc = gc_head;
	minishell->env = env_parsing(envp, minishell);
	if (!minishell->env)
	{
		printf("Error: Failed to parse environment\n");
		gc_destroy(gc_head);
		return (1);
	}
	minishell->env_array = convert_t_env_to_array(minishell);
	if (!minishell->env_array)
	{
		return (1);
	}
	return (0);
}

static char	*get_input(t_minishell *minishell)
{
	char	*input;

	if (isatty(STDIN_FILENO))
		ft_add_readline(PROMPT, &input, minishell);
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			return (NULL);
	}
	return (input);
}

void	main_loop(t_minishell *minishell)
{
	t_ast_node	*ast;
	t_ast_node	*head;
	char		*input;

	input = NULL;
	while (1)
	{
		update_exit_status_from_signal(minishell);
		input = get_input(minishell);
		if (!input && isatty(STDIN_FILENO))
			clean_exit(1, minishell) ;
		ast = build_ast(parsing(tokenize(input, minishell), minishell),
				minishell);
		head = ast;
		prefix_exec(ast, head, minishell);
		update_exit_status_from_signal(minishell);
		if (*input)
			add_history(input);
		if (input)
			free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	minishell;

	if (ac < 1 || !av || init_minishell(&minishell, envp) == 1)
		return (1);
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (1);
	else
		interactive_setup_signals();
	main_loop(&minishell);
	clean_exit(EXIT_SUCCESS, &minishell);
	return (0);
}
