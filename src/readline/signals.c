/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:21:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 19:29:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int g_sig_received = 0;

static void	handle_signal(int sig)
{
	g_sig_received = sig;
	if (sig == SIGINT) // Ctrl+C
	{
		write(1, "^C", 1);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
//avant d'executer une commande
void reset_signals_child(void)
{
	signal(SIGINT, SIG_DFL);//remet par defaut
	signal(SIGQUIT, SIG_DFL);
}

//dit au systeme de rediriger les signaux ici
void setup_signals(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask); //clear les signaux pour pas les bloquer
	sa.sa_handler = handle_signal; //defini la fonction a appeler a la place d'executer les signaux
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL); //ctrl C
	sa.sa_handler = SIG_IGN; //ignoree
	sigaction(SIGQUIT, &sa, NULL);
}
