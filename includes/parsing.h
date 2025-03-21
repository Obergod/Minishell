/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:54:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 18:48:33 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "minishell.h"
#include "garbage_collector.h"
#include <stdlib.h>
#include "token.h"

enum	e_logic_operator_type
{
	NONE,
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS
};

typedef struct s_cmd
{
    char *command_raw; //grep -e "salut"
    char **command; //["grep"] ["-e"] ["salut"] [NULL]
	size_t _arg_count;
	size_t _arg_capacity;
    char *infile; //file d'entree
	char *outfile; //file de sortie
	int append; //>> ou pas
	enum e_logic_operator_type logic_operator_type; //&& || ( )
	char *heredoc; //delimiteur
	struct s_cmd *next; //des que ya un pipe on passe au prochain
} t_cmd;


enum	error_parsing
{
	SUCCESS,
	ERR_SYNTAX_REDIRECT, //Redirection sans argument
	ERR_SYNTAX_PIPE, // Erreur de pipe
	ERR_SYNTAX_LOGIC, //Erreur d'operateur
	ERR_SYNTAX_TOKEN // Toutes autres erreurs
};

// Déclarations des fonctions
t_cmd *parsing(t_token *token, t_minishell *minishell);

#endif
