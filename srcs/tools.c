/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 19:36:16 by aouichou          #+#    #+#             */
/*   Updated: 2023/01/19 19:04:29 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ext	*get_ext(void)
{
	static t_ext	*ext = NULL;

	if (ext == NULL)
	{
		ext = (t_ext *)malloc(sizeof(t_ext));
		ext->exit_status = 0;
		if (!ext)
			return (NULL);
		ft_memset(ext, 0, 1);
	}
	return (ext);
}

t_main	*get_ptr(void)
{
	static t_main	*ptr = NULL;

	if (ptr == NULL)
	{
		ptr = (t_main *)malloc(sizeof(t_main));
		if (!ptr)
			return (NULL);
		ptr->cmds = NULL;
		ptr->environ = NULL;
		ptr->pid = 0;
	}
	return (ptr);
}
