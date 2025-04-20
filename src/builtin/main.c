// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/20 15:57:05 by ufalzone          #+#    #+#             */
// /*   Updated: 2025/04/20 15:59:42 by ufalzone         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/main.h"

// static int	execute_builtin(char **args, t_minishell *minishell)
// {
// 	if (!args || !args[0])
// 		return (0);
// 	if (ft_strcmp(args[0], "exit") == 0)
// 		return (ft_exit(args, minishell));
// 	else if (ft_strcmp(args[0], "echo") == 0)
// 		return (ft_echo(args));
// 	else if (ft_strcmp(args[0], "cd") == 0)
// 		return (ft_cd(args, minishell));
// 	else if (ft_strcmp(args[0], "pwd") == 0)
// 		return (ft_pwd());
// 	else if (ft_strcmp(args[0], "env") == 0)
// 		return (ft_env(minishell));
// 	else if (ft_strcmp(args[0], "export") == 0)
// 		return (ft_export(args, minishell));
// 	else if (ft_strcmp(args[0], "unset") == 0)
// 		return (ft_unset(args, minishell));
// 	else
// 	{
// 		ft_putstr_fd(PROMPT, 2);
// 		ft_putstr_fd(args[0], 2);
// 		ft_putstr_fd(": commande non trouvée\n", 2);
// 		return (127);
// 	}
// }

// static void	init_minishell(t_minishell *minishell, char **envp)
// {
// 	minishell->exit_status = 0;
// 	minishell->gc = gc_init();
// 	minishell->env = env_parsing(envp, minishell);
// 	minishell->env_array = convert_t_env_to_array(minishell);
// 	minishell->cmd = NULL;
// 	// setup_signals();
// }

// static char	**parse_input(char *input, t_minishell *minishell)
// {
// 	char	**args;

// 	if (!input || input[0] == '\0')
// 		return (NULL);
// 	args = gc_split(input, ' ', minishell->gc);
// 	return (args);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_minishell	minishell;
// 	char		*input;
// 	char		**args;

// 	(void)argc;
// 	(void)argv;
// 	init_minishell(&minishell, envp);
// 	while (1)
// 	{
// 		input = readline(PROMPT);
// 		if (!input)
// 		{
// 			ft_putstr_fd("exit\n", 1);
// 			break;
// 		}
// 		if (input[0] != '\0')
// 			add_history(input);
// 		args = parse_input(input, &minishell);
// 		minishell.exit_status = execute_builtin(args, &minishell);
// 		free(input);
// 	}
// 	clean_exit(minishell.exit_status, &minishell);
// 	return (minishell.exit_status);
// }

