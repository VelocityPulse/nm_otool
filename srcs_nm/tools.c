/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 17:25:35 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 17:04:06 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	set_endian(t_data *nm_data, unsigned int magic_number)
{
	nm_data->endian = MAGIC;
	if (magic_number == FAT_MAGIC_64 || magic_number == FAT_MAGIC ||
			magic_number == MH_MAGIC_64 || magic_number == MH_MAGIC)
		return ;
	nm_data->endian = CIGAM;
}

void	free_nm_data(t_data *nm_data)
{
	free_nlist64(&nm_data->nlist64_list);
	free_nlist32(&nm_data->nlist32_list);
}

void	order_nlist64_mode(t_data *nm_data)
{
	t_nmlist64	*begin;
	t_nmlist64	*list;
	t_nmlist64	*end;

	if (!nm_data->r)
		return ;
	list = NULL;
	end = nm_data->nlist64_list;
	while (end->next)
		end = end->next;
	list = new_nmlist64(end->ptr, NULL, NULL, end->str);
	begin = list;
	end = end->back;
	while (end)
	{
		list->next = new_nmlist64(end->ptr, NULL, list, end->str);
		list = list->next;
		end = end->back;
	}
	free_nm_data(nm_data);
	nm_data->nlist64_list = begin;
}

void	order_nlist32_mode(t_data *nm_data)
{
	t_nmlist32	*begin;
	t_nmlist32	*list;
	t_nmlist32	*end;

	if (!nm_data->r)
		return ;
	list = NULL;
	end = nm_data->nlist32_list;
	while (end->next)
		end = end->next;
	list = new_nmlist32(end->ptr, NULL, NULL, end->str);
	begin = list;
	end = end->back;
	while (end)
	{
		list->next = new_nmlist32(end->ptr, NULL, list, end->str);
		list = list->next;
		end = end->back;
	}
	free_nm_data(nm_data);
	nm_data->nlist32_list = begin;
}
