/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:45:47 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/16 17:45:48 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"
int	is_cmd(t_ast_node *node)

{
	return(node->cmd && node->cmd->command &&
			node->cmd->command[0] && node->cmd->command[0][0]);
}

int	skip_cmd(t_ast_node *node)
{
	char	c1;

	if (!is_cmd(node) && node->type == NODE_CMD)
		return (1);
	if (is_cmd(node))
	{
		c1 = node->cmd->command[0][0];
		if (c1 == '\0')
			return (1);
		else if (c1 == ':' && node->cmd->command[0][1] == '\0')
				return (1);
		else if (c1 == '!' && node->cmd->command[0][1] == '\0')
				return (1);
	}
	return (0);
}

char	**check_empty(char **cmd, t_minishell *minishell)
{
	while (cmd && *cmd && **cmd == '\0')
			cmd++;
	 if (cmd && !*cmd)  // *cmd is NULL (no non-empty args left)
        minishell->exit_status = 0;
	return (cmd);
}
