#include "../includes/minishell.h"
#include "../includes/garbage_collector.h"
#include "../includes/parsing.h"
#include "../includes/token.h"
#include "../includes/expand.h"
#include "../includes/env_parsing.h"

/* Global variable accessible to all files in the project */
int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	t_token *token;
	t_minishell minishell;
	t_gc_head *gc_head;
	t_cmd *cmd_head;

	if (ac < 2)
	{
		printf("Usage: %s \"input string\"\n", av[0]);
		return (1);
	}

	gc_head = gc_init();
	minishell.gc = gc_head;
	
	// Initialize environment
	minishell.env = env_parsing(envp, &minishell);
	if (!minishell.env) {
		printf("Error: Failed to parse environment\n");
		gc_destroy(gc_head);
		return (1);
	}
	
	token = tokenize(av[1], &minishell);
	if (token) 
	{
		token = expand_vars(token, &minishell);
		
		// Print token information
		t_token *current = token;
		printf("******/		tokenization		******/\n\n");
		while (current)
		{
			printf("str : %s\n", current->str);
			printf("t_type : %u\n", current->type);
			printf("state : %u\n\n", current->state);
			current = current->next;
		}
		printf("\n\n\n");
	}
	else
	{
		printf("Error: Failed to tokenize input\n");
	}
	cmd_head = parsing(token, &minishell);
	printf("/*******		parsing		********/\n\n");
	while (cmd_head)
	{
		printf("Command_raw :%s\n", cmd_head->command_raw);
		for (int i=0;cmd_head->command[i];i++)
			printf("[%d] Command: %s\n", i, cmd_head->command[i]);
		printf("Infile: %s\n", cmd_head->infile);
		printf("Outfile: %s\n", cmd_head->outfile);
		printf("Append: %d\n", cmd_head->append);
		printf("Heredoc: %s\n", cmd_head->heredoc);
		printf("\n");
		cmd_head = cmd_head->next;
	}
	gc_destroy(gc_head);
	return (0);
}
