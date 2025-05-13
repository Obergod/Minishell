/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:53:54 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:27:56 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_PARSING_H
# define ENV_PARSING_H

# include "garbage_collector.h"
# include "minishell.h"
# include <complex.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*raw;
	struct s_env	*next;
}					t_env;

// Fonction principale
t_env				*env_parsing(char **envp, t_minishell *minishell);
t_env				*init_env(t_minishell *minishell);
int					init_head_env_parsing(char **envp, t_minishell *minishell,
						t_env **current, t_env **head);
t_env				*create_value_env(char *key, char *value,
						t_minishell *minishell);

// Utilitaires parsing
char				*find_in_env(char *key, t_minishell *minishell);
int					edit_in_env(char *key, char *new_value,
						t_minishell *minishell);
char				**convert_t_env_to_array(t_minishell *minishell);
t_env				*create_value_env(char *key, char *value,
						t_minishell *minishell);
void				add_to_env(t_env **head, t_env *current);

#endif
