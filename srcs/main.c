/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 19:43:33 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/09 18:42:55 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_history(t_main *ptr)
{
	char	*cmd;
	int		fd;
	char	*temp;

	temp = ft_getenv("HOME", ptr->environ, 4);
	if (temp)
	{
		temp = ft_strjoin_free(temp, "/.minishell_history", 1);
		fd = open(temp, O_CREAT | O_RDWR | O_APPEND, 0666);
		cmd = get_next_line(fd);
		while (cmd)
		{
			cmd = ft_substr_free(cmd, 0, ft_strchri(cmd, '\n'), 1);
			add_history(cmd);
			free(cmd);
			cmd = get_next_line(fd);
		}
		close(fd);
		free(temp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_main	*ptr;
	char	*prompt;
	char	*line;
	int		r;
	t_ext	*ext;

	ptr = init_main(argv, envp);
	get_history(ptr);
	while (argc && argv && ttyname(STDIN_FILENO))
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		prompt = get_prompt(ptr);
		line = readline(prompt);
		free(prompt);
		if (!check_line(line, ptr))
			break ;
	}
	free_ptr();
	r = get_ext()->exit_status;
	free(get_ext());
	rl_clear_history();
	exit (r);
}
