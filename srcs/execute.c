/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 03:50:42 by aouichou          #+#    #+#             */
/*   Updated: 2023/01/27 19:17:00 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*check_path(char *cmd, char *path, char **paths)
{
	int		i;
	char	*temp;

	i = -1;
	path = NULL;
	while (paths && paths[++i])
	{
		free(path);
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		path = ft_strjoin_free(temp, cmd, 1);
		if (!access(path, F_OK))
			return (path);
	}
	if (path)
		free(path);
	return (NULL);
}

static	DIR	*check_cmd(t_main *ptr, t_list *cmds, char ***array, char *path)
{
	DIR		*dir;
	t_cmd	*cmd;

	dir = NULL;
	cmd = cmds->content;
	if (cmd && cmd->cmd_array)
		dir = opendir(*cmd->cmd_array);
	if (cmd && cmd->cmd_array && ft_strchr(*cmd->cmd_array, '/') && !dir)
	{
		*array = ft_split(*cmd->cmd_array, '/');
		cmd->path = ft_strdup(*cmd->cmd_array);
		free(cmd->cmd_array[0]);
		cmd->cmd_array[0] = ft_strdup(array[0][ft_array_size(*array)]);
	}
	else if (cmd && cmd->cmd_array && !check_builtin(cmd) && !dir)
	{
		path = ft_getenv("PATH", ptr->environ, 4);
		*array = ft_split(path, ':');
		free(path);
		cmd->path = check_path(*cmd->cmd_array, cmd->path, *array);
		if (!cmd->path || !cmd->cmd_array[0] || !cmd->cmd_array[0][0])
			ft_error(*cmd->cmd_array, NO_CMD, 127);
	}
	return (dir);
}

void	get_cmd(t_main *ptr, t_list *cmds, char **array, char *path)
{
	DIR		*dir;
	t_cmd	*cmd;

	cmd = cmds->content;
	dir = check_cmd(ptr, cmds, &array, path);
	if (!check_builtin(cmd) && cmd && cmd->cmd_array && dir)
		ft_error(*cmd->cmd_array, IS_DIR, 126);
	else if (!check_builtin(cmd) && cmd && cmd->path && access(cmd->path, F_OK) == -1)
		ft_error(cmd->path, NO_DIR, 127);
	else if (!check_builtin(cmd) && cmd && cmd->path && access(cmd->path, X_OK) == -1)
		ft_error(cmd->path, NO_PERM, 126);
	if (dir)
		closedir(dir);
	ft_array_free(&array);
}

void	*execute(t_main *ptr, t_list *cmds)
{
	int	fd[2];

	/*get_cmd(ptr, cmds, NULL, NULL);
	if (pipe(fd) == -1)
		return(ft_error(NULL, PIPE_ERR, 1));*/
	ft_pipex(ptr, cmds);
	/*if (!ft_pipex(ptr, cmds));
		return (NULL);
	close(fd[WRITE_FD]);
	if (cmds->next && !((t_cmd *)cmds->next->content)->infile)
		((t_cmd *)cmds->next->content)->infile = fd[READ_FD];
	else
		close(fd[WRITE_FD]);
	if (((t_cmd *)cmds->content)->infile > 2);
		close(((t_cmd *)cmds->content)->infile);
	if (((t_cmd *)cmds->content)->outfile > 2);
		close(((t_cmd *)cmds->content)->outfile);*/
	return (NULL);
}
