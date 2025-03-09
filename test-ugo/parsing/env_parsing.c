/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/09 21:18:25 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <stdlib.h>
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

char	**gc_split(const char *str, char c, t_minishell *minishell)
{
	char	**result;
	int		words;
	int		i;
	int		pos;

	if (!str)
		return (NULL);
	words = count_words(str, c);
	result = gc_malloc(sizeof(char *) * (words + 1), minishell->gc);
	if (!result)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < words)
	{
		while (str[pos] == c)
			pos++;
		result[i] = get_next_word(str, c, &pos, minishell);
		i++;
	}
	result[i] = NULL;
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
			return (current->value);
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
	gc_destroy(gc_head);
	// i = 0;
	// gc_head = gc_init();
	// result_split = ft_split(envp[i], '=');
	// gc_alloc(result_split, gc_head);
	// head = malloc(sizeof(t_env));
	// gc_alloc(head, gc_head);
	// head->key = result_split[0];
	// head->value = result_split[1];
	// head->next = NULL;
	// // printf("%s=%s\n", head->key, head->value);
	// current = head;
	// while (envp[++i])
	// {
	// 	if (current==NULL)
	// 	{
	// 		result_split = ft_split(envp[i], '=');
	// 		gc_alloc(result_split, gc_head);
	// 		current = malloc(sizeof(t_env));
	// 		current->key = result_split[0];
	// 		current->value = result_split[1];
	// 		current->next = NULL;
	// 	}
	// 	// printf("%s=%s\n", current->key, current->value);
	// 	current = current->next;
	// }
	// aff_env(head);
}

// int main(int ac, char **av, char **envp)
// {
// 	char **result;
// 	t_env *head;
// 	t_env *current;
// 	int i;

// 	i = 0;
// 	result = ft_split(envp[i], '=');
// 	head = malloc(sizeof(t_env));
// 	if (!result || !head)
// 		return (1);
// 	head->key = result[0];
// 	head->value = result[1];
// 	head->next = NULL;
// 	while (envp[++i])
// 	{
// 		result = ft_split(envp[i], '=');
// 		current = malloc(sizeof(t_env));
// 		if (!result || !current)
// 			return (1);
// 		current->key = result[0];
// 		current->value = result[1];
// 		current->next = NULL;
// 		current = current->next;
// 	}

// }

// t_env *env_parsing(char **env)
// {
// 	t_env *env;

// }
