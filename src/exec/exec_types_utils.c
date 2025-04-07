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



















