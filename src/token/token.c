/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:17:02 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/11 15:42:26 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/* Global variable is defined in test_dir/test_tok.c */
// int g_exit_status = 0;

t_token	*tokenize(const char *input)
{
	int					i;
	t_token				*token;
	enum e_state		state;
	enum e_state		token_state;
	enum e_token_type	type;
	int					nb_tok;
	char				*buff;

	if (quotes_verif(input) == 1)
		return (NULL);
	i = -1;
	nb_tok = 0;
	buff = malloc(sizeof(char) * ft_strlen(input) + 1);
	token = NULL;
	state = NORMAL;
	token_state = NORMAL;
	while (input[++i])
	{
		if (state == NORMAL)
		{
			if (input[i] == ' ' || input[i] == '\t')
			{
				if (nb_tok > 0)
				{
					buff[nb_tok] = '\0';
					add_token(&token, buff, T_WORD, token_state);
					nb_tok = 0;
					token_state = state;
				}
			}
			else if (input[i] == '"')
			{
				state = IN_DQUOTE;
				if (nb_tok == 0)
					token_state = state;
			}
			else if (input[i] == '\'')
			{
				state = IN_SQUOTE;
				if (nb_tok == 0)
					token_state = state;
			}
			else if (is_operator(input[i]))
			{
				if (nb_tok > 0)
				{
					buff[nb_tok] = '\0';
					add_token(&token, buff, T_WORD, token_state);
					nb_tok = 0;
					token_state = state;
				}
				operator_str(input, buff, i);
				type = handle_operator(input, &i);
				add_token(&token, buff, type, NORMAL);
			}
			else
			{
				if (nb_tok == 0)
					token_state = state;
				buff[nb_tok++] = input[i];
			}
		}
		else if (state == IN_SQUOTE)
		{
			if (input[i] == '\'')
				state = NORMAL;
			else
			{
				if (nb_tok == 0)
					token_state = state;
				buff[nb_tok++] = input[i];
			}
		}
		else if (state == IN_DQUOTE)
		{
			if (input[i] == '"')
				state = NORMAL;
			else
			{
				if (nb_tok == 0)
					token_state = state;
				buff[nb_tok++] = input[i];
			}
		}
	}
	if (nb_tok > 0)
	{
		buff[nb_tok] = '\0';
		add_token(&token, buff, T_WORD, token_state);
	}
	free(buff);
	return (token);
}

void	operator_str(const char *input, char *buff, int i)
{
	if (is_operator(input[i + 1]))
	{
		buff[0] = input[i];
		buff[1] = input[i + 1];
		buff[2] = '\0';
	}
	else
	{
		buff[0] = input[i];
		buff[1] = '\0';
	}
}


enum e_token_type	handle_operator(const char *input, int *i)
{
	char	op;
	char	next_char;

	op = input[*i];
	next_char = input[*i + 1];
	if (op == '<')
	{
		if (next_char == '<')
			(*i)++;
		return (T_REDIR);
	}
	else if (op == '>')
	{
		if (next_char == '>')
			(*i)++;
		return (T_REDIR);
	}
	else if (op == '|')
	{
		if (next_char == '|')
		{
			(*i)++;
			return (T_LOGIC);
		}
		return (T_PIPE);
	}
	else if (op == '(' || op == ')')
		return (T_LOGIC);
	else if (op == '&')
	{
		if (next_char == '&')
		{
			(*i)++;
			return (T_LOGIC);
		}
//		exit_and_cleanup;
	}
	return (-1);
}

void	add_token(t_token **token, char *buff, enum e_token_type type, enum e_state state)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = malloc(sizeof(t_token));
//	if (!new_token)
//		clean_up_and_exit();
	new_token->type = type;
	new_token->state = state;
	new_token->next = NULL;
	new_token->str = ft_strdup(buff);
//		if (!new_token->str)
//			clean_up_and_exit;
	if (*token == NULL)
		*token = new_token;
	else
	{
		tmp = *token;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}
