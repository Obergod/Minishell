/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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

#endif
