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

# include "../../full_libft/include/libft.h"
#include <string.h>

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
	T_OTHER,
	T_EOF
};

typedef struct	s_token
{
	char			*str;
	enum e_token_type		type;
	struct s_token	*next;
}	t_token;

/*****		A trier			*****/
t_token	*tokenize(const char *input);



/*****		token_utils		*****/
int					is_operator(char c);
char				*operator_str(const char *input, int i);
enum e_token_type	handle_operator(const char *input, int *i);
void				add_token(t_token **token, char *buff, enum e_token_type type, int i);

#endif
