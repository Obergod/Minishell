/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:14:29 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/17 19:47:06 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;
	size_t	len;

	len = (size * nmemb);
	res = (void *)malloc(len);
	if (!res)
		return (NULL);
	ft_memset(res, 0, len);
	return (res);
}
/*
int	main(void) {
   int n = 5;
   int *array;
	int	*a;

   array = (int*)calloc(n, sizeof(int));
	a = array;
   if (array == NULL)
	{
      		fprintf(stderr, "Memory allocation failed\n");
      		return (1);
	}
   printf("Array elements after calloc: ");
   while (n-- > 0)
      printf("%d ", *a++);
   printf("\n");

   free(array);
   return (0);
}*/
