/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:47:55 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/25 16:24:06 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
#define AST_H

#include "minishell.h"
#include "parsing.h"

enum node_type
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
    enum node_type type;
	t_cmd *cmd;
	int subshell;
	struct s_ast_node *left;
    struct s_ast_node *right;

} t_ast_node;

t_ast_node	*create_node(enum node_type type, t_minishell *minishell);
t_ast_node	*create_command_node(t_cmd *cmd, t_minishell *minishell);
t_ast_node	*build_ast(t_cmd *cmd_list, t_minishell *minishell);

#endif
