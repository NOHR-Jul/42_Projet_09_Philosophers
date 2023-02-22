/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:35:23 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/08 18:30:33 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static	char	**split_and_trim(t_main *ptr, char **argv)
{
	int		i;
	int		quotes[2];
	char	**temp_array;

	i = -1;
	while (argv && argv[++i])
	{
		argv[i] = expand_variables(argv[i], -1, quotes, ptr);
		argv[i] = expand_path(argv[i], -1, quotes, ft_getenv("HOME", ptr->environ, 4));
		temp_array = cmds_split_trim(argv[i], "<|>");
		ft_array_replace(&argv, temp_array, i);
		i += ft_array_size(temp_array) - 1;
		ft_array_free(&temp_array);
	}
	return (argv);
}

static void	*parsing(t_main *ptr, char **argv)
{
	int		i;
	int		do_exit;
	t_ext	*ext;

	do_exit = 0;
	ext = get_ext();
	ptr->cmds = fill_list(split_and_trim(ptr, argv), -1);
	if (!ptr->cmds)
		return (ptr);
	i = ft_lstsize(ptr->cmds);
	ext->exit_status = builtin(ptr, ptr->cmds, &do_exit, 0);
	while (i-- > 0)
		waitpid(-1, &ext->exit_status, 0);
	if (!do_exit && ext->exit_status == 13)
		ext->exit_status = 0;
	if (ext->exit_status > 255)
		ext->exit_status /= 255;
	if (argv && do_exit)
	{
		ft_lstclear(&ptr->cmds, free_lst);
		return (NULL);
	}
	return (ptr);
}

void	*check_line(char *line, t_main *ptr)
{
	t_cmd	*cmd;
	char	**args_array;
	char	*temp;
	int		fd;

	if (!line)
	{
		ft_putendl_fd("exit", 1);
		return (NULL);
	}
	if (line[0] != '\0')
	{
		add_history(line);
		temp = ft_getenv("HOME", ptr->environ, 4);
		if (temp)
		{
			temp = ft_strjoin_free(temp, "/.minishell_history", 1);
			fd = open(temp, O_CREAT | O_RDWR | O_APPEND, 0666);
			ft_putendl_fd(line, fd);
			close(fd);
			free(temp);
		}
	}
	args_array = split_and_check_quotes(line, " ");
	free(line);
	if (!args_array)
	{
		ft_error(NULL, QUOTE, 1);
		return ("");
	}
	ptr = parsing(ptr, args_array);
	if (ptr && ptr->cmds)
		cmd = ptr->cmds->content;
	if (ptr && ptr->cmds && cmd && cmd->cmd_array && ft_lstsize(ptr->cmds) == 1)
		ptr->environ = ft_setenv("_", cmd->cmd_array[ft_array_size(cmd->cmd_array) - 1], ptr->environ, 1); //////////////////////////////////////// cmd->cmd_array[0] ???
	if (ptr && ptr->cmds)
		ft_lstclear(&ptr->cmds, free_lst);
	return (ptr);
}
