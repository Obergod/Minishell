/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:full_libft/libft/src/ft_is_even.c
/*   ft_is_even.c                                       :+:      :+:    :+:   */
========
/*   readline.h                                         :+:      :+:    :+:   */
>>>>>>>> main:includes/readline.h
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<<< HEAD:full_libft/libft/src/ft_is_even.c
/*   Created: 2025/03/21 14:31:49 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/21 14:31:50 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_even(int nb)
{
	return (nb % 2 == 0);
}
========
/*   Created: 2025/03/07 15:50:37 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 15:10:53 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
#define READLINE_H

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define PROMPT "Entrer votre commande: "

int	ft_readline(void);

#endif
>>>>>>>> main:includes/readline.h
