/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 15:44:20 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/02 18:28:59 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_builtin(t_cmd *cmd)
{
	int			i;
	static char	tab[7][7] = {{"echo"}, {"exit"}, {"cd"}, {"export"},
	{"unset"}, {"env"}, {"pwd"}};

	i = -1;
	if (!cmd || cmd->cmd_array == NULL)
		return (0);
	if ((cmd->cmd_array && ft_strchr(*cmd->cmd_array, '/'))
		|| (cmd->path && ft_strchr(cmd->path, '/')))
		return (0);
	while (++i < 7)
	{
		if (ft_strncmp(tab[i], *cmd->cmd_array, ft_strlen(tab[i])) == 0)
			return (1);
	}
	return (0);
}

int	ft_echo(t_list	*cmd)
{
	int		i[2];
	int		nwline;
	char	**argv;

	i[0] = 0;
	i[1] = 0;
	nwline = 1;
	argv = ((t_cmd *)cmd->content)->cmd_array;
	while (argv && argv[++i[0]])
	{
		if (!i[1] && !ft_strncmp(argv[i[0]], "-n", 2)
			&& (ft_countchar(argv[i[0]], 'n')
				== (int)(ft_strlen(argv[i[0]]) - 1)))
			nwline = 0;
		else
		{
			i[1] = 1;
			ft_putstr_fd(argv[i[0]], 1);
			if (argv[i[0] + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	return (write(1, "\n", nwline) == 2);
}

int	ft_pwd(void)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	ft_putendl_fd(temp, 1);
	free(temp);
	return (0);
}

int	ft_cd(t_main *ptr)
{
	char	**str[2];
	char	*temp;
	char	*temp_array[2];

	get_ext()->exit_status = 0;
	str[0] = ((t_cmd *)ptr->cmds->content)->cmd_array;
	if (str[0][1] && !ft_strncmp(str[0][1], "-", ft_strlen(str[0][1])))
	{
		free(str[0][1]);
		str[0][1] = ft_getenv("OLDPWD", ptr->environ, 6);
		ft_putendl_fd(str[0][1], 1);
	}
	temp = ft_getenv("HOME", ptr->environ, 4);
	if (!temp)
		temp = ft_strdup("");
	str[1] = ft_array_expand(NULL, temp);
	free(temp);
	temp = getcwd(NULL, 0);
	str[1] = ft_array_expand(str[1], temp);
	free(temp);
	cd_errors(str);
	if (get_ext()->exit_status == 0)
		ptr->environ = ft_setenv("OLDPWD", str[1][1], ptr->environ, 6);
	temp = getcwd(NULL, 0);
	if (!temp)
		temp = ft_strdup("");
	str[1] = ft_array_expand(str[1], temp);
	free(temp);
	ptr->environ = ft_setenv("PWD", str[1][2], ptr->environ, 3);
	ft_array_free(&str[1]);
	return (get_ext()->exit_status);
}

int	builtin(t_main *ptr, t_list *cmds, int *do_exit, int n)
{
	char	**array;
	t_ext	*ext;

	ext = get_ext();
	array = ((t_cmd *)cmds->content)->cmd_array;
	n = 0;
	if (array)
		n = ft_strlen(*array);
	if (array && !ft_strncmp(*array, "exit", n) && n == 4)
		ext->exit_status = ft_exit(cmds, do_exit);
	else if (!cmds->next && array && !ft_strncmp(*array, "cd", n) && n == 2)
		ext->exit_status = ft_cd(ptr);
	else if (!cmds->next && array && !ft_strncmp(*array, "export", n) && n == 6)
		ext->exit_status = ft_export(ptr);
	else if (!cmds->next && array && !ft_strncmp(*array, "unset", n) && n == 5)
		ext->exit_status = ft_unset(ptr);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		execute(ptr, cmds);
	}
	return (ext->exit_status);
}
