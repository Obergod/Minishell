/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:30:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/06 20:18:35 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include <sys/wait.h>

/*
 * Cette fonction exécute une commande simple (nœud CMD)
 * Elle s'occupe de fork et d'exécuter la commande avec execve
 */
int execute_command(t_ast_node *node, t_minishell *minishell)
{
    pid_t pid;
    int status;

    if (!node || !node->cmd || !node->cmd->command || !node->cmd->command[0])
        return (0);

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    else if (pid == 0) // Processus enfant
    {
        // Ici, on exécuterait normalement la commande avec execve
        // Pour ce test, on va simplement afficher la commande
        printf("Exécution de la commande : %s\n", node->cmd->command[0]);
        exit(0);
    }
    else // Processus parent
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
}

/*
 * Cette fonction exécute un pipeline (nœud PIPE)
 * Elle utilise la récursivité pour traiter l'ensemble du pipeline
 */
int execute_pipeline(t_ast_node *node, t_minishell *minishell)
{
    int pipefd[2];
    pid_t pid_left, pid_right;
    int status_left, status_right;

    if (!node)
        return (0);

    // Si c'est une commande simple, on l'exécute directement
    if (node->type == NODE_CMD)
        return (execute_command(node, minishell));

    // Si c'est un nœud PIPE, on crée un pipe et on exécute les deux côtés
    if (node->type == NODE_PIPE)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return (1);
        }

        // Fork pour le côté gauche du pipe
        pid_left = fork();
        if (pid_left == -1)
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            return (1);
        }
        else if (pid_left == 0) // Processus enfant pour la commande gauche
        {
            // Rediriger la sortie standard vers le pipe
            close(pipefd[0]); // Fermer la lecture du pipe
            dup2(pipefd[1], STDOUT_FILENO); // stdout -> pipe write
            close(pipefd[1]); // Fermer la copie du descripteur

            // Exécuter le côté gauche
            if (node->left->type == NODE_CMD)
                exit(execute_command(node->left, minishell));
            else
                exit(execute_pipeline(node->left, minishell));
        }

        // Fork pour le côté droit du pipe
        pid_right = fork();
        if (pid_right == -1)
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            // Attendre que le processus gauche termine
            waitpid(pid_left, NULL, 0);
            return (1);
        }
        else if (pid_right == 0) // Processus enfant pour la commande droite
        {
            // Rediriger l'entrée standard depuis le pipe
            close(pipefd[1]); // Fermer l'écriture du pipe
            dup2(pipefd[0], STDIN_FILENO); // stdin <- pipe read
            close(pipefd[0]); // Fermer la copie du descripteur

            // Exécuter le côté droit
            // Si c'est un NODE_CMD, alors c'est la dernière commande du pipeline
            // Sinon, c'est un autre nœud PIPE et la récursivité continuera
            if (node->right->type == NODE_CMD)
            {
                printf("Dernière commande du pipeline: %s\n",
                    node->right->cmd->command[0]);
                exit(execute_command(node->right, minishell));
            }
            else
                exit(execute_pipeline(node->right, minishell));
        }

        // Processus parent: fermer les descripteurs et attendre les enfants
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid_left, &status_left, 0);
        waitpid(pid_right, &status_right, 0);

        // Retourner le statut de la dernière commande (côté droit)
        if (WIFEXITED(status_right))
            return (WEXITSTATUS(status_right));
        return (1);
    }

    // Autres opérateurs (AND, OR) peuvent être ajoutés ici

    return (0);
}

/*
 * Fonction principale pour l'exécution de l'AST
 */
int execute_ast(t_ast_node *ast, t_minishell *minishell)
{
    if (!ast)
        return (0);

    // Exécuter en fonction du type de nœud
    if (ast->type == NODE_CMD)
        return (execute_command(ast, minishell));
    else if (ast->type == NODE_PIPE)
        return (execute_pipeline(ast, minishell));
    else if (ast->type == NODE_AND)
    {
        // Pour les opérateurs logiques AND (&&)
        int status = execute_ast(ast->left, minishell);
        if (status == 0) // Succès à gauche, exécuter la droite
            return (execute_ast(ast->right, minishell));
        return (status);
    }
    else if (ast->type == NODE_OR)
    {
        // Pour les opérateurs logiques OR (||)
        int status = execute_ast(ast->left, minishell);
        if (status != 0) // Échec à gauche, exécuter la droite
            return (execute_ast(ast->right, minishell));
        return (status);
    }

    return (0);
}
int	init_minishell(t_minishell *minishell, char **envp)
{
	t_gc_head	*gc_head;

	gc_head = gc_init();
	minishell->gc = gc_head;

	// Initialize environment
	minishell->env = env_parsing(envp, minishell);
	if (!minishell->env) {
		printf("Error: Failed to parse environment\n"); //creer un environnement
															//PWD=/home/ufalzone/42cursus/Cercle-3/Minishell/off-minishell-github/test_dir/bin
															// SHLVL=1
															// _=/usr/bin/env
		gc_destroy(gc_head);
		return (1);
	}
	return (0);
}
int main(int ac, char **av, char **envp)
{
    t_minishell minishell;
    t_token *tokens;
    t_cmd *cmd_list;
    t_ast_node *ast;
    char *line;

    // Initialiser minishell
    if (init_minishell(&minishell, envp) != 0)
        return (1);

    // Pour ce test, on va utiliser une ligne de commande fixe
    line = "ls -l | grep test | wc -l";
    printf("Ligne de commande testée: %s\n", line);

    // Tokenisation
    tokens = tokenize(line, &minishell);
    if (!tokens)
    {
        printf("Erreur: Échec de la tokenisation\n");
        return (1);
    }

    // Analyse syntaxique et création de la liste de commandes
    cmd_list = parsing(tokens, &minishell);
    if (!cmd_list)
    {
        printf("Erreur: Échec de l'analyse syntaxique\n");
        return (1);
    }

    // Construction de l'AST
    ast = build_ast(cmd_list, &minishell);
    if (!ast)
    {
        printf("Erreur: Échec de la construction de l'AST\n");
        return (1);
    }

    // Afficher l'AST pour visualisation
    printf("\n--- Arbre syntaxique (AST) ---\n");
    print_ast(ast, 0);

    // Exécuter l'AST
    printf("\n--- Exécution de l'AST ---\n");
    int status = execute_ast(ast, &minishell);
    printf("Statut de sortie: %d\n", status);

    // Nettoyage et libération de la mémoire
    gc_destroy(minishell.gc);
    return (0);
}
