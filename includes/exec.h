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
#include "../full_libft/include/get_next_line.h"

typedef struct	s_pipe
{
	int	pipes[2];
	pid_t	pid_l;
	pid_t	pid_r;
	int		exit_status_l;
	int		exit_status_r;
}	t_pipe;

/*****		A trier		*******/
void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell);
void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell);

/*****		exec_types		*****/
int		exec_pipes(t_ast_node *node, t_ast_node *head, t_minishell *minishell);
int		exec_cmd(t_ast_node *node, t_ast_node *head, t_minishell *minishell);
int		exec_log_operators(t_ast_node *node, t_ast_node *head, t_minishell *minishell);
int		exec_builtins(t_ast_node *node, t_minishell *minishell);
int		exec_fork_builtins(t_ast_node *node, t_minishell *minishell);

/*****		exec_utils		*****/
char	*try_path(char **cmd_path, char *cmd, t_minishell *minishell, int *err);
char	*get_cmd_path(t_minishell *minishell, char *cmd, int *err);
int		handle_redir(t_ast_node *node, t_minishell *minishell, int *fd_in, int *fd_out);
int		is_builtin(t_ast_node *node);
int		is_fork_builtin(t_ast_node *node);
int		is_cmd(t_ast_node *node);
int		skip_cmd(t_ast_node *node);
int		is_only_space(char *str);

/****		errors		*******/
int		check_file_accesss(char *file, int in_out);
int		check_cmd_access(char *cmd);

#endif
