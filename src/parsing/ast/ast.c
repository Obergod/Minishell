/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:20:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 19:25:32 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node *build_ast(t_cmd *cmd_list, t_minishell *minishell)
{
	int paren_count;
	t_cmd *current;
	t_ast_node *root;

	if (is_invalid_cmd_list(cmd_list))
		return (NULL);
	paren_count = 0;
	current = cmd_list;
	root = build_ast_recursive(&current, minishell, &paren_count);
	if (paren_count != 0)
		return (NULL);
	return (root);
}