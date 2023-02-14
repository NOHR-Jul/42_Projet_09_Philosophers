/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:41:04 by juchene           #+#    #+#             */
/*   Updated: 2023/02/10 15:37:18 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	Function to destroy forks mutexes and free the fork array
void	ph_free_forks_tab(t_main *main_s)
{
	int	i;

	i = 0;
	if (!main_s->forks)
		return ;
	while (i < main_s->nbr_phil)
	{
		pthread_mutex_destroy(&main_s->forks[i].fork_lock);
		i++;
	}
	if (main_s->forks)
		free(main_s->forks);
}

//	Function to destroy main mutexes and free the philo array
void	ph_free_main(t_main *main_s)
{
	int	i;

	i = 0;
	ph_free_forks_tab(main_s);
	if (!main_s->philos)
		return ;
	while (i < main_s->nbr_phil)
	{
		pthread_mutex_destroy(&main_s->philos[i].ph_lock);
		i++;
	}
	if (main_s->philos)
		free(main_s->philos);
}
