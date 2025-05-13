/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:00:33 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/13 15:55:50 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include "../full_libft/include/ft_printf.h"
# include "ast.h"
# include "builtin.h"
# include "env_parsing.h"
# include "exec.h"
# include "expand.h"
# include "garbage_collector.h"
# include "minishell.h"
# include "parsing.h"
# include "readline.h"
# include "token.h"
# include "wildcard.h"
# include <errno.h>

# define PROMPT "pizzshell-5.2$ "

extern int	g_sig_received;
/* Function declarations for main */

#endif
