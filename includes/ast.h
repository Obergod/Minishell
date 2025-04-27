/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:47:55 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/25 18:58:10 by ufalzone         ###   ########.fr       */
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
	t_redir *subshell_redir;
	struct s_ast_node *left;
    struct s_ast_node *right;

} t_ast_node;

/* Fonctions de l'AST */
t_ast_node *init_ast_node(enum node_type type, t_cmd *cmd, t_minishell *minishell);
t_ast_node *build_ast(t_cmd *cmd_list, t_minishell *minishell);
t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell, int *parenthesis_count);
void print_ast(t_ast_node *node, int depth);

/* Fonctions du visualiseur d'AST */
void print_ast_compact(t_ast_node *node);
void print_ast_with_commands(t_ast_node *node, int depth);
void print_ast_execution_order(t_ast_node *node);
void visualize_ast(t_ast_node *root, int mode);

#endif
