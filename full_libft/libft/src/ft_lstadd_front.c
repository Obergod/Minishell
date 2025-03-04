/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:18:08 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 19:16:06 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}
/*
int	main()
{
	t_list	*one;
	t_list	*two;
	int	i[2] = {1, 2};

	one = ft_lstnew(&i[0]);
	two = ft_lstnew(&i[1]);
	printf("this is one : %d\n", *(int *)one->content);

	ft_lstadd_front(&one, two);
	while(one)
	{
		printf("this is int : %d\n", *(int *)one->content);	
		one = one->next;
	}
}*/
