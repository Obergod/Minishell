#include "ast.h"
#include "parsing.h"
#include "garbage_collector.h"
#include "minishell.h"
#include <stdio.h>

// Fonction pour imprimer l'AST
void print_ast(t_ast_node *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    if (node->type == NODE_CMD && node->cmd && node->cmd->command_raw)
    {
        printf("CMD: %s", node->cmd->command_raw);
        if (node->subshell)
            printf(" (SUBSHELL)");
        printf("\n");
    }
    else
    {
        const char *type_str = "UNKNOWN";
        if (node->type == NODE_PIPE)
            type_str = "PIPE";
        else if (node->type == NODE_AND)
            type_str = "AND";
        else if (node->type == NODE_OR)
            type_str = "OR";

        printf("%s\n", type_str);
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

// Fonction pour créer une commande test
t_cmd *create_test_cmd(char *raw_cmd, enum e_logic_operator_type op_type, t_minishell *minishell)
{
    t_cmd *cmd = new_cmd(minishell);
    if (!cmd)
        return NULL;

    cmd->command_raw = gc_strdup(raw_cmd, minishell->gc);
    cmd->command = gc_malloc(sizeof(char*) * 2, minishell->gc);
    if (!cmd->command)
        return cmd;

    cmd->command[0] = gc_strdup(raw_cmd, minishell->gc);
    cmd->command[1] = NULL;
    cmd->logic_operator_type = op_type;

    return cmd;
}

// Fonction pour créer une liste de commandes de test
t_cmd *create_test_cmd_list(const char **cmds, const int *ops, int count, t_minishell *minishell)
{
    if (count <= 0)
        return NULL;

    t_cmd *head = NULL;
    t_cmd *current = NULL;

    for (int i = 0; i < count; i++)
    {
        t_cmd *new = create_test_cmd((char*)cmds[i], ops[i], minishell);
        if (!new)
            return head;

        if (!head)
        {
            head = new;
            current = head;
        }
        else
        {
            current->next = new;
            current = new;
        }
    }

    return head;
}

// Fonction pour tester le cas complexe: "(ls | redir) && ((redir | cat) || cat)"
void test_complex_case(t_minishell *minishell)
{
    printf("\n=== TEST: (ls | redir) && ((redir | cat) || cat) ===\n");

    // Créer la liste de commandes
    const char *cmds[] = {
        "OPEN_PAREN", "ls", "redir", "CLOSE_PAREN",
        "NONE",
        "OPEN_PAREN", "OPEN_PAREN", "redir", "cat", "CLOSE_PAREN",
        "cat", "CLOSE_PAREN"
    };

    const int ops[] = {
        OPEN_PARENTHESIS, NONE, NONE, CLOSE_PARENTHESIS,
        AND,
        OPEN_PARENTHESIS, OPEN_PARENTHESIS, NONE, NONE, CLOSE_PARENTHESIS,
        OR, CLOSE_PARENTHESIS
    };

    t_cmd *cmd_list = create_test_cmd_list(cmds, ops, sizeof(cmds)/sizeof(cmds[0]), minishell);

    // Afficher la liste de commandes
    printf("Liste de commandes créée:\n");
    t_cmd *current = cmd_list;
    int i = 0;
    while (current)
    {
        printf("[%d] Cmd: %s (Op: %d)\n", i++,
            current->command_raw ? current->command_raw : "(null)",
            current->logic_operator_type);
        current = current->next;
    }

    // Construire l'AST
    t_ast_node *ast = build_ast(cmd_list, minishell);

    // Afficher l'AST
    printf("\nArbre syntaxique résultant:\n");
    print_ast(ast, 0);
}

// Fonction pour tester d'autres cas complexes
void test_other_complex_cases(t_minishell *minishell)
{
    printf("\n=== TEST: (ls) || (cat && (grep | wc)) ===\n");

    const char *cmds2[] = {
        "OPEN_PAREN", "ls", "CLOSE_PAREN",
        "NONE",
        "OPEN_PAREN", "cat", "NONE", "OPEN_PAREN", "grep", "wc", "CLOSE_PAREN", "CLOSE_PAREN"
    };

    const int ops2[] = {
        OPEN_PARENTHESIS, NONE, CLOSE_PARENTHESIS,
        OR,
        OPEN_PARENTHESIS, NONE, AND, OPEN_PARENTHESIS, NONE, NONE, CLOSE_PARENTHESIS, CLOSE_PARENTHESIS
    };

    t_cmd *cmd_list2 = create_test_cmd_list(cmds2, ops2, sizeof(cmds2)/sizeof(cmds2[0]), minishell);

    printf("Liste de commandes créée:\n");
    t_cmd *current = cmd_list2;
    int i = 0;
    while (current)
    {
        printf("[%d] Cmd: %s (Op: %d)\n", i++,
            current->command_raw ? current->command_raw : "(null)",
            current->logic_operator_type);
        current = current->next;
    }

    t_ast_node *ast2 = build_ast(cmd_list2, minishell);

    printf("\nArbre syntaxique résultant:\n");
    print_ast(ast2, 0);

    // Encore plus complexe avec des parenthèses imbriquées
    printf("\n=== TEST: ((ls && cat) || (grep && wc)) && (echo || (pwd && cd)) ===\n");

    const char *cmds3[] = {
        "OPEN_PAREN", "OPEN_PAREN", "ls", "NONE", "cat", "CLOSE_PAREN",
        "NONE",
        "OPEN_PAREN", "grep", "NONE", "wc", "CLOSE_PAREN", "CLOSE_PAREN",
        "NONE",
        "OPEN_PAREN", "echo", "NONE", "OPEN_PAREN", "pwd", "NONE", "cd", "CLOSE_PAREN", "CLOSE_PAREN"
    };

    const int ops3[] = {
        OPEN_PARENTHESIS, OPEN_PARENTHESIS, NONE, AND, NONE, CLOSE_PARENTHESIS,
        OR,
        OPEN_PARENTHESIS, NONE, AND, NONE, CLOSE_PARENTHESIS, CLOSE_PARENTHESIS,
        AND,
        OPEN_PARENTHESIS, NONE, OR, OPEN_PARENTHESIS, NONE, AND, NONE, CLOSE_PARENTHESIS, CLOSE_PARENTHESIS
    };

    t_cmd *cmd_list3 = create_test_cmd_list(cmds3, ops3, sizeof(cmds3)/sizeof(cmds3[0]), minishell);

    printf("Liste de commandes créée:\n");
    current = cmd_list3;
    i = 0;
    while (current)
    {
        printf("[%d] Cmd: %s (Op: %d)\n", i++,
            current->command_raw ? current->command_raw : "(null)",
            current->logic_operator_type);
        current = current->next;
    }

    t_ast_node *ast3 = build_ast(cmd_list3, minishell);

    printf("\nArbre syntaxique résultant:\n");
    print_ast(ast3, 0);
}

int main()
{
    // Initialiser le GC pour les tests
    t_minishell minishell;
    minishell.gc = gc_init();
    if (!minishell.gc)
    {
        printf("Erreur: Impossible d'initialiser le garbage collector\n");
        return 1;
    }

    // Exécuter les tests
    test_complex_case(&minishell);
    test_other_complex_cases(&minishell);

    // Nettoyer
    gc_destroy(minishell.gc);

    return 0;
}
