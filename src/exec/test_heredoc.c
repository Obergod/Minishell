//parcourir l'ast
//quand redir heredoc, l'ovurir et stocker le resultat dans un char

#include "../../includes/parsing.h"
#include "../../includes/ast.h"
#include "../../includes/exec.h"

void ast_heredoc(t_ast_node *node, t_minishell *minishell)
{
    t_redir *current;
    
    if (!node)
        return;
    ast_heredoc(node->left, minishell);
    if (node->type == NODE_CMD)
    {
        current = node->cmd->redirs;
        while (current)
        {
            if (current->type == REDIR_HEREDOC)
            {
                int fd = here_doc(current->file_or_delimiter, minishell);
                if (fd < 0)
                    return;
                char *line;
                char *content = gc_strdup("", minishell->gc);
                while ((line = get_next_line(fd)) != NULL)
                {
                    gc_alloc(line, minishell->gc);
                    content = gc_strjoin(content, line, minishell->gc);
                }
                close(fd);
                node->cmd->heredoc = content;
            }
            current = current->next;
        }
    }
    ast_heredoc(node->right, minishell);
}

void print_heredoc(t_ast_node *node, t_minishell *minishell)
{
    while (node)
    {
        if (node->type == NODE_CMD)
            printf("Heredoc: %s\n", node->cmd->heredoc);
        print_heredoc(node->left, minishell);
        node = node->right;
    }
}
