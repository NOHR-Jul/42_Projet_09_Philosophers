/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 06:10:00 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/10 18:34:33 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_getenv(char *var, char **envp, int z)
{
	int	i;
	int	zz;

	i = 0;
	if (z < 0)
		z = ft_strlen(var);
	while (!ft_strchr(var, '=') && envp && envp[i])
	{
		zz = z;
		if (zz < ft_strchri(envp[i], '='))
			zz = ft_strchri(envp[i], '=');
		if (!ft_strncmp(envp[i], var, zz))
			return (ft_substr(envp[i], zz + 1, ft_strlen(envp[i])));
		i++;
	}
	return (NULL);
}

int	var_in_envp(char **envp, char *var, int i[2])
{
	int		pos;
	int		len;
	char	*temp[2];

	i[1] = 0;
	if (ft_strchri(var, '=') != -1)
		temp[0] = ft_strtok(var, '=');
	else
		temp[0] = ft_strdup(var);
	len = ft_strlen(temp[0]);
	while (envp[i[1]])
	{
		if (ft_strchri(envp[i[1]], '=') != -1)
			temp[1] = ft_strtok(envp[i[1]], '=');
		else
			temp[1] = ft_strdup(envp[i[1]]);
		if (len < ft_strlen(temp[1]))
			len = ft_strlen(temp[1]);
		if (!ft_strncmp(temp[1], temp[0], len))
		{
			free(temp[0]);
			free(temp[1]);
			return (1);
		}
		i[1]++;
	}
	free(temp[0]);
	free(temp[1]);
	return (0);
}

char	**ft_setenv(char *var, char *value, char **envp, int n)
{
	int		i[2];
	char	*temp[2];

	if (n < 0)
		n = ft_strlen(var);
	i[0] = -1;
	temp[0] = ft_strjoin(var, "=");
	temp[1] = ft_strjoin(temp[0], value);
	free(temp[0]);
	while (!ft_strchr(var, '=') && envp && envp[++i[0]])
	{
		i[1] = n;
		if (i[1] < ft_strchri(envp[i[0]], '='))
			i[1] = ft_strchri(envp[i[0]], '=');
		if (!ft_strncmp(envp[i[0]], var, i[1]))
		{
			temp[0] = envp[i[0]];
			envp[i[0]] = temp[1];
			free(temp[0]);
			return (envp);
		}
	}
	envp = ft_array_expand(envp, temp[1]);
	free(temp[1]);
	return (envp);
}

int	ft_unset(t_main *ptr)
{
	char	**cmd;
	char	*temp;
	int		i[2];

	i[0] = 0;
	cmd = ((t_cmd *)ptr->cmds->content)->cmd_array;
	if (ft_array_size(cmd) >= 2)
	{
		while (cmd[++i[0]])
		{
			if (cmd[i[0]][ft_strlen(cmd[i[0]]) - 1] != '=')
			{
				temp = ft_strjoin_free(cmd[i[0]], "=", 1);
				cmd[i[0]] = temp;
			}
			if (var_in_envp(ptr->environ, cmd[i[0]], i))
				ft_array_replace(&ptr->environ, NULL, i[1]);
		}
	}
	return (0);
}
