/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:17:02 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/05 18:17:03 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*tokenize(const char *input)
{
	int				i;
	t_token			*token;
	enum e_state	state;
	int				nb_tok;
	char			*buff;

	i = -1;
	nb_tok = 0;
	buff = malloc(sizeof(char) * ft_strlen(input));
	token = NULL;
	state = NORMAL	;
	while (input[++i])
	{
		if (state == NORMAL)
		{
			if (input[i] == ' ' || input[i] == '\t')
			{
				if (nb_tok > 0)
				{
					buff[nb_tok] = '\0';
					add_token(*token, buff, T_WORD);
					nb_tok = 0;
				}
			}
			else if (input[i] == '"')
				state = IN_DQUOTE;
			else if (input[i] == '\'')
				state = IN_SQUOTE;
			else if (is_operator(input[i]))
			{
				if (nb_tok > 0)
				{
					buff[nb_tok] = '\0';
					add_token(*token, buff, T_WORD);
					nb_tok = 0;
				}
				else
				{
					token->type = handle_operator(input, &i);
					add_token(*token, NULL, token->type);
				}
			}
			else
				buff[nb_tok++] = input[i];
		}
		else if (state == IN_SQUOTE)
		{
			if (input[i] == '\'')
				state = NORMAL;
			else
				buff[nb_tok++] = input[i];
		}
		else if (state == IN_DQUOTE)
		{
			if (input[i] == '"')
				state = NORMAL;
			else
				buff[nb_tok++] = input[i];
		}
	}
}

enum e_token_type	handle_operator(char *input, int *i)
{
	char	op;
	char	next_char;

	op = input[*i];
	next_char = input[*i + 1];
	if (op == '<')
	{
		if (next_char == '<')
			*i++;
		return (T_REDIR);
	}
	else if (op == '>')
	{
		if (next_char == '>')
			*i++;
		return (T_REDIR)
	}
	else if (op == '|')
	{
		if (next_char == '|')
		{
			*i++;
			return (T_LOGIC);
		}
		return (T_PIPE);
	}
	else if (op == '&')
	{
		if (next_char == '&')
		{
			*i++;
			return (T_LOGIC);
		}
//		exit_and_cleanup;
	}
}

void	add_back(t_token **token, char *buff, enum e_token_type type)
{
	t_token	*new_token;
	t_token	*tmp;
	char	op[2];

	new_token = malloc(sizeof(t_token));
	if (!new_token)
//		clean_up_and_exit();
	new_token->type = type;
	new_token->next = NULL;
	if (type == T_WORD)
	{
		new_token->str = ft_strdup(buff);
		if (!new_token->str)
//			clean_up_and_exit;
	else
	//	handle_other_str();
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


