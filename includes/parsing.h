/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/06 19:14:10 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include <stdlib.h>
#include "garbage_collector.h"
#include "token.h"
// Forward declaration of t_minishell to prevent circular dependency
typedef struct s_minishell t_minishell;

enum	e_logic_operator_type
{
	NONE,
	PIPE,   // Ajout explicite du type PIPE
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
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


//ls -> | -> grep -e "salut"
typedef struct s_cmd
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"] [NULL]
	char *cmd_path;
	size_t _arg_count;
	size_t _arg_capacity;
	enum e_logic_operator_type logic_operator_type; //&& || ( )
	struct s_redir *redirs; // liste des redirections
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

#endif
