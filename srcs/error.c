/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 20:26:05 by aouichou          #+#    #+#             */
/*   Updated: 2023/01/20 18:15:58 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	errors(int error)
{
	if (error == NO_DIR)
		ft_putstr_fd("No such file or directory: ", STDERR_FILENO);
	else if (error == IS_DIR)
		ft_putstr_fd("Is a directory: ", STDERR_FILENO);
	else if (error == NOT_DIR)
		ft_putstr_fd("Not a directory: ", STDERR_FILENO);
	else if (error == NO_CMD)
		ft_putstr_fd("command not found: ", STDERR_FILENO);
	else if (error == NO_PERM)
		ft_putstr_fd("permission denied: ", STDERR_FILENO);
	else if (error == QUOTE)
		ft_putstr_fd("no matching quote found: ", STDERR_FILENO);
	else if (error == DUP_ERR)
		ft_putstr_fd("dup2 failed: ", STDERR_FILENO);
	else if (error == FORK_ERR)
		ft_putstr_fd("fork failed: ", STDERR_FILENO);
	else if (error == PIPE_ERR)
		ft_putstr_fd("error while creating pipe: ", STDERR_FILENO);
	else if (error == PIPE_ENDERR)
		ft_putstr_fd("syntax error near unexpected token `|': ", STDERR_FILENO);
	else if (error == MEM)
		ft_putstr_fd("no memory left on device: ", STDERR_FILENO);
}

void	*ft_error(char *arg, int error, int rno)
{
	ft_putstr_fd(RED, STDERR_FILENO);
	ft_putstr_fd("Minishell: Error! ", STDERR_FILENO);
	errors(error);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(BLANK, STDERR_FILENO);
	get_ext()->exit_status = rno;
	return (NULL);
}

void	cd_errors(char **str[2])
{
	DIR		*dir;
	t_ext	*ext;

	dir = NULL;
	ext = get_ext();
	if (str[0][1])
		dir = opendir(str[0][1]);
	if (!str[0][1] && str[1][0] && !str[1][0][0])
	{
		ext->exit_status = 1;
		ft_error("HOME not set", 0, 1);
	}
	if (str[1][0] && !str[0][1])
		ext->exit_status = chdir(str[1][0]) == -1;
	if (str[0][1] && dir && access(str[0][1], F_OK) != -1)
		chdir(str[0][1]);
	else if (str[0][1] && access(str[0][1], F_OK) == -1)
		ft_error(str[0][1], NO_DIR, 1);
	else if (str[0][1])
		ft_error(str[0][1], NOT_DIR, 1);
	if (str[0][1] && dir)
		closedir(dir);
}
