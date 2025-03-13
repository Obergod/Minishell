/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/13 14:49:06 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <stdlib.h>
#include "env_parsing.h"

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

#include <stdio.h>

static void free_split(char **str)
{
	int i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}

void aff_env(t_env *env)
{
	t_env *current;

	current = env;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

static int init_head_env_parsing(char **envp, t_minishell *minishell, t_env **current, t_env **head)
{
	char **result_split;

	*head = gc_malloc(sizeof(t_env), minishell->gc);
	if (!*head)
		return (1);
	result_split = gc_split(envp[0], '=', minishell);
	if (!result_split)
		return (1);
	(*head)->key = result_split[0];
	(*head)->value = result_split[1];
	(*head)->next = NULL;
	*current = *head;
	return (0);
}

size_t ft_strlen(char *str)
{
	size_t i;

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

#include <stdlib.h>

t_env *env_parsing(char **envp, t_minishell *minishell)
{
	t_env *current;
	t_env *tmp;
	char **result_split;
	int i;
	t_env *head;

	if (init_head_env_parsing(envp, minishell, &current, &head) == 1)
		return (NULL);
	i = 0;
	while (envp[++i])
	{
		gc_alloc(result_split = gc_split(envp[i], '=', minishell), minishell->gc);
		if (!result_split)
			return (NULL);
		tmp = gc_malloc(sizeof(t_env), minishell->gc);
		if (!tmp)
			return (NULL);
		tmp->key = result_split[0];
		tmp->value = result_split[1];
		tmp->raw = gc_strjoin_three(result_split[0], "=", result_split[1], minishell);
		tmp->next = NULL;
		current->next = tmp;
		current = current->next;
	}
	return (head);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *find_in_env(char *key, t_minishell *minishell)
{
	t_env *current;

	current = minishell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			printf("%s\n", current->raw);
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;
	char *result;

	gc_head = gc_init();
	minishell.gc = gc_head;
	minishell.env = env_parsing(envp, &minishell);
	// aff_env(minishell.env);
	if ((result = find_in_env(av[1], &minishell)) != NULL)
			printf("%s\n", result);

	gc_print(gc_head);
	gc_destroy(gc_head);
}
