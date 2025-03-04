/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:41:43 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 19:20:22 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!*lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	lst = NULL;
}
/*
void	ft_putzero(void *s)
{
	unsigned char	*dst;

	dst = (unsigned char *)s;
		*dst = '\0';
}

int	main()
{
	t_list	*one;
	t_list	*two;
	int	i[2] = {1, 2};

	one = ft_lstnew(&i[0]);
	two = ft_lstnew(&i[1]);

	ft_lstadd_front(&one, two);
	while(one)
	{
		printf("this is int : %d\n", *(int *)one->content);	
		one = one->next;
	}
	ft_lstclear(&two, &ft_putzero);
	if (!two && !one)
		printf("well done!\n");
	else
		printf("not good\n");
	return (0);
}*/
