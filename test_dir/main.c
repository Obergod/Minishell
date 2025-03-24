/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:59:20 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/24 13:59:24 by mafioron         ###   ########.fr       */
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

int	ft_readline(void)
{
	char	*input;

	rl_catch_signals = 0; // Desactive la gestion auto des signaux de readline
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (ft_add_readline(PROMPT, &input) != NULL)
	{
		if (*input)
			add_history(input);
		printf("Commande entrée: %s\n", input);
		free(input);
	}
	rl_clear_history();
	handle_signal(SIGTERM);
	return (0);
}

int	init_minishell(t_minishell *minishell, char **envp)
{
	t_gc_head	*gc_head;

	gc_head = gc_init();
	minishell->gc = gc_head;
	
	// Initialize environment
	minishell->env = env_parsing(envp, minishell);
	if (!minishell->env) {
		printf("Error: Failed to parse environment\n");
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

void	print_parsing(t_cmd *cmd_head)
{
	while (cmd_head)
	{
		printf("Command_raw :%s\n", cmd_head->command_raw);
		for (int i=0;cmd_head->command[i];i++)
			printf("[%d] Command: %s\n", i, cmd_head->command[i]);
		printf("Infile: %s\n", cmd_head->infile);
		printf("Outfile: %s\n", cmd_head->outfile);
		printf("Append: %d\n", cmd_head->append);
		printf("Heredoc: %s\n", cmd_head->heredoc);
		printf("\n");
		cmd_head = cmd_head->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_token 	*token;
	t_minishell	minishell;
	t_cmd		*cmd_head;

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
			print_token(token);
		else
			printf("Error: Failed to tokenize input\n");
		cmd_head = parsing(token, &minishell);	
		if (cmd_head)
			print_parsing(cmd_head);
		else
			printf("Error : parsing failed\n");
		if (*input)
			add_history(input);
		free(input);
	}
	rl_clear_history();
	handle_signal(SIGTERM);
	return (0);
}
