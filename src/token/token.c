/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:17:02 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 19:50:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/token.h"

void	process_space(t_tokenizer *tok, t_minishell *minishell)
{
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(tok, T_WORD, tok->token_state, minishell);
		tok->nb_tok = 0;
		tok->token_state = tok->state;
	}
}

int	handle_normal_state(const char *input, t_tokenizer *tok,
		t_minishell *minishell)
{
	if (input[tok->i] == '\'')
		tok->state = IN_SQUOTE;
	if (input[tok->i] == '"')
		tok->state = IN_DQUOTE;
	if (input[tok->i] == ' ' || input[tok->i] == '\t')
		process_space(tok, minishell);
	else if (is_operator(input[tok->i]))
	{
		if (process_operator(input, tok, minishell) == 1)
			return (1);
	}
	else
	{
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
		tok->buff[tok->nb_tok++] = input[tok->i];
	}
	return (0);
}

void	handle_quotes(const char *input, t_tokenizer *tok, char quote)
{
	if (input[tok->i] == quote)
		tok->state = NORMAL;
	if (tok->nb_tok == 0)
		tok->token_state = tok->state;
	tok->buff[tok->nb_tok++] = input[tok->i];
}

t_token	*tokenize(const char *input, t_minishell *minishell)
{
	t_tokenizer	tok;
	t_token		*result;

	if (verif_quotes(input) == 1)
		return (NULL);
	init_tokenizer(&tok, input, minishell);
	while (input[++tok.i])
	{
		if (tok.state == NORMAL)
		{
			if (handle_normal_state(input, &tok, minishell) == 1)
				return (NULL);
		}
		else if (tok.state == IN_SQUOTE)
			handle_quotes(input, &tok, '\'');
		else if (tok.state == IN_DQUOTE)
			handle_quotes(input, &tok, '\"');
	}
	finalize_token(&tok, minishell, input);
	result = tok.token_list;
	return (result);
}

void	add_token(t_tokenizer *tok, enum e_token_type type, enum e_state state,
		t_minishell *minishell)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = gc_malloc((sizeof(t_token)), minishell->gc);
	if (!new_token)
		clean_exit(1, minishell);
	new_token->type = type;
	new_token->state = state;
	new_token->next = NULL;
	if (!tok->buff || !tok->buff[0])
		new_token->str = gc_strdup("", minishell->gc);
	else
		new_token->str = gc_strdup((tok->buff), minishell->gc);
	if (!new_token->str)
		clean_exit(1, minishell);
	if (tok->token_list == NULL)
		tok->token_list = new_token;
	else
	{
		tmp = tok->token_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}
