/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_security.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/04 14:41:49 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 10:52:39 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	trigger_false_pointer(t_data *nm_data, char *ptr)
{
	if (ptr < nm_data->ptr || ptr > nm_data->ptr + nm_data->ptr_offset)
	{
		if (munmap(nm_data->ptr, nm_data->ptr_offset) < 0)
		{
			ft_putstr("munmap error\n");
			exit(EXIT_FAILURE);
		}
		ft_putstr("ft_nm error : bad pointer position\n");
		exit(EXIT_FAILURE);
	}
}
