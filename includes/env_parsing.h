/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:53:54 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/04 17:33:25 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_PARSING_H
#define ENV_PARSING_H

#include "garbage_collector.h"
#include "minishell.h"
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>

typedef struct s_env
{
	char *key;
	char *value;
	char *raw;
	struct s_env *next;
} t_env;

/****		A trier		****/
char *find_in_env(char *key, t_minishell *minishell);
t_env *env_parsing(char **envp, t_minishell *minishell);
t_env *init_env(t_minishell *minishell);
#endif
