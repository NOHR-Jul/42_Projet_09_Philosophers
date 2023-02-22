/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 18:52:24 by aouichou          #+#    #+#             */
/*   Updated: 2023/01/19 19:05:23 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_lst(void *content)
{
	t_cmd	*node;

	node = content;
	ft_array_free(&node->cmd_array);
	free(node->path);
	if (node->infile > STDERR_FILENO)
		close(node->infile);
	if (node->outfile > STDERR_FILENO)
		close(node->outfile);
	free(node);
}

void	free_ptr(void)
{
	t_main	*ptr;

	ptr = get_ptr();
	ft_array_free(&ptr->environ);
	ft_lstclear(&ptr->cmds, free_lst);
	free(ptr);
}
