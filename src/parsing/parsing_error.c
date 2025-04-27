/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:52:49 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 16:54:18 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include <unistd.h>
#include <stdio.h>

/**
 * @brief Affiche un message d'erreur de syntaxe similaire à Bash
 * 
 * Cette fonction reproduit fidèlement les messages d'erreur de Bash
 * pour les différentes erreurs de parsing détectées. Les erreurs sont
 * affichées sur STDERR et respectent le format standard de Bash.
 * 
 * @param error_code Le code d'erreur de parsing
 * @return Le code d'erreur reçu
 */
enum error_parsing	print_error(enum error_parsing error_code)
{
	if (error_code == SUCCESS)
		return (SUCCESS);
	
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	
	if (error_code == ERR_SYNTAX_REDIRECT)
	{
		// Erreur quand une redirection n'a pas d'argument
		// Exemple: "ls >"
		ft_putstr_fd("syntax error near unexpected token `newline'\n", STDERR_FILENO);
	}
	else if (error_code == ERR_SYNTAX_PIPE)
	{
		// Erreur quand un pipe est mal placé
		// Exemples: "| ls" ou "ls |"
		ft_putstr_fd("syntax error near unexpected token `|'\n", STDERR_FILENO);
	}
	else if (error_code == ERR_SYNTAX_LOGIC)
	{
		// Erreur avec les opérateurs logiques
		// Exemples: "&& ls" ou "ls ||"
		ft_putstr_fd("syntax error near unexpected token\n", STDERR_FILENO);
	}
	else if (error_code == ERR_SYNTAX_TOKEN)
	{
		// Erreur avec les parenthèses ou autres tokens
		// Exemples: "(" ou ")"
		ft_putstr_fd("syntax error: unexpected token\n", STDERR_FILENO);
	}
	else
	{
		// Erreur générique de syntaxe
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
	}
	
	return (error_code);
} 