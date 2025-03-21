/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:11:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "minishell.h"
#include "garbage_collector.h"
#include <stdlib.h>


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

enum	e_logic
{
	AND,
	OR
};

enum    e_state
{
    NORMAL,
    IN_QUOTES,
    IN_DQUOTES
};

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

// Déclarations des fonctions
t_token *create_token(char *str, enum e_token_type type, t_minishell *minishell);
void add_token(t_token **head, t_token *new_token);
void print_cmd(t_cmd *cmd, int test_type);
t_cmd *parsing(t_token *token, t_minishell *minishell);

#endif
