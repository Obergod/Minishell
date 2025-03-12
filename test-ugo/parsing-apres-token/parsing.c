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

char	**gc_split_env(const char *str, char c, t_minishell *minishell)
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

//<< en position 0
//EOF sera en position 1
//cmd en position 2

//< en position 0
//infile en position 1
//cmd en position 2

//aucun operator en 0
//commande sera en 0

t_cmd	*new_cmd(t_minishell *minishell)
{
	t_cmd *new;

	new = gc_malloc(sizeof(t_cmd), minishell->gc);
	new->command_raw = NULL;
	new->command = NULL;
	new->infile = NULL;
	new->outfile = NULL;
	new->append = -1;
	new->heredoc = NULL;
	new->next = NULL;
	return (new);
}

void split_cmd(t_cmd *cmd_list, t_minishell *minishell)
{
	t_cmd *current;

	current = cmd_list;
	while (current)
	{
		if (current->command_raw != NULL)
			current->command = gc_split_env(current->command_raw, ' ', minishell); //mettre vrai split
		current = current->next;
	}
}

void add_arg_to_cmd(t_cmd *cmd, char *str, t_minishell *minishell)
{
	int i;

	if (cmd->command_raw == NULL)
		cmd->command_raw = gc_strdup(str, minishell);
	else 
		cmd->command_raw = gc_strjoin_three(cmd->command_raw, " ", str, minishell);
	// if (cmd->command == NULL)
	// {
	// 	cmd->command[0] = gc_strdup(str, minishell);
	// 	cmd->command[1] = NULL;
	// }
	// else
	// {
	// 	i = 0;
	// 	while (cmd->command[i])
	// 		i++;
	// 	cmd->command[i] = gc_strdup(str, minishell);
	// 	cmd->command[i + 1] = NULL;
	// }
}


void add_cmd_to_list(t_cmd **cmd_list, t_cmd *current)
{
	t_cmd *tmp;

	if (cmd_list == NULL)
		*cmd_list = current;
	tmp = *cmd_list;
	while (tmp)
		tmp = tmp->next;
	tmp->next = current;
}

t_cmd *parsing(t_token *token, t_minishell *minishell)
{
	t_cmd *cmd_list;
	t_cmd *current_cmd;
	
	current_cmd = new_cmd(minishell);
	while (token)
	{
		if (token->type == T_WORD)
		{
			add_arg_to_cmd(current_cmd, token->str, minishell);
		}
		//trouver un moyen de verifier qu'il y'a bien un token->next->str
		else if (token->type == T_REDIR)
		{
			if (ft_strcmp(token->str, "<") == 0)
			{
				current_cmd->infile = gc_strdup(token->next->str, minishell);
			}
			else if(ft_strcmp(token->str, ">") == 0)
			{
				current_cmd->outfile = gc_strdup(token->next->str, minishell);
				current_cmd->append = 0;
			}
			else if(ft_strcmp(token->str, ">>") == 0)
			{
				current_cmd->outfile = gc_strdup(token->next->str, minishell);
				current_cmd->append = 1;
			}
			else if (ft_strcmp(token->str, "<<") == 0)
			{
				current_cmd->heredoc = gc_strdup(token->next->str, minishell);
			}
			token=token->next;
		}
		else if(token->type == T_PIPE)
		{
			add_cmd_to_list(&cmd_list, current_cmd);
			current_cmd = new_cmd(minishell);
		}
		token = token->next;
	}
	add_cmd_to_list(&cmd_list, current_cmd);
	split_cmd(cmd_list, minishell);
	return(cmd_list);
}



// t_cmd parsing(t_token *token, t_minishell *minishell)
// {
// 	t_cmd *cmd;
// 	t_cmd *tmp;
// 	t_token *current;
// 	int i;
// 	char *result_raw;
// 	int nb_params;

// 	i = 0;
// 	current = token;
// 	result_raw[0] = '\0';
// 	while (current)
// 	{
// 		if (current->type == T_WORD)
// 		{
// 			tmp = gc_malloc(sizeof(t_cmd), minishell->gc);
// 			while (current->type == T_WORD && current)
// 			{
// 				result_raw = gc_strjoin_three(result_raw, " ", current->str, minishell);
// 				nb_params++;
// 				current = current->next;
// 			}
// 			tmp->command_raw = gc_strdup(result_raw + 1, minishell); // +1 pour skip l'espace du debut
// 			tmp->command = gc_split(result_raw + 1, ' ', minishell);
// 			tmp->nb_of_params = nb_params;
// 			tmp->type = COMMAND;
// 			cmd = tmp;
// 		}

// 		i++;
// 	}

// }

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
