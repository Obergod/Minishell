#include "token.h"

int	main(int ac, char **av)
{
	t_token *token;

	token = tokenize(av[1]);
	while (token)
	{
		if (token->str)
			printf("str : %s\n", token->str);
		else
			printf("no str\n");
		printf("t_type : %u\n", token->type);
		token = token->next;
	}
	if (token->str)
		free(token->str);
	if (token)
		free(token);
}
