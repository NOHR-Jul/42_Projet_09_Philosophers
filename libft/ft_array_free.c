/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 04:20:09 by aouichou          #+#    #+#             */
/*   Updated: 2022/12/28 18:50:45 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_array_free(char ***array)
{
	int		i;
	char	**temp_tab;

	i = -1;
	temp_tab = *array;
	if (temp_tab)
	{
		while (temp_tab[++i])
			free(temp_tab[i]);
		free(temp_tab);
	}
}
