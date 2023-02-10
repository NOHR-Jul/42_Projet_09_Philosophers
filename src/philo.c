/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 14:44:12 by juchene           #+#    #+#             */
/*   Updated: 2023/02/10 13:11:06 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	Function that displays t_main struct
void	ph_show_main_s(t_main *main_s)
{
	printf("\n-- SHOW MAIN_S --\n\n");
	printf("main_s->index\t\t: [%d]\n", main_s->index);
	printf("main_s->nbr_phil\t: [%d]\n", main_s->nbr_phil);
	printf("main_s->time_to_die\t: [%ld]\n", main_s->time_to_die);
	printf("main_s->time_to_eat\t: [%ld]\n", main_s->time_to_eat);
	printf("main_s->time_to_sleep\t: [%ld]\n", main_s->time_to_sleep);
	printf("main_s->min_meal\t: [%d]\n", main_s->min_meal);
	printf("main_s->mn_status\t: [%d]\n", main_s->mn_status);
	printf("main_s->print_lock\t: [%p]\n", &main_s->print_lock);
	printf("\n-- -- -- -- -- --\n\n");
}

//	Function that displays the content of a tab
void	ph_show_tab(char *tittle, char **tab)
{
	int	i;

	i = 0;
	printf("\n-- %s --\n\n", tittle);
	while (tab[i])
	{
		printf("%d\n", ph_atol(tab[i]));
		i++;
	}
}

// temps pour penser fixe ou parametre				// OK
// detection de mort en prenant les fourchettes		// OK
// a verifier si timetosleep = 0					// OK
// gestion des erreurs								// OK
// verif de la mort dans le usleep
int	main(int argc, char **argv)
{
	t_main	main_s;

	if (argc < 5 || argc > 6)
		return (ft_print_err(ARGS_NB, 1));
	ph_init_main_s(&main_s, argc, argv);
	if (!ph_arg_val(argv))
		ph_process(&main_s);
	else
		ft_print_err(ARGS_VAL, 2);
	pthread_mutex_destroy(&main_s.print_lock);
	pthread_mutex_destroy(&main_s.status_lock);
	ph_free_main(&main_s);
	return (0);
}
