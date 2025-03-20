/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:24:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/20 19:24:19 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../../full_libft/include/libft.h"
#include <string.h>

/****		A trier		****/
t_token	*expand_vars(t_token *token);
char	*expand_str(char *str);
char	*get_vars(char *str, int *i);
int	get_full_len(char *str);


#endif
