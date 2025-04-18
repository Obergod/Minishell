/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:17:04 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 15:13:16 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*
 * Rappel :
 * La commande "cd" doit être exécutée dans le processus principal (le shell lui-même).
 * Si "chdir" est appelée dans un processus enfant (par exemple, via un fork),
 * le changement de répertoire n'affectera que ce processus enfant et sera perdu une fois ce dernier terminé.
 * C'est pourquoi il est important de traiter les builtins comme "cd" directement dans le shell,
 * pour que l'appel à "chdir" modifie réellement le répertoire de travail du shell.
 */

static int access_cd(char *path, t_minishell *minishell)
{
	char *old_pwd;
	char *pwd;

	gc_alloc(old_pwd = getcwd(NULL, 0), minishell->gc);
	if (chdir(path) == -1)
	{
		perror("chdir:");
		return (1);
	}
	edit_in_env("OLDPWD", old_pwd, minishell);
	gc_alloc(pwd = getcwd(NULL, 0), minishell->gc);
	edit_in_env("PWD", pwd, minishell);
	return (0);
}

int ft_cd(char **args, t_minishell *minishell)
{
	char *target;

	if (!args || !args[0] || ft_strcmp(args[0], "cd") != 0)
		return (1);
	if (args[1])
		target = args[1];
	else
	{
		target = find_in_env("HOME", minishell);
		if (!target)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}
	return (access_cd(target, minishell));
}
