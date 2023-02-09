/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:33:28 by juchene           #+#    #+#             */
/*   Updated: 2023/02/09 10:53:32 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_print_err(char *str, int code)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	write(1, str, i);
	return (code);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ph_atol(char *nbr)
{
	long int	nb;
	int			i;

	nb = 0;
	i = 0;
	if (!nbr || nbr[i] < 48 || nbr[i] > 57)
		return (-1);
	while (nbr[i])
	{
		nb *= 10;
		if (nbr[i] < 48 || nbr[i] > 57)
			return (0);
		else
			nb += (nbr[i] - 48);
		i++;
	}
	if (i > 10 || nb != (int)nb)
		return (-1);
	return ((int)nb);
}

int	ph_arg_val(char **tab)
{
	int	i;

	i = 1;
	if (!tab || !ph_atol(tab[i]))
		return (i);
	if (ph_atol(tab[i]) > 250)
		return (i);
	while (tab && tab[i])
	{
		if (ph_atol(tab[i]) <= 0)
			return (i);
		i++;
	}
	return (0);
}

long int	get_time_in_ms(void)
{
	struct timeval	tv;
	long int		ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ms);
}
