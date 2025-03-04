/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:45:13 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/17 20:15:21 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*stock;
	void	*buf;

	res = 0;
	while (lst)
	{
		buf = f(lst->content);
		stock = ft_lstnew(buf);
		if (!stock)
		{
			del(buf);
			ft_lstclear(&res, del);
			return (NULL);
		}
		ft_lstadd_back(&res, stock);
		lst = lst->next;
	}
	ft_lstclear(&lst, del);
	return (res);
}
/*
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
}

int con(int c)
{
	return(c++);
}

int main()
{
	t_list *list;
	int		i;

	i = 0;
	while (i++ < 10)
		ft_lstadd_front(list, ft_lstnew(i));
	ft_lstmap(list, con, free);
}*/
