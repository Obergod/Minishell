/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_visualizer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 00:00:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/24 18:15:16 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include "../../includes/minishell.h"
#include "../../includes/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Fonctions principales
void    visualize_ast(t_ast_node *ast, const char *output_filename);
void    generate_dot_file(t_ast_node *ast, const char *filename);
void    render_ast_image(const char *dot_filename, const char *output_filename);

// Fonctions internes
void    write_dot_header(FILE *dot_file);
void    write_dot_footer(FILE *dot_file);
void    write_node(FILE *dot_file, t_ast_node *node, int *node_counter);
void    write_ast_recursive(FILE *dot_file, t_ast_node *node,
                            int parent_id, int current_id, int *node_counter);
char    *get_node_color(enum node_type type);
char    *get_node_label(t_ast_node *node);

#endif
