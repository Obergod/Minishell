/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_types_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:52:37 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/07 18:52:41 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

char	*try_path(char **cmd_path, char *cmd, t_minishell *minishell, int *err)
{
	int		i;
	char	*full_path;

	i = -1;
	while (cmd_path[++i])
	{
		full_path = gc_strjoin_three(cmd_path[i], "/", cmd, minishell->gc);
		if (access(full_path, F_OK) == 0)
			return (full_path);
	}
	*err = check_cmd_access(cmd);
	return (NULL);
}

char	*get_cmd_path(t_minishell *minishell, char *cmd, int *err)
{
	char	**cmd_path;
	t_env	*env;
	char *value;
	
	env = minishell->env;
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		*err = check_cmd_access(cmd);
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
	return (try_path(cmd_path, cmd, minishell, err));
}

int	check_file_accesss(char *file, int in_out)
{
	if (in_out == 0)
	{
		if (access(file, F_OK | R_OK) == -1)
		{
			perror(file);
			return (1);
		}
	}
	else if (in_out == 1)
	{
		if (access(file, F_OK) == 0 &&
				access(file, W_OK) == -1)
		{
			perror(file);
			return (1);
		}
	}
	return (0);
}

int check_cmd_access(char *cmd)
{
	struct stat stat_buffer;

    if (stat(cmd, &stat_buffer) == -1) {
        if (errno == ENOENT) {
            return (127);
        } else {
            return (126);
        }
    }
    if (S_ISDIR(stat_buffer.st_mode)) {
        return (126);
    }
    if (access(cmd, X_OK) == -1) {
return (126);
    }

    return (0);
}

void	close_pipes(int *pipes)
{
	close(pipes[0]);
	close(pipes[1]);
}

void	close_fds(int *fd_in, int *fd_out)
{
	if (*fd_in)
		close(*fd_in);
	if (*fd_out)
		close(*fd_out);
}

int	wait_and_signal(pid_t pid, int status, t_minishell *minishell)
{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				minishell->exit_status = 131;
				ft_putendl_fd("Quit (core dumped)", 2);
			}
			else if (WTERMSIG(status) == SIGINT)
            	minishell->exit_status = 130;
			return (minishell->exit_status);
		}
		return(WEXITSTATUS(status));
}



















