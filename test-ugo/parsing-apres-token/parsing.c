/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/11 16:54:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	count_words(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*get_next_word(const char *str, char c, int *pos, t_minishell *minishell)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (str[*pos + len] && str[*pos + len] != c)
		len++;
	word = gc_malloc(sizeof(char) * (len + 1), minishell->gc);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[*pos + i];
		i++;
	}
	word[i] = '\0';
	*pos += len;
	return (word);
}

static char	*get_rest_of_string(const char *str, int pos, t_minishell *minishell)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (str[pos + len])
		len++;
	word = gc_malloc(sizeof(char) * (len + 1), minishell->gc);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[pos + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**gc_split(const char *str, char c, t_minishell *minishell)
{
	char	**result;
	int		pos;

	if (!str)
		return (NULL);
	result = gc_malloc(sizeof(char *) * 3, minishell->gc);
	if (!result)
		return (NULL);
	pos = 0;
	result[0] = get_next_word(str, c, &pos, minishell);
	if (str[pos] == c)
	{
		pos++;
		result[1] = get_rest_of_string(str, pos, minishell);
	}
	else
	{
		result[1] = gc_malloc(sizeof(char), minishell->gc);
		if (result[1])
			result[1][0] = '\0';
	}

	result[2] = NULL;
	return (result);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *gc_strjoin_three(char *s1, char *s2, char *s3, t_minishell *minishell)
{
	char *result;
	int i;
	int j;
	int k;

	result = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1), minishell->gc);
	if (!result)
		return (NULL);
	i = -1;
	j = -1;
	k = -1;
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	while (s3[++k])
		result[i + j + k] = s3[k];
	result[i + j + k] = '\0';
	return (result);
}

char	*gc_strdup(const char *s1, t_minishell *minishell)
{
	char	*str;
	int		i;

	str = gc_malloc(sizeof(char) * (ft_strlen(s1) + 1), minishell->gc);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}


t_cmd parsing(t_token *token, t_minishell *minishell)
{
	t_cmd *cmd;
	t_token *current;
	char *result_raw;
	int nb_params;

	token = current;
	result_raw[0] = '\0';
	while (current)
	{
		if (current->type == T_WORD)
		{
			cmd = gc_malloc(sizeof(t_cmd), minishell->gc);
			while (current->type == T_WORD && current)
			{
				result_raw = gc_strjoin_three(result_raw, " ", current->str, minishell);
				nb_params++;
				current = current->next;
			}
			cmd->command_raw = gc_strdup(result_raw, minishell);
			cmd->command = gc_split(result_raw, ' ', minishell);
			cmd->nb_of_params = nb_params;
		}
	}
}

int main()
{
	t_minishell minishell;
	t_gc_head *gc;
	t_cmd parsing;
	t_cmd *parsing2;

	parsing2 = malloc(sizeof(t_cmd));
	parsing2->command_raw = "salut";
	parsing.command_raw = "salut";
	minishell.gc = (gc = gc_init());
	printf("%p\n", parsing);
	printf("%p", parsing2);

}
