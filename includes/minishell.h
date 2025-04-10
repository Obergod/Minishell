/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:16:18 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/06 18:24:06 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../full_libft/include/libft.h"

// Forward declarations
struct s_env;
struct s_cmd;
struct s_gc_head;

typedef struct s_minishell
{
	struct s_env *env;
	struct s_cmd *cmd;
	struct s_gc_head *gc;
	int exit_status;
} t_minishell;

#endif
