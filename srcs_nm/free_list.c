/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:27:26 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 13:34:14 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	free_nlist64(t_nmlist64 **begin)
{
	t_nmlist64 *item;

	item = *begin;
	while (item)
	{
		*begin = item->next;
		ft_memdel((void **)&item);
		item = *begin;
	}
	*begin = NULL;
}

void	free_nlist32(t_nmlist32 **begin)
{
	t_nmlist32 *item;

	item = *begin;
	while (item)
	{
		*begin = item->next;
		ft_memdel((void **)&item);
		item = *begin;
	}
	*begin = NULL;
}
