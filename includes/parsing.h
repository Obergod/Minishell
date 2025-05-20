/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:04:32 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/19 19:29:33 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "garbage_collector.h"
# include "token.h"
# include <stdlib.h>

enum							e_logic_operator_type
{
	NONE,
	PIPE,
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
};

enum							e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
};

typedef struct s_redir
{
	enum e_redir_type			type;
	char						*file_or_delimiter;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						*command_raw;
	char						**command;
	char						*cmd_path;
	size_t						_arg_count;
	size_t						_arg_capacity;
	enum e_logic_operator_type	logic_operator_type;
	int							is_redirect;
	struct s_redir				*redirs;
	struct s_cmd				*next;
}								t_cmd;

enum							e_error_parsing
{
	SUCCESS,
	ERR_SYNTAX_REDIRECT,
	ERR_SYNTAX_PIPE,
	ERR_SYNTAX_LOGIC,
	ERR_SYNTAX_TOKEN
};

t_cmd							*parsing(t_token *token,
									t_minishell *minishell);
void							add_cmd_to_list(t_cmd **cmd_list,
									t_cmd *current);
void							t_logic_parsing(t_token *token,
									t_cmd **current_cmd);
t_cmd							*parsing(t_token *token,
									t_minishell *minishell);
t_cmd							*new_cmd(t_minishell *minishell);
void							add_arg_to_cmd(t_cmd *cmd, char *str,
									t_gc_head *gc);
void							add_cmd_to_list(t_cmd **cmd_list,
									t_cmd *current);
enum e_error_parsing			check_parsing(t_token *token_p);
enum e_error_parsing			check_parenthesis(t_token *token_p);
enum e_error_parsing			start_end_check_parsing(t_token *token,
									int status);
enum e_error_parsing			print_error(enum e_error_parsing error_code);
t_redir							*new_redir(enum e_redir_type type,
									char *file_or_delimiter, t_gc_head *gc);
void							add_redir_to_cmd(t_cmd *cmd, t_redir *redir);
void							t_redir_parsing(t_token *token,
									t_cmd **current_cmd,
									t_minishell *minishell);
void							handle_parsing_operator(t_token *token,
									t_cmd **list, t_cmd **current,
									t_minishell *minishell);
char							**resize_command_array(t_cmd *cmd,
									t_gc_head *gc);

#endif
