/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/20 16:28:51 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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

static int	word_len(const char *str, char c)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

static char	*gc_strndup(const char *s, size_t n, t_minishell *minishell)
{
	char	*str;
	size_t	i;

	str = gc_malloc(sizeof(char) * (n + 1), minishell->gc);
	if (!str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**gc_split(const char *s, char c, t_minishell *minishell)
{
	char	**result;
	int		words;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = gc_malloc(sizeof(char *) * (words + 1), minishell->gc);
	if (!result)
		return (NULL);
	i = 0;
	while (i < words)
	{
		while (*s == c)
			s++;
		len = word_len(s, c);
		result[i] = gc_strndup(s, len, minishell);
		if (!result[i])
			return (NULL);
		s += len;
		i++;
	}
	result[i] = NULL;
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

//status : 0 = debut
//status : 1 = fin
static enum error_parsing start_end_check_parsing(t_token *token, int status)
{
	if (status == 0)
	{
		if (token->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		if (token->type == T_LOGIC)
			return (ERR_SYNTAX_LOGIC);
	}
	else if (status == 1)
	{
		if (token->type == T_REDIR) //si ca se finit par une redirection
			return (ERR_SYNTAX_REDIRECT);
		if (token->type == T_PIPE) //si ca se finit par un pipe
			return (ERR_SYNTAX_PIPE);
		if (token->type == T_LOGIC)
			return (ERR_SYNTAX_PIPE);
	}
	return (SUCCESS);
}

enum error_parsing check_parsing(t_token *token_p)
{
	t_token *token;

	token = token_p;
	if (!token)
		return (ERR_SYNTAX_TOKEN);
	if (start_end_check_parsing(token, 0) != SUCCESS)
		return (start_end_check_parsing(token, 0));
	while (token->next)
	{
		if (token->next && token->type == T_PIPE && token->next->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		if (token->next && token->type == T_REDIR && token->next->type != T_WORD)
			return (ERR_SYNTAX_REDIRECT);
		if (token->next && token->type == T_LOGIC && token->next->type == T_LOGIC)
			return (ERR_SYNTAX_LOGIC);
		token = token->next;
	}
	//on est au dernier token de la liste
	if (start_end_check_parsing(token, 1) != SUCCESS)
		return (start_end_check_parsing(token, 1));
	return (SUCCESS);
}


t_cmd	*new_cmd(t_minishell *minishell)
{
	t_cmd *new;
	int i;

	new = gc_malloc(sizeof(t_cmd), minishell->gc);
	new->command_raw = NULL;
	new->command = gc_malloc(sizeof(char *) * 4, minishell->gc);
	i = -1;
	while (++i < 4)
		new->command[i] = NULL;
	new->_arg_capacity = 4;
	new->_arg_count = 0;
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
			current->command = gc_split(current->command_raw, ' ', minishell); //mettre vrai split
		current = current->next;
	}
}

char **gc_strdup_array_size(char **s1, size_t size, t_gc_head *gc)
{
	int i;
	int j;
	char *new_str;
	char **new_array;

	j = 0;
	new_array = gc_malloc((sizeof(char *) * size) + 1, gc);
	while (s1[j])
	{
		new_str = gc_malloc((sizeof(char) * ft_strlen(s1[j]) + 1), gc);
		if (!new_str)
			return (NULL);
		i = 0;
		while (s1[j][i])
		{
			new_str[i] = s1[j][i];
			i++;
		}
		new_str[i] = '\0';
		new_array[j] = new_str;
		j++;
	}
	new_array[j] = NULL;
	return (new_array);
}

void add_arg_to_cmd(t_cmd *cmd, char *str, t_minishell *minishell)
{
	int i;
	char **tmp;

	if (cmd->command_raw == NULL)
		cmd->command_raw = gc_strdup(str, minishell);
	else
		cmd->command_raw = gc_strjoin_three(cmd->command_raw, " ", str, minishell);
	if (cmd->_arg_count >= cmd->_arg_capacity - 1)
	{
		cmd->command = gc_strdup_array_size(cmd->command, cmd->_arg_capacity * 2, minishell->gc);
		cmd->_arg_capacity = cmd->_arg_capacity * 2;

	} else {
		cmd->command[cmd->_arg_count] = gc_strdup(str, minishell);
		cmd->command[cmd->_arg_count + 1] = NULL;
		cmd->_arg_count++;
	}
}

void add_cmd_to_list(t_cmd **cmd_list, t_cmd *current)
{
	t_cmd *tmp;

	if (*cmd_list == NULL)
	{
		*cmd_list = current;
		return;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = current;
}

void print_error(enum error_parsing error, char *str)
{
    switch (error)
    {
        case SUCCESS:
            printf("SUCCESS\n");
            break;
        case ERR_SYNTAX_REDIRECT:
            printf("ERR_SYNTAX_NEWLINE\n");
            break;
        case ERR_SYNTAX_PIPE:
            printf("ERR_SYNTAX_PIPE\n");
            break;
        case ERR_SYNTAX_TOKEN:
            printf("ERR_SYNTAX_TOKEN\n");
            break;
    }
}


//regarder pur regler le soucis de **command, le split peut etre imprecis
t_cmd *parsing(t_token *token, t_minishell *minishell)
{
	t_cmd *cmd_list;
	t_cmd *current_cmd;

	cmd_list = NULL;
	current_cmd = new_cmd(minishell);
	if (check_parsing(token) != SUCCESS)
	{
		print_error(check_parsing(token), NULL);
		return (NULL);
	}
	while (token)
	{
		if (token->type == T_WORD)
			add_arg_to_cmd(current_cmd, token->str, minishell);
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
					current_cmd->infile = NULL;
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
	// split_cmd(cmd_list, minishell);
	return(cmd_list);
}

// Fonction pour créer un token et l'ajouter à la liste
t_token *create_token(char *str, enum e_token_type type, t_minishell *minishell)
{
	t_token *token;

	token = gc_malloc(sizeof(t_token), minishell->gc);
	if (!token)
		return (NULL);
	token->str = gc_strdup(str, minishell);
	token->type = type;
	token->next = NULL;
	return (token);
}

// Fonction pour ajouter un token à la fin de la liste
void add_token(t_token **head, t_token *new_token)
{
	t_token *current;

	if (*head == NULL)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// Fonction pour afficher le contenu d'une commande
void print_cmd(t_cmd *cmd, int test_type)
{
	int i;

	// Définition des codes couleur
	char *green = "\033[0;32m";  // Vert pour les résultats attendus
	char *blue = "\033[0;34m";   // Bleu pour les résultats réels
	char *reset = "\033[0m";     // Réinitialisation de la couleur

	printf("%sRésultat attendu:%s\n", green, reset);
	printf("%s-------------------\n", green);

	// Affichage des résultats attendus en vert, personnalisés selon le test
	switch (test_type)
	{
		case 1: // Commande simple: ls -la
			printf("%sCommand Raw: ls -la\n", green);
			printf("Command Args: [ls] [-la]\n");
			printf("Infile: NULL\n");
			printf("Outfile: NULL\n");
			printf("Append: -1\n");
			printf("Heredoc: NULL%s\n", reset);
			break;
		case 2: // Redirection entrée: cat < input.txt
			printf("%sCommand Raw: cat\n", green);
			printf("Command Args: [cat]\n");
			printf("Infile: input.txt\n");
			printf("Outfile: NULL\n");
			printf("Append: -1\n");
			printf("Heredoc: NULL%s\n", reset);
			break;
		case 3: // Redirection lsortie: echo hello > output.txt
			printf("%sCommand Raw: echo helo\n", green);
			printf("Command Args: [echo] [hello]\n");
			printf("Infile: NULL\n");
			printf("Outfile: output.txt\n");
			printf("Append: 0\n");
			printf("Heredoc: NULL%s\n", reset);
			break;
		case 4: // Pipe: ls -l | grep test
			if (cmd->command_raw && cmd->command_raw[0] == 'l') {
				printf("%sCommand Raw: ls -l\n", green);
				printf("Command Args: [ls] [-l]\n");
				printf("Infile: NULL\n");
				printf("Outfile: NULL\n");
				printf("Append: -1\n");
				printf("Heredoc: NULL%s\n", reset);
			} else {
				printf("%sCommand Raw: grep test\n", green);
				printf("Command Args: [grep] [test]\n");
				printf("Infile: NULL\n");
				printf("Outfile: NULL\n");
				printf("Append: -1\n");
				printf("Heredoc: NULL%s\n", reset);
			}
			break;
		case 5: // Commande complexe: cat < input.txt | grep pattern | wc -l >> result.txt
			if (cmd->command_raw && cmd->command_raw[0] == 'c') {
				printf("%sCommand Raw: cat\n", green);
				printf("Command Args: [cat]\n");
				printf("Infile: input.txt\n");
				printf("Outfile: NULL\n");
				printf("Append: -1\n");
				printf("Heredoc: NULL%s\n", reset);
			} else if (cmd->command_raw && cmd->command_raw[0] == 'g') {
				printf("%sCommand Raw: grep pattern\n", green);
				printf("Command Args: [grep] [pattern]\n");
				printf("Infile: NULL\n");
				printf("Outfile: NULL\n");
				printf("Append: -1\n");
				printf("Heredoc: NULL%s\n", reset);
			} else {
				printf("%sCommand Raw: wc -l\n", green);
				printf("Command Args: [wc] [-l]\n");
				printf("Infile: NULL\n");
				printf("Outfile: result.txt\n");
				printf("Append: 1\n");
				printf("Heredoc: NULL%s\n", reset);
			}
			break;
		case 6: // Heredoc: cat << EOF
			printf("%sCommand Raw: cat\n", green);
			printf("Command Args: [cat]\n");
			printf("Infile: NULL\n");
			printf("Outfile: NULL\n");
			printf("Append: -1\n");
			printf("Heredoc: EOF%s\n", reset);
			break;
		default:
			printf("%sCommand Raw: devrait contenir la commande brute\n", green);
			printf("Command Args: devrait contenir les arguments séparés\n");
			printf("Infile: devrait contenir le fichier d'entrée si présent\n");
			printf("Outfile: devrait contenir le fichier de sortie si présent\n");
			printf("Append: devrait être 1 pour >> ou 0 pour > ou -1 si absent\n");
			printf("Heredoc: devrait contenir le délimiteur si présent%s\n", reset);
	}

	printf("%s-------------------\n", reset);
	printf("%sRésultat réel:%s\n", blue, reset);

	// Affichage des résultats réels en bleu
	printf("%sCommand Raw: %s\n", blue, cmd->command_raw ? cmd->command_raw : "NULL");

	printf("Command Args: ");
	if (cmd->command)
	{
		i = 0;
		while (cmd->command[i])
		{
			printf("[%s] ", cmd->command[i]);
			i++;
		}
	}
	else
		printf("NULL");
	printf("\n");

	printf("Infile: %s\n", cmd->infile ? cmd->infile : "NULL");
	printf("Outfile: %s\n", cmd->outfile ? cmd->outfile : "NULL");
	printf("Append: %d\n", cmd->append);
	printf("Heredoc: %s%s\n", cmd->heredoc ? cmd->heredoc : "NULL", reset);
	printf("-------------------\n");
}

// Fonction pour tester le parsing
void test_parsing(t_minishell *minishell)
{
	t_token *tokens = NULL;
	t_cmd *result;
	char *cyan = "\033[0;36m";   // Cyan pour les descriptions de test
	char *yellow = "\033[0;33m";  // Jaune pour les avertissements
	char *red = "\033[0;31m";     // Rouge pour les erreurs
	char *reset = "\033[0m";     // Réinitialisation de la couleur

	// Test 1: Commande simple
	printf("\n%s=== Test 1: Commande simple ===\n", cyan);
	printf("Commande: ls -la%s\n", reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-la", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 1);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 2: Commande avec redirection entrée
	printf("\n%s=== Test 2: Commande avec redirection entrée ===\n", cyan);
	printf("Commande: cat < input.txt%s\n", reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 2);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 3: Commande avec redirection sortie
	printf("\n%s=== Test 3: Commande avec redirection sortie ===\n", cyan);
	printf("Commande: echo hello > output.txt%s\n", reset);
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("hello", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 3);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 4: Commande avec pipe
	printf("\n%s=== Test 4: Commande avec pipe ===\n", cyan);
	printf("Commande: ls -l | grep test%s\n", reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);

	// Afficher toutes les commandes dans la liste
	t_cmd *current = result;
	int cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 4);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 5: Commande complexe avec plusieurs redirections et pipes
	printf("\n%s=== Test 5: Commande complexe ===\n", cyan);
	printf("Commande: cat < input.txt | grep -e test de la commande plutot longue ou quoi la team sadjasdjasuidhasd asuidh asidhas dhasiudh asiudh asd | wc -l >> result.txt%s\n", reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("-e", T_WORD, minishell));
	add_token(&tokens, create_token("test de la commande plutot longue ou quoi la team sadjasdjasuidhasd asuidh asidhas dhasiudh asiudh asd", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("result.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);

	// Afficher toutes les commandes dans la liste
	current = result;
	cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 5);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 6: Commande avec heredoc
	printf("\n%s=== Test 6: Commande avec heredoc ===\n", cyan);
	printf("Commande: cat << EOF%s\n", reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 6);

	// ===== TESTS COMPLEXES AVEC CAS D'ERREUR =====

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 7: Redirection sans fichier
	printf("\n%s=== Test 7: Redirection sans fichier ===\n", cyan);
	printf("%sCommande: cat < %s\n", reset, red);
	printf("%s(Ce test devrait générer une erreur car il manque le fichier après <)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 8: Pipe sans commande après
	printf("\n%s=== Test 8: Pipe sans commande après ===\n", cyan);
	printf("%sCommande: ls | %s\n", reset, red);
	printf("%s(Ce test devrait générer une erreur car il manque une commande après |)%s\n", yellow, reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	result = parsing(tokens, minishell);
	current = result;
	cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 0);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 9: Pipe sans commande avant
	printf("\n%s=== Test 9: Pipe sans commande avant ===\n", cyan);
	printf("%sCommande: | ls%s\n", red, reset);
	printf("%s(Ce test devrait générer une erreur car il manque une commande avant |)%s\n", yellow, reset);
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("ls", T_WORD, minishell));

	result = parsing(tokens, minishell);
	current = result;
	cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 0);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 10: Multiples redirections du même type
	printf("\n%s=== Test 10: Multiples redirections du même type ===\n", cyan);
	printf("%sCommande: cat < file1.txt < file2.txt%s\n", reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs redirections du même type - seule la dernière devrait être prise en compte)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file2.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 0);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 11: Redirections multiples de différents types
	printf("\n%s=== Test 11: Redirections multiples de différents types ===\n", cyan);
	printf("%sCommande: cat < input.txt > output.txt >> append.txt << EOF%s\n", reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs redirections de types différents)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("append.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));

	result = parsing(tokens, minishell);
	print_cmd(result, 0);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 12: Pipes multiples sans commandes entre eux
	printf("\n%s=== Test 12: Pipes multiples sans commandes entre eux ===\n", cyan);
	printf("%sCommande: ls | | grep test%s\n", red, reset);
	printf("%s(Ce test devrait générer une erreur car il y a deux pipes consécutifs)%s\n", yellow, reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	current = result;
	cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 0);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 13: Commande très complexe avec de nombreuses redirections et pipes
	printf("\n%s=== Test 13: Commande très complexe ===\n", cyan);
	printf("%sCommande: cat < in1.txt < in2.txt | grep pattern > out1.txt | sort | uniq >> out2.txt << EOF | wc -l%s\n", reset);
	printf("%s(Ce test vérifie comment votre code gère une commande très complexe avec de nombreuses redirections et pipes)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in2.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("sort", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("uniq", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out2.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));

	result = parsing(tokens, minishell);
	current = result;
	cmd_num = 1;
	while (current)
	{
		printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
		print_cmd(current, 0);
		current = current->next;
	}

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 14: Redirection sans commande
	printf("\n%s=== Test 14: Redirection sans commande ===\n", cyan);
	printf("%sCommande: > output.txt%s\n", red, reset);
	printf("%s(Ce test devrait générer une erreur car il n'y a pas de commande avant la redirection)%s\n", yellow, reset);
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 15: Commande vide
	printf("\n%s=== Test 15: Commande vide ===\n", cyan);
	printf("%sCommande: (aucun token)%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une commande vide)%s\n", yellow, reset);

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (commande vide)%s\n", red, reset);

	// ===== TESTS EXTRÊMEMENT COMPLEXES =====
	printf("\n%s========= TESTS EXTRÊMEMENT COMPLEXES POUR CASSER LE PARSING =========%s\n\n", red, reset);

	// Test 16: Plusieurs redirections consécutives de différents types
	tokens = NULL;
	printf("\n%s=== Test 16: Redirections consécutives multiples ===\n", cyan);
	printf("%sCommande: echo test > file1 > file2 >> file3 < file4 << EOF < file5%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une cascade de redirections consécutives)%s\n", yellow, reset);
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("file1", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("file2", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("file3", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file4", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file5", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 17: Pipeline vide entre commandss
	tokens = NULL;
	printf("\n%s=== Test 17: Pipeline avec des commandes vides au milieu ===\n", cyan);
	printf("%sCommande: cat file1 | | | grep pattern%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs pipes consécutifs)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("file1", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 18: Redirection entre pipes sans commande
	tokens = NULL;
	printf("\n%s=== Test 18: Redirection entre pipes sans commande ===\n", cyan);
	printf("%sCommande: ls | > file1 | grep test%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une redirection entre pipes sans commande)%s\n", yellow, reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("file1", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 19: Mélange complexe de redirections et pipes
	tokens = NULL;
	printf("\n%s=== Test 19: Mélange ultra-complexe de redirections et pipes ===\n", cyan);
	printf("%sCommande: < in1 cat | < in2 > out1 grep pattern | << EOF >> out2 sort -r | < in3 > out3 > out4 << DELIM wc -l%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère un mélange très complexe de redirections avant et après les commandes)%s\n", yellow, reset);

	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in1", T_WORD, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in2", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1", T_WORD, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out2", T_WORD, minishell));
	add_token(&tokens, create_token("sort", T_WORD, minishell));
	add_token(&tokens, create_token("-r", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in3", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out3", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out4", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("DELIM", T_WORD, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 20: Entrée standard depuis redirection avant autre redirection
	tokens = NULL;
	printf("\n%s=== Test 20: Redirection avant commande ===\n", cyan);
	printf("%sCommande: < infile > outfile cat%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une redirection avant la commande)%s\n", yellow, reset);

	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("infile", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("outfile", T_WORD, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 21: Multiples commandes sans pipes (erreur en bash)
	tokens = NULL;
	printf("\n%s=== Test 21: Multiples commandes sans pipes ===\n", cyan);
	printf("%sCommande: ls -l grep test%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs commandes sans pipes - bash traiterait 'grep' et 'test' comme des arguments)%s\n", yellow, reset);

	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 22: Pipe à la fin avec une redirection
	tokens = NULL;
	printf("\n%s=== Test 22: Pipe à la fin avec redirection ===\n", cyan);
	printf("%sCommande: cat file | > output%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère un pipe suivi d'une redirection sans commande)%s\n", yellow, reset);

	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("file", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 23: Pipeline complexe terminant par une redirection
	tokens = NULL;
	printf("\n%s=== Test 23: Pipeline complexe terminant par redirection ===\n", cyan);
	printf("%sCommande: cat < file1 | grep pattern | sort | > output%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère un pipeline se terminant par une redirection sans commande)%s\n", yellow, reset);

	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file1", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("sort", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 24: Redirection en chaîne
	tokens = NULL;
	printf("\n%s=== Test 24: Redirections en chaîne ===\n", cyan);
	printf("%sCommande: < input > output1 > output2 > output3 cat%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une longue chaîne de redirections avant la commande)%s\n", yellow, reset);

	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output1", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output2", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output3", T_WORD, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 25: Redirection sans fichier ni commande
	tokens = NULL;
	printf("\n%s=== Test 25: Redirection sans fichier ni commande ===\n", cyan);
	printf("%sCommande: >%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère une redirection sans fichier ni commande)%s\n", yellow, reset);

	add_token(&tokens, create_token(">", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 26: Pipeline ultra complexe avec redirections imbriquées
	tokens = NULL;
	printf("\n%s=== Test 26: Pipeline ultra complexe avec redirections imbriquées ===\n", cyan);
	printf("%sCommande: << EOF1 < in1 cmd1 arg1 > out1 | < in2 >> out2 cmd2 | > out3 << EOF2 cmd3 arg1 arg2 | cmd4 < in3 > out4 | < in4 > out5 cmd5%s\n", red, reset);
	printf("%s(Ce test vérifie comment votre code gère un pipeline extrêmement complexe avec des redirections imbriquées partout)%s\n", yellow, reset);

	// Première commande avec redirections
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF1", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in1", T_WORD, minishell));
	add_token(&tokens, create_token("cmd1", T_WORD, minishell));
	add_token(&tokens, create_token("arg1", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	// Deuxième commande avec redirections
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in2", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out2", T_WORD, minishell));
	add_token(&tokens, create_token("cmd2", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	// Troisième commande avec redirections
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out3", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF2", T_WORD, minishell));
	add_token(&tokens, create_token("cmd3", T_WORD, minishell));
	add_token(&tokens, create_token("arg1", T_WORD, minishell));
	add_token(&tokens, create_token("arg2", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	// Quatrième commande avec redirections après
	add_token(&tokens, create_token("cmd4", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in3", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out4", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	// Cinquième commande avec redirections avant
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in4", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out5", T_WORD, minishell));
	add_token(&tokens, create_token("cmd5", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);
}
