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
#include "expand.h"

void	init_tokenizer(t_tokenizer *tok, const char *input, t_minishell *minishell)
{
	tok->i = -1;
	tok->nb_tok = 0;
	tok->buff = gc_malloc((sizeof(char) * ft_strlen(input) + 1), minishell->gc);
	tok->token_list = NULL;
	tok->state = NORMAL;
	tok->token_state = NORMAL;
}

void	process_space(t_tokenizer *tok, t_minishell *minishell)
{
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(&tok->token_list, tok->buff, T_WORD, tok->token_state, minishell);
		tok->nb_tok = 0;
		tok->token_state = tok->state;
	}
}

void	process_operator(const char *input, t_tokenizer *tok, t_minishell *minishell)
{
	enum e_token_type	type;

	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(&tok->token_list, tok->buff, T_WORD, tok->token_state, minishell);
		tok->nb_tok = 0;
		tok->token_state = tok->state;
	}
	operator_str(input, tok->buff, tok->i);
	type = handle_operator(input, &tok->i);
	add_token(&tok->token_list, tok->buff, type, NORMAL, minishell);
}

void	handle_normal_state(const char *input, t_tokenizer *tok, t_minishell *minishell)
{
	if (input[tok->i] == ' ' || input[tok->i] == '\t')
		process_space(tok, minishell);
	else if (input[tok->i] == '"')
	{
		tok->state = IN_DQUOTE;
		tok->buff[tok->nb_tok++] = input[tok->i];
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
	}
	else if (input[tok->i] == '\'')
	{
		tok->state = IN_SQUOTE;
		tok->buff[tok->nb_tok++] = input[tok->i];
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
	}
	else if (is_operator(input[tok->i]))
		process_operator(input, tok, minishell);
	else
	{
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
		tok->buff[tok->nb_tok++] = input[tok->i];
	}
}

void	handle_quotes(const char *input, t_tokenizer *tok, char quote)
{
	if (input[tok->i] == quote)
		tok->state = NORMAL;
	else
	{
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
		tok->buff[tok->nb_tok++] = input[tok->i];
	}
}

void	finalize_token(t_tokenizer *tok, t_minishell *minishell)
{
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(&tok->token_list, tok->buff, T_WORD, tok->token_state, minishell);
	}
}

t_token	*tokenize(const char *input, t_minishell *minishell)
{
	t_tokenizer	tok;
	t_token		*result;

	if (quotes_verif(input) == 1)
		return (NULL);
	init_tokenizer(&tok, input, minishell);
	while (input[++tok.i])
	{
		if (tok.state == NORMAL)
			handle_normal_state(input, &tok, minishell);
		else if (tok.state == IN_SQUOTE)
			handle_quotes(input, &tok, '\'');
		else if (tok.state == IN_DQUOTE)
			handle_quotes(input, &tok, '"');
	}
	finalize_token(&tok, minishell);
	result = tok.token_list;
	//gc_free(tok.buff, minishell->gc);
	return (result);
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
		return (T_PARANTHESIS);
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

void	add_token(t_token **token, char *buff, enum e_token_type type, enum e_state state, t_minishell *minishell)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = gc_malloc((sizeof(t_token)), minishell->gc);
//	if (!new_token)
//		clean_up_and_exit();
	new_token->type = type;
	new_token->state = state;
	new_token->next = NULL;
	new_token->str = gc_strdup((buff), minishell->gc);
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
