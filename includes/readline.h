/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:53:40 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/25 17:51:41 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
#define READLINE_H

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#define PROMPT "bash-5.2 : "

int	ft_readline(t_minishell *minishell);
void reset_signals_child(void);
void update_exit_status_from_signal(t_minishell *minishell);
void exec_setup_signals(void);
void interactive_setup_signals(void);

#endif
