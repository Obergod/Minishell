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

t_token	*tokenize(const char *input)
{
	int				i;
	t_token			*token;
	enum e_state	state;

	i = -1;
	t_token = malloc(sizeof(t_token));
	state = NORMAL;
	while (input[++i])
	{
		if (state == NORMAL)
		{
			if (input[i] == "")
		}
		else if (state == IN_SQUOTE)
		{

		}
		else if (state == IN_DQUOTE)
		{

		}
	}
}
