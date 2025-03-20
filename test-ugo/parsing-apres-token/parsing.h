/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/20 15:59:54 by ufalzone         ###   ########.fr       */
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
	size_t _arg_count;
	size_t _arg_capacity;
    char *infile; //file d'entree
	char *outfile; //file de sortie
	int append; //>> ou pas
	char *heredoc; //delimiteur
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

enum	error_parsing
{
	SUCCESS,
	ERR_SYNTAX_REDIRECT, //Redirection sans argument
	ERR_SYNTAX_PIPE, // Erreur de pipe
	ERR_SYNTAX_LOGIC, //Erreur d'operateur
	ERR_SYNTAX_TOKEN // Toutes autres erreurs
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

// Déclarations des fonctions
t_token *create_token(char *str, enum e_token_type type, t_minishell *minishell);
void add_token(t_token **head, t_token *new_token);
void print_cmd(t_cmd *cmd, int test_type);
t_cmd *parsing(t_token *token, t_minishell *minishell);
int ft_strcmp(const char *s1, const char *s2);
char **gc_split(const char *s, char c, t_minishell *minishell);
char *gc_strdup(const char *s1, t_minishell *minishell);
char *gc_strjoin_three(char *s1, char *s2, char *s3, t_minishell *minishell);
void print_cmd_simple(t_cmd *cmd);
void test_parsing_colorized(t_minishell *minishell);
void test_parsing_simple(t_minishell *minishell);

#endif
