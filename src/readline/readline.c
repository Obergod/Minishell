/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:46:05 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 15:27:38 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/readline.h"

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
	signal(SIGQUIT, handle_signal);
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

// int main(void)
// {
// 	ft_readline();
// 	return (0);
// }
