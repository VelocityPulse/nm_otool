/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_security.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/04 14:41:49 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 14:06:17 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

int		trigger_false_pointer(t_data *nm_data, char *ptr)
{
	if (ptr < nm_data->ptr || ptr > nm_data->ptr + nm_data->ptr_offset)
	{
		if (nm_data->mapped == MAPPED &&
				munmap(nm_data->ptr, nm_data->ptr_offset) < 0)
		{
			ft_printf("\nERROR in file [%s] : munmap fail\n",
					nm_data->file_name);
			return (_ERROR_);
		}
		ft_printf("\nERROR in file [%s] : bad pointer position\n",
				nm_data->file_name);
		return (_ERROR_);
	}
	return (_SUCCESS_);
}
