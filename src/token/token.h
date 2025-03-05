/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:52 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/05 18:16:54 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "libft.h"
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

enum		e_state
{
	NORMAL,
	IN_SQUOTE,
	IN_DQUOTE
};


enum		e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR,
	T_LOGIC,
	T_EOF
}

typedef struct	s_token
{
	char			*str;
	enum e_type		type;
	struct s_token	*next;
}	t_token


#endif
