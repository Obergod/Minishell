/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:53:40 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/12 20:22:55 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# include "minishell.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>

void	reset_signals_child(void);
void	update_exit_status_from_signal(t_minishell *minishell);
void	exec_setup_signals(void);
void	interactive_setup_signals(void);
char	*ft_add_readline(const char *prompt, char **stock,
			t_minishell *minishell);
void	handle_interactive_signal(int sig);

#endif
