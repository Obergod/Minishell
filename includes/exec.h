/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:04:08 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/15 18:55:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../full_libft/include/get_next_line.h"
# include <stdlib.h>
# include <sys/wait.h>

typedef struct s_pipe
{
	int		pipes[2];
	pid_t	pid_l;
	pid_t	pid_r;
	int		exit_status_l;
	int		exit_status_r;
}			t_pipe;

typedef struct s_wait
{
	pid_t	pid;
	int		status;
}			t_wait;

typedef struct s_fds
{
	int		fd_in;
	int		fd_out;
}			t_fds;

/*****		exec.c		*******/
void		prefix_exec(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);
void		process(t_ast_node *node, t_ast_node *head, t_minishell *minishell);
t_redir		*find_redirections_in_ast(t_ast_node *node);
int			handle_builtins(t_ast_node *node, t_minishell *minishell);

/*****		exec_process.c	*****/
void		handle_child_process(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell, t_redir *redirections);
void		handle_subshell(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);
void		handle_cmd_process(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);
void		handle_command_not_found(t_ast_node *node, t_minishell *minishell);

/*****		exec_types.c		*****/
int			exec_pipes(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);
int			exec_cmd(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);
int			exec_log_operators(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell);

/*****		builtin.c	*****/
int			exec_builtins(t_ast_node *node, t_minishell *minishell);
int			exec_fork_builtins(t_ast_node *node, t_minishell *minishell);
int			is_builtin(t_ast_node *node);
int			is_fork_builtin(t_ast_node *node);

/*****		exec_utils.c		*****/
int			is_cmd(t_ast_node *node);
int			skip_cmd(t_ast_node *node);
int			is_only_space(char *str);
char		**check_empty(char **cmd, t_minishell *minishell);

/*****		file_utils.c		*****/
char		*try_path(char **cmd_path, char *cmd, t_minishell *minishell,
				int *err);
char		*get_cmd_path(t_minishell *minishell, char *cmd, int *err);
int			check_file_accesss(char *file, int in_out);
int			check_cmd_access(char *cmd);

/*****		fd_utils.c		*****/
void		close_pipes(int *pipes);
void		close_fds(int *fd_in, int *fd_out);
int			wait_and_signal(pid_t pid, int status, t_minishell *minishell);

/*****		redirs.c		*****/
int			handle_redir(t_ast_node *node, t_minishell *minishell, int *fd_in,
				int *fd_out);
int			handle_input(t_redir *redir, int *fd_in);
int			handle_output(t_redir *redir, int *fd_out);

/*****		heredoc.c		*****/
int			here_doc(char *delimiter, t_minishell *minishell);
int			handle_heredoc(t_redir *redir, int *fd_in, t_minishell *minishell);

#endif
