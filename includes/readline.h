/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:53:40 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:53:42 by ufalzone         ###   ########.fr       */
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
