/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:52 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/21 16:55:41 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <string.h>
# include "minishell.h"
# include "env_parsing.h"
# include "garbage_collector.h"

extern int g_exit_status;

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
};

typedef struct	s_token
{
	char				*str;
	enum e_token_type	type;
	enum e_state		state;
	struct s_token		*next;
}	t_token;

/*****		A trier			*****/
t_token	*tokenize(const char *input, t_minishell *minishell);



/*****		token_utils		*****/
int					is_operator(char c);
void				operator_str(const char *input, char *buff, int i);
enum e_token_type	handle_operator(const char *input, int *i);
void	add_token(t_token **token, char *buff, enum e_token_type type, enum e_state state, t_minishell *minishell);
int					quotes_verif(const char *input);

#endif
