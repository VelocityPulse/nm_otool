/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 17:25:35 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 14:08:35 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

void	set_endian(t_data *nm_data, unsigned int magic_number)
{
	nm_data->endian = MAGIC;
	if (magic_number == FAT_MAGIC_64 || magic_number == FAT_MAGIC ||
			magic_number == MH_MAGIC_64 || magic_number == MH_MAGIC)
		return ;
	nm_data->endian = CIGAM;
}
