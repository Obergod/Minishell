/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:52:49 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 18:07:08 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

enum error_parsing	print_error(enum error_parsing error_code)
{
	if (error_code == SUCCESS)
		return (SUCCESS);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error_code == ERR_SYNTAX_REDIRECT)
		ft_putstr_fd("syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
	else if (error_code == ERR_SYNTAX_PIPE)
		ft_putstr_fd("syntax error near unexpected token `|'\n", STDERR_FILENO);
	else if (error_code == ERR_SYNTAX_LOGIC)
		ft_putstr_fd("syntax error near unexpected token\n", STDERR_FILENO);
	else if (error_code == ERR_SYNTAX_TOKEN)
		ft_putstr_fd("syntax error: unexpected token\n", STDERR_FILENO);
	else
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
	return (error_code);
}
