/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:01:39 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/09 20:01:40 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	is_builtin(t_ast_node *node)
{
	if (node->cmd && node->cmd->command && node->cmd->command[0])
	{
		if (!ft_strcmp(node->cmd->command[0], "export"))
			return (1);
		else if (!ft_strcmp(node->cmd->command[0], "unset"))
			return (1);
		else if (!ft_strcmp(node->cmd->command[0], "exit"))
			return (1);
		else if (!ft_strcmp(node->cmd->command[0], "cd"))
			return (1);
	}
	return (0);
}

int	is_fork_builtin(t_ast_node *node)
{
	if (node->cmd && node->cmd->command && node->cmd->command[0])
	{
		if (!ft_strcmp(node->cmd->command[0], "echo"))
			return (1);
		else if (!ft_strcmp(node->cmd->command[0], "env"))
			return (1);
		else if (!ft_strcmp(node->cmd->command[0], "pwd"))
			return (1);
	}
	return (0);
}

int	exec_builtins(t_ast_node *node, t_minishell *minishell)
{
	if (node->cmd && node->cmd->command && node->cmd->command[0])
	{
		if (!ft_strcmp(node->cmd->command[0], "export"))
			return (ft_export(node->cmd->command, minishell));
		else if (!ft_strcmp(node->cmd->command[0], "unset"))
			return (ft_unset(node->cmd->command, minishell));
		else if (!ft_strcmp(node->cmd->command[0], "exit"))
			return (ft_exit(node->cmd->command, minishell));
		else if (!ft_strcmp(node->cmd->command[0], "cd"))
			return (ft_cd(node->cmd->command, minishell));
	}
	return (0);
}

int	exec_fork_builtins(t_ast_node *node, t_minishell *minishell)
{
	if (node->cmd && node->cmd->command && node->cmd->command[0])
	{
		if (!ft_strcmp(node->cmd->command[0], "echo"))
			return (ft_echo(node->cmd->command));
		else if (!ft_strcmp(node->cmd->command[0], "env"))
			return (ft_env(minishell));
		else if (!ft_strcmp(node->cmd->command[0], "pwd"))
			return (ft_pwd());
	}
	return (0);
}
