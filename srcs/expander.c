/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 14:19:17 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/10 19:06:07 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_var(char *str, int i, t_main *ptr)
{
	int		z;
	char	*path;
	char	*var;

	z = ft_strchri_set(str + i, "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != NULL);
	if (z == -1)
		z = ft_strlen(str) - 1;
	path = ft_substr(str, 0, i - 1);
	var = ft_getenv(str + i, ptr->environ, ft_strchri_set(str + i, "\"\'$|>< "));
	if (!var && str[i] == '$')
		var = ft_itoa(ptr->pid);
	else if (!var && str[i] == '?')
		var = ft_itoa(get_ext()->exit_status);
	path = ft_strjoin_free(path, var, 1);
	path = ft_strjoin_free(path, str + i + z, 1);
	free(var);
	return (path);
}

char	*expand_variables(char *str, int i, int quotes[2], t_main *ptr)
{
	char	*set1;
	char	*set2;

	set1 = "/~%^{}:; ";
	set2 = "/~%^{}:;\"";
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && str[i] == '$' && str[i + 1]
			&& ((ft_strchri_set(str + i + 1, set1) && !quotes[1])
				|| (ft_strchri_set(str + i + 1, set2) && quotes[1])))
			return (expand_variables(get_var(str, ++i, ptr), -1, quotes, ptr));
	}
	return (str);
}

char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	char	*path;
	char	*temp;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && !quotes[1] && str[i] == '~'
			&& (i == 0 || str[i - 1] != '$'))
		{
			path = ft_substr(str, 0, i);
			path = ft_strjoin_free(path, var, 1);
			temp = ft_substr(str, i + 1, ft_strlen(str));
			free(str);
			str = ft_strjoin_free(path, temp, 2);
			return (expand_path(str, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}
