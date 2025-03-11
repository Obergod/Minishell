/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/11 15:22:42 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "../../src/garbage_collector/garbage_collector.h"

enum    e_type
{
    COMMAND,
    PIPE,
    OPERATOR,
};

enum    e_pipe
{
    START,
    MID,
    END,
};

enum    e_state
{
    NORMAL,
    IN_QUOTES,
    IN_DQUOTES
};

typedef struct s_env
{
	char *key;
	char *value;
	char *raw;
	struct s_env *next;
} t_env;

typedef struct s_cmd
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"]
    int numbr_of_params;
    enum e_type type;
    enum e_pipe pipe;
    struct s_cmd *prev;
	struct s_cmd *next;
} t_cmd;

typedef struct s_minishell
{
	t_env *env;
	t_cmd *cmd;
	t_gc_head *gc;
} t_minishell;

#endif
