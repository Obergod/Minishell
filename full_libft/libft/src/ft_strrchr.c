/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:51:28 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 16:30:23 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *src, int c)
{
	char	*s;

	s = (char *)src;
	if ((unsigned char)c == '\0')
	{
		s = (char *)src + ft_strlen(src);
		return (s);
	}
	while (*src)
	{
		if (*src == (unsigned char)c)
			s = (char *)src;
		src++;
	}
	if (*s == (unsigned char)c)
		return (s);
	return (NULL);
}
/*
int	main(int ac, char **av)
{

	const char *str = "Welcome to Tutorialspoint";
   char ch = '\0';
   char *p = strchr(str, ch);

   if (p != NULL) 
   {
       printf("String starting from '%c' is: %s\n", ch, p);
	printf("this is my char : %s\n", ft_strrchr(str, ch));
   } 
   else 
   {
       printf("Character '%c' not found in the string.\n", ch);
   }

}*/
