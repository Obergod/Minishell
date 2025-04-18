/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:06:08 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 16:49:18 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/readline.h"
#include "../../full_libft/include/libft.h"
#include <stdio.h>

static char	*ft_add_readline(const char *prompt, char **stock)
{
	(*stock) = readline(prompt);
	if (*stock == NULL)
		return (NULL);
	return (*stock);
}

void test_builtin(char *input, t_minishell *minishell)
{
	char **args = gc_split(input, ' ', minishell->gc);
	if (!args || !args[0])
		return;
	if (ft_strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		ft_cd(args, minishell);
	else if (ft_strcmp(args[0], "env") == 0)
		ft_env(minishell);
	else if (ft_strcmp(args[0], "export") == 0)
		ft_export(args, minishell);
	else if (ft_strcmp(args[0], "unset") == 0) 
		ft_unset(args, minishell);
	else if (ft_strcmp(args[0], "exit") == 0)
		ft_exit(args, minishell);
	else if (ft_strcmp(args[0], "ls") == 0)
	{
		if (access("/bin/ls", X_OK) == 0)
			system("/bin/ls");
		else
			printf("'ls' n'est pas accessible\n");
	}
	else
		printf("'%s' n'est pas un builtin\n", args[0]);
}

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;

	(void)ac;
	(void)av;
	// char **test = { NULL };
	// envp = test;

	// Initialisation du garbage collector
	gc_head = gc_init();
	if (!gc_head)
	{
		printf("Erreur: Échec de l'initialisation du garbage collector\n");
		return (1);
	}
	minishell.gc = gc_head;

	// Initialisation de l'environnement
	minishell.env = env_parsing(envp, &minishell);
	if (!minishell.env)
	{
		minishell.env = init_env(&minishell);
		t_env *env = minishell.env;
		while (env)
		{
			printf("%s\n", env->raw);
			env = env->next;
		}
	}
	char *input;
	while (ft_add_readline(PROMPT, &input) != NULL)
	{
		if (*input)
			add_history(input);
		test_builtin(input, &minishell);
		free(input);
	}
	rl_clear_history();
	return (0);
}
