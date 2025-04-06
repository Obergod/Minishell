/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:03:56 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 20:03:56 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

void	prefix_exec(t_ast_node *node, t_minishell *minishell)
{
	if (!node)
		return ;
	process(node, minishell);
	prefix_exec(node->left, minishell);
	prefix_exec(node->right, minishell);
}


void	process(t_ast_node *node, t_minishell *minishell)
{
	if (node->type == NODE_PIPE)
		exec_pipes(node, minishell);
	else if (node->type == NODE_CMD)
		exec_cmd(node, minishell);
}

int	exec_pipes(t_ast_node *node, t_minishell *minishell)
{
	t_pipe	pipes;

	if (pipe(pipes.pipes) == -1)
		exit(EXIT_FAILURE);
	pipes.pid_l = fork();
	if (pipes.pid_l < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}	
	else if (pipes.pid_l == 0)
	{
		close(pipes.pipes[0]);
		dup2(pipes.pipes[1], STDOUT_FILENO);
		close(pipes.pipes[1]);
prefix_exec(node->left, minishell);
		exit(EXIT_SUCCESS);
	}
	pipes.pid_r = fork();
	if (pipes.pid_r < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}	
	else if (pipes.pid_r == 0)
	{
		close(pipes.pipes[1]);
		dup2(pipes.pipes[0], STDIN_FILENO);
		close(pipes.pipes[0]);
		prefix_exec(node->right, minishell);
		exit(EXIT_SUCCESS);
	}
	close(pipes.pipes[0]);
	close(pipes.pipes[1]);
	waitpid(pipes.pid_l, &pipes.exit_status_l, 0);
	waitpid(pipes.pid_r, &pipes.exit_status_r, 0);
	return(WEXITSTATUS(pipes.exit_status_r));
}

char	*try_path(char **cmd_path, char *cmd, t_minishell *minishell)
{
	int		i;
	char	*full_path;

	i = -1;
	while (cmd_path[++i])
	{
		full_path = gc_strjoin_three(cmd_path[i], "/", cmd, minishell->gc);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
	}
	return (NULL);
}

char	*get_cmd_path(t_minishell *minishell, char *cmd)
{
	char	**cmd_path;
	t_env	*env;
	char *value;
	
	env = minishell->env;
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (gc_strdup(cmd, minishell->gc));
		return (NULL);
	}
	value = find_in_env("PATH", minishell);
	//if (!value)
	//	cmd_path[0] = gc_strdup("/usr/bin/", minishell->gc);
	//cmd_path[1] = NULL;
		//PATH n'existe pas donc surement env -i'
	cmd_path = gc_split(value, ':', minishell->gc);
	if (!cmd_path)
		return (NULL);
	return (try_path(cmd_path, cmd, minishell));
}

int	exec_cmd(t_ast_node *node, t_minishell *minishell)
{
	node->cmd->cmd_path = get_cmd_path(minishell, node->cmd->command[0]);
	if (!node->cmd->cmd_path)
		return (-1);
	execve(node->cmd->cmd_path, node->cmd->command, NULL);
	perror("exec failed");
	return (1);
}


















