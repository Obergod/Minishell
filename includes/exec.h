/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:04:08 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 20:04:09 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <stdlib.h>
# include <sys/wait.h>

typedef struct	s_pipe
{
	int	pipes[2];
	pid_t	pid_l;
	pid_t	pid_r;
	int		exit_status_l;
	int		exit_status_r;
}	t_pipe;

/*****		A trier		*******/
void	prefix_exec(t_ast_node *node, t_minishell *minishell);
void	process(t_ast_node *node, t_minishell *minishell);
int	exec_pipes(t_ast_node *node, t_minishell *minishell);

#endif
