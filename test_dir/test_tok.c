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
	
	token = tokenize(av[1]);
	if (token) 
	{
		token = expand_vars(token, &minishell);
		
		// Print token information
		t_token *current = token;
		while (current)
		{
			printf("str : %s\n", current->str);
			printf("t_type : %u\n", current->type);
			printf("state : %u\n\n", current->state);
			current = current->next;
		}
	}
	else
	{
		printf("Error: Failed to tokenize input\n");
	}

	gc_destroy(gc_head);
	return (0);
}
