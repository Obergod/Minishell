#include "token.h"

int	main(int ac, char **av)
{
	t_token *token;

	token = tokenize(av[1]);
	printf("str : %s\n", token->str);
	printf("t_type : %u\n", token->type);
	if (token->str)
		free(token->str);
	if (token)
		free(token);
}
