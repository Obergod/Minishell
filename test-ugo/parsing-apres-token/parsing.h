/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/11 16:45:38 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "../../src/garbage_collector/garbage_collector.h"
#include <stdio.h>

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

typedef struct s_cmd //
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"] [NULL]
    char *infile; //file d'entree
	char *outfile; //file de sortie
	int append; //>> ou pas
	char *heredoc; //delimiteur
	struct s_cmd *prev;
	struct s_cmd *next; //des que ya un pipe on passe au prochain
} t_cmd;

enum	e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR,
	T_LOGIC,
	T_OTHER
};

typedef struct s_token
{
	char *str;
	enum e_token_type type;
	struct s_token *next;
} t_token;

typedef struct s_minishell
{
	t_env *env;
	t_cmd *cmd;
	t_gc_head *gc;
} t_minishell;

#endif
