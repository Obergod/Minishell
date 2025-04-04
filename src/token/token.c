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

void	handle_normal_state(const char *input, t_tokenizer *tok, t_minishell *minishell)
{
	if (input[tok->i] == ' ' || input[tok->i] == '\t')
		process_space(tok, minishell);
	// laisser quotes et retirer dans l'expand ?
	else if (input[tok->i] == '"')
	{
		tok->state = IN_DQUOTE;
		if (tok->nb_tok == 0)
			tok->token_state = tok->state;
	}
	else if (input[tok->i] == '\'')
	{
		tok->state = IN_SQUOTE;
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
