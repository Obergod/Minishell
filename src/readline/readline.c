/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:46:05 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 19:41:05 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/readline.h"
#include "../../includes/main.h"


// static char	*ft_add_readline(const char *prompt, char **stock)
// {
// 	(*stock) = readline(prompt);
// 	if (*stock == NULL)
// 		return (NULL);
// 	return (*stock);
// }

// int	ft_readline(t_minishell *minishell)
// {
// 	char	*input;

// 	setup_signals();
// 	while (ft_add_readline(PROMPT, &input) != NULL)
// 	{
// 		if (*input)
// 			add_history(input);
// 		printf("Commande entrée: %s\n", input);
// 		free(input);
// 	}
// 	rl_clear_history();
// 	printf("exit\n");
// 	clean_exit(minishell);
// 	return (0);
// }

//  int main(void)
//  {
//  	ft_readline();
//  	return (0);
//  }
