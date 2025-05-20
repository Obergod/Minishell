/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:47:55 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 13:37:49 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"
# include "parsing.h"

enum					e_node_type
{
	NODE_NONE,
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_OPEN_PARENTHESIS,
	NODE_CLOSE_PARENTHESIS
};

typedef struct s_ast_node
{
	enum e_node_type	type;
	t_cmd				*cmd;
	int					subshell;
	t_redir				*subshell_redir;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*parent;
}						t_ast_node;

/* Fonctions de l'AST */
t_ast_node				*init_ast_node(enum e_node_type type, t_cmd *cmd,
							t_minishell *minishell);
t_ast_node				*build_ast(t_cmd *cmd_list, t_minishell *minishell);
int						check_parenthesis_balance(t_cmd *cmd_list);
void					print_ast(t_ast_node *node, int depth);
t_ast_node				*build_ast_recursive(t_cmd **cmd_list,
							t_minishell *minishell, int *paren_count);
t_ast_node				*build_ast_recursive_loop(t_ast_node *left,
							t_cmd **cmd_list, t_minishell *minishell,
							int *paren_count);
t_ast_node				*build_expr_parenthesis(t_cmd **cmd_list,
							t_minishell *minishell, int *paren_count);
t_ast_node				*build_expr_cmd(t_cmd **cmd_list,
							t_minishell *minishell);
t_ast_node				*build_expr(t_cmd **cmd_list, t_minishell *minishell,
							int *paren_count);
int						is_invalid_cmd_list(t_cmd *cmd_list);
t_ast_node				*init_pipe_node(t_ast_node *left, t_cmd **cmd_list,
							t_minishell *minishell, int *paren_count);
t_ast_node				*handle_pipes(t_cmd **cmd_list, t_minishell *minishell,
							int *paren_count);
t_ast_node				*init_logic_node(t_ast_node *left, t_cmd **cmd_list,
							t_minishell *minishell, int *paren_count);
t_ast_node				*build_ast(t_cmd *cmd_list, t_minishell *minishell);
void							ast_heredoc(t_ast_node *node,
									t_minishell *minishell);
void							print_heredoc(t_ast_node *node,
									t_minishell *minishell);
#endif
