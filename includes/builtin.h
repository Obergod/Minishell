/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:35:19 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/05 17:25:53 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
#define BUILTIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#include "env_parsing.h"
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_cd(char **args, t_minishell *minishell);
int		ft_env(t_minishell *minishell);
int		ft_export(char **args, t_minishell *minishell);
int		ft_unset(char **args, t_minishell *minishell);
int		ft_exit(char **args);

#endif
