/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:24:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/21 16:55:34 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../full_libft/include/libft.h"
#include "token.h"
#include <string.h>

/****		A trier		****/
t_token	*expand_vars(t_token *token, t_minishell *minishell);
char	*expand_str(char *str, t_minishell *minishell);
char	*get_vars(char *str, int *i, t_minishell *minishell);
int		get_full_len(char *str, t_minishell *minishell);

#endif
