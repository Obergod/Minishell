/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:00:33 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/25 16:32:26 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <errno.h>
#include "../full_libft/include/ft_printf.h"

# include "ast.h"
# include "minishell.h"
# include "garbage_collector.h"
# include "token.h"
# include "env_parsing.h"
# include "expand.h"
# include "readline.h"
# include "parsing.h"
# include "builtin.h"
# include "exec.h"
# include "wildcard.h"

extern int g_sig_received;
/* Function declarations for main */

#endif
