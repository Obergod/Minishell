/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:21:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/12 20:23:57 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"
#include <unistd.h>

int			g_sig_received = 0;

void	handle_interactive_signal(int sig)
{
	g_sig_received = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	handle_exec_signal(int sig)
{
	g_sig_received = sig;
	write(1, "\n", 1);
}

// appeler ca a chaque readline
void	update_exit_status_from_signal(t_minishell *minishell)
{
	if (g_sig_received == SIGINT)
	{
		minishell->exit_status = 130;
		g_sig_received = 0;
	}
}

// dit au systeme de rediriger les signaux ici
void	exec_setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_exec_signal;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
