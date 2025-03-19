#include "token.h"

int	main(int ac, char **av)
{
	t_token *token;

	token = tokenize(av[1]);
	while (token)
	{
		printf("str : %s\n", token->str);
		printf("t_type : %u\n", token->type);
		printf("state : %u\n\n", token->state);
		token = token->next;
	}
	//if (token->str)
	//	free(token->str);
	if (token)
		free(token);
}
