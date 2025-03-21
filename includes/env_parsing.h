/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:include/expand.h
/*   expand.h                                           :+:      :+:    :+:   */
========
/*   env_parsing.h                                      :+:      :+:    :+:   */
>>>>>>>> main:includes/env_parsing.h
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<<< HEAD:include/expand.h
/*   Created: 2025/03/20 19:24:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/20 19:24:19 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../../full_libft/include/libft.h"
#include <string.h>

/****		A trier		****/
t_token	*expand_vars(t_token *token);
char	*expand_str(char *str);
char	*get_vars(char *str, int *i);
int	get_full_len(char *str);

========
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 15:25:55 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

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
>>>>>>>> main:includes/env_parsing.h

#endif
