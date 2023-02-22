/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:46:13 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/10 20:06:08 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_export_variable_check(char *variable)
{
	int	i;

	i = 0;
	if (ft_isdigit(variable[i]))
		return (0);
	if (variable[i] == '=')
		return (0);
	while (variable[i] && variable[i] != '=')
	{
		if (!ft_isalpha(variable[i]))
			if (variable[i] != '_')
				if (variable[i] == '+' && variable[i + 1] != '=')
					return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_main *ptr)
{
	int		i[2];
	int		pos[2];
	char	*temp_str;
	char	**cmd;
	char	**temp;

	cmd = ((t_cmd *)ptr->cmds->content)->cmd_array;
	if (ft_array_size(cmd) >= 2)
	{
		i[0] = 1;
		while (cmd[i[0]])
		{
			if (ft_export_variable_check(cmd[i[0]]))
			{
				pos[1] = 0;
				if (ft_strchr(cmd[i[0]], '+'))
				{
					temp_str = ft_strjoin_free(ft_strtok(cmd[i[0]], '+'), ft_strrtok(cmd[i[0]], '+'), 2);
					pos[1] = 1;
				}
				else
					temp_str = cmd[i[0]];
				pos[0] = var_in_envp(ptr->environ, temp_str, i);
				if (pos[0] == 1)
				{
					if (pos[1] == 1)
						temp_str = ft_strjoin_free(ptr->environ[i[1]], ft_strrtok(temp_str, '='), 2);
					else
						free(ptr->environ[i[1]]);
					ptr->environ[i[1]] = ft_strdup(temp_str);
				}
				else if (!pos[0])
					ptr->environ = ft_array_expand(ptr->environ, temp_str);
				if (pos[1] == 1)
					free(temp_str);
			}
			else
			{
				temp_str = ft_strjoin("export: '", cmd[i[0]]);
				temp_str = ft_strjoin_free(temp_str, "': not a valid identifier", 1);
				ft_error(temp_str, 0, 1);
				free(temp_str);
			}
			i[0]++;
		}
	}
	else
	{
		temp = ft_array_sort(ptr->environ);
		ft_array_print_fd(temp, 1, 1);
	}
	return (0);
}
