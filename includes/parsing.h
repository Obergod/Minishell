/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 16:58:15 by ufalzone         ###   ########.fr       */
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

t_cmd *parsing(t_token *token, t_minishell *minishell);
void	add_arg_to_cmd(t_cmd *cmd, char *str, t_gc_head *gc);
void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *current);
void	t_logic_parsing(t_token *token, t_cmd **current_cmd);
t_cmd	*parsing(t_token *token, t_minishell *minishell);
t_cmd	*new_cmd(t_minishell *minishell);
void	add_arg_to_cmd(t_cmd *cmd, char *str, t_gc_head *gc);
void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *current);
enum error_parsing	check_parsing(t_token *token_p);
enum error_parsing	check_parenthesis(t_token *token_p);
enum error_parsing	start_end_check_parsing(t_token *token, int status);
enum error_parsing	print_error(enum error_parsing error_code);
t_redir	*new_redir(enum e_redir_type type, char *file_or_delimiter,
		t_gc_head *gc);
void	add_redir_to_cmd(t_cmd *cmd, t_redir *redir);
void	t_redir_parsing(t_token *token, t_cmd **current_cmd,
		t_minishell *minishell);

#endif
