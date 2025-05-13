/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:24:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/24 20:25:43 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../full_libft/include/libft.h"
# include "main.h"
# include <string.h>

/****		A trier		****/
char	**expand_vars(char **cmd, t_minishell *minishell);
char	*expand_str(char *str, t_minishell *minishell);
char	*get_vars(char *str, int *i, t_minishell *minishell);
int		get_full_len(char *str, t_minishell *minishell);
char	*remove_quotes(char *str, t_minishell *minishell);
int		remove_quotes_redirs(t_redir *redirs, t_minishell *minishell);

#endif
