/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:33:29 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/18 19:38:02 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <unistd.h>

int	ft_printf(const char *str, ...);
int	ft_printnbr_base(size_t nbr, char c);
int	ft_printchar(char c);
int	ft_printstr(char *s);
int	ft_printnbs(int nb);
int	ft_printunsigned(unsigned int n);
int	ft_printptr(size_t n);

#endif
