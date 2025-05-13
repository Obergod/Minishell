/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:02:37 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/14 18:02:41 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

void	close_pipes(int *pipes)
{
	close(pipes[0]);
	close(pipes[1]);
}

void	close_fds(int *fd_in, int *fd_out)
{
	if (*fd_in != -1)
		close(*fd_in);
	if (*fd_out != -1)
		close(*fd_out);
	*fd_in = -1;
	*fd_out = -1;
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
	return (WEXITSTATUS(status));
}
