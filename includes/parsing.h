/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/24 14:38:07 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "minishell.h"
#include "garbage_collector.h"
#include <stdlib.h>
#include "token.h"

enum	e_logic_operator_type
{
	NONE,
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS
};

enum	e_redir_type
{
	REDIR_IN,   // <
	REDIR_OUT,  // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
};

typedef struct s_redir
{
	enum e_redir_type type;
	char *file_or_delimiter;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"] [NULL]
	size_t _arg_count;
	size_t _arg_capacity;
	enum e_logic_operator_type logic_operator_type; //&& || ( )
	struct s_redir *redirs; // Liste des redirections
	struct s_cmd *next; //des que ya un pipe on passe au prochain
} t_cmd;


enum	error_parsing
{
	SUCCESS,
	ERR_SYNTAX_REDIRECT, //Redirection sans argument
	ERR_SYNTAX_PIPE, // Erreur de pipe
	ERR_SYNTAX_LOGIC, //Erreur d'operateur
	ERR_SYNTAX_TOKEN // Toutes autres erreurs
};

// Déclarations des fonctions
t_cmd *parsing(t_token *token, t_minishell *minishell);
t_redir *new_redir(enum e_redir_type type, char *file_or_delimiter, t_gc_head *gc);
void add_redir_to_cmd(t_cmd *cmd, t_redir *redir);

#endif
