/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_security.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/04 14:41:49 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 17:53:37 by cchameyr         ###   ########.fr       */
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

void	print_by_security(t_data *nm_data, char *str)
{
	unsigned int		i;
	char				*ptr;
	unsigned int		ptr_end;
	unsigned int		ui_str;

	i = 0;
	ptr = nm_data->ptr;
	ptr_end = (unsigned int)nm_data->ptr + nm_data->ptr_offset;
	ui_str = (unsigned int)str;
	while (ui_str < ptr_end && str[i])
	{
		ui_str++;
		i++;
	}
	ft_putnstr(str, i);
}

void	print_by_security_endl(t_data *nm_data, char *str)
{
	unsigned int		i;
	char				*ptr;
	unsigned int		ptr_end;
	unsigned int		ui_str;

	i = 0;
	ptr = nm_data->ptr;
	ptr_end = (unsigned int)nm_data->ptr + nm_data->ptr_offset;
	ui_str = (unsigned int)str;
	while (ui_str < ptr_end && str[i])
	{
		ui_str++;
		i++;
	}
	ft_putnstr(str, i);
	ft_putchar('\n');
}
