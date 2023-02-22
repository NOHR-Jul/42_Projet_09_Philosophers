/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 15:44:26 by juchene           #+#    #+#             */
/*   Updated: 2023/02/09 17:22:58 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ppx_close_fd(int fd[2])
{
	close(fd[READ_FD]);
	close(fd[WRITE_FD]);
}

void	*ppx_execute(t_main *ptr, t_list *cmds, int *fd, int tmp_fd)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmds->content;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (dup2(tmp_fd, STDIN_FILENO) < 0)
		return (ft_error(NULL, DUP_ERR, 1));
	ppx_close_fd(fd);
	close(tmp_fd);
	if (!check_builtin(cmd) && cmd->cmd_array)
		execve(cmd->path, cmd->cmd_array, ptr->environ);
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "cd", 2))
		get_ext()->exit_status = ft_cd(ptr);
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "export", 6))
		get_ext()->exit_status = ft_export(ptr);
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "unset", 5))
		get_ext()->exit_status = ft_unset(ptr);
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "pwd", 3))
		get_ext()->exit_status = ft_pwd();
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "echo", 4))
		get_ext()->exit_status = ft_echo(cmds);
	else if (check_builtin(cmd) && cmd->cmd_array && !ft_strncmp(*cmd->cmd_array, "env", 3))
	{
		if (cmd->cmd_array[1])
		{
			ft_error(cmd->cmd_array[1], NO_DIR, 127);
			get_ext()->exit_status = 127;
			return ("");
		}
		ft_array_print_fd(ptr->environ, 1, 1);
		get_ext()->exit_status = 0;
	}
	return ("");
}

void	*ppx_child_exec(t_main *ptr, t_list *cmds, int *fd, int tmp_fd)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmds->content;
	if (dup2(cmd->outfile, STDOUT_FILENO) < 0)
		return (ft_error(NULL, DUP_ERR, 1));
	ppx_close_fd(fd);
	if (ppx_execute(ptr, cmds, fd, tmp_fd))
		exit(get_ext()->exit_status);
	return ("");
}

void	*ppx_redirect(t_list *cmds, int (*fd)[2], int i)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmds->content;
	if (pipe(*fd) < 0)
		return (ft_error(NULL, PIPE_ERR, 1));
	if (i != 0)
		cmd->infile = (*fd)[READ_FD];
	if (cmds->next && cmd->outfile == STDOUT_FILENO)
		cmd->outfile = (*fd)[WRITE_FD];
	return ("");
}

void	ppx_fork(t_main *ptr, t_list *cmds, int fd[2], int *tmp_fd)
{
	ptr->pid = fork();
	if (ptr->pid < 0)
	{
		ppx_close_fd(fd);
		close(*tmp_fd);
		ft_error(NULL, FORK_ERR, 1);
	}
	else if (ptr->pid == 0)
		ppx_child_exec(ptr, cmds, fd, *tmp_fd);
	else
	{
		close(*tmp_fd);
		close(fd[WRITE_FD]);
		*tmp_fd = fd[READ_FD];
	}
}

int	ft_pipex(t_main *ptr, t_list *cmds)
{
	int		i;
	int		tmp;
	int		fd[2];
	t_cmd	*cmd;
	DIR		*dir;

	i = 0;
	cmd = (t_cmd *)cmds->content;
	tmp = dup(cmd->infile);
	dir = NULL;
	while (cmds && cmd)
	{
		cmd = (t_cmd *)cmds->content;
		get_cmd(ptr, cmds, NULL, NULL);
		ppx_redirect(cmds, &fd, i);
		if (cmd->cmd_array)
			dir = opendir(*cmd->cmd_array);
		if ((cmd->cmd_array && access(cmd->path, X_OK) == 0) || check_builtin(cmd))
			ppx_fork(ptr, cmds, fd, &tmp);
		else if (!check_builtin(cmd) && ((cmd->path && !access(cmd->path, F_OK))
				|| dir))
			get_ext()->exit_status = 126;
		else if (!check_builtin(cmd) && cmd->cmd_array)
			get_ext()->exit_status = 127;
		if (dir)
			closedir(dir);
		i++;
		cmds = cmds->next;
	}
	close(tmp);
	while (i > 0)
	{
		i--;
		if (i != 0)
			waitpid(-1, 0, 0);
		else if (i == 0)
			waitpid(ptr->pid, &(get_ext()->exit_status), 0);
	}
	if (get_ext()->exit_status > 255)
		get_ext()->exit_status /= 256;
	else
		get_ext()->exit_status %= 256 + 256 * (get_ext()->exit_status < 0);
	return (0);
}
