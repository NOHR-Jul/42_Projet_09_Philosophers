/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_contract.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:38:29 by aouichou          #+#    #+#             */
/*   Updated: 2022/12/29 19:47:51 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_array_contract(char **array, int z)
{
	char	**new_array;
	int		i;
	int		j;

	if (!array)
		return (NULL);
	new_array = (char **)malloc(sizeof(char *) * ft_array_size(array));
	if (!new_array)
		return (NULL);
	i = 0;
	j = 0;
	while (array[i])
	{
		if (i == z)
		{
			if (array[i + 1])
				i++;
			else
				break ;
		}
		new_array[j++] = array[i++];
	}
	ft_array_free(&array);
	new_array[j] = NULL;
	return (new_array);
}
