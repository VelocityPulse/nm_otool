/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 11:36:52 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/10 10:59:45 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	init_sub_nm_data(t_data *nm_data, t_data *sub_nm_data, char *ptr,
		int offset)
{
	trigger_false_pointer(nm_data, ptr);
	sub_nm_data->nlist64_list = NULL;
	sub_nm_data->nlist32_list = NULL;
	sub_nm_data->ptr = ptr;
	if (offset !=  0)
		sub_nm_data->ptr_offset = offset;
}

void	handle_fat64(t_data *nm_data, char *ptr)
{
	int					i;
	struct fat_arch_64	*fa;
	struct fat_header	*f_header;
	int					offset;
	t_data				sub_nm_data;

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	trigger_false_pointer(nm_data, (void *)fa);
	i = -1;
	while (++i < nm_bsp64(nm_data, f_header->nfat_arch))
	{
		offset = nm_bsp64(nm_data, fa->offset);
		sub_nm_data.ptr_offset = nm_bsp64(nm_data, fa->size);
		trigger_false_pointer(nm_data, (void *)ptr + offset);
		if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_X86_64)
			break ;
		fa = (void *)fa + sizeof(struct fat_arch);
		trigger_false_pointer(nm_data, (void *)fa);
	}
	f_header = (void *)ptr + offset;
	trigger_false_pointer(nm_data, nm_data->ptr + nm_bsp64(nm_data, fa->size));
	init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
			nm_bsp64(nm_data, fa->size));
	ft_nm(&sub_nm_data, (void *)f_header);
}

void	handle_fat32(t_data *nm_data, char *ptr)
{
	int					i;
	struct fat_arch		*fa;
	struct fat_header	*f_header;
	int					offset;
	t_data				sub_nm_data;

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	trigger_false_pointer(nm_data, (void *)fa);
	i = -1;
	while (++i < nm_bsp32(nm_data, f_header->nfat_arch))
	{
		offset = nm_bsp32(nm_data, fa->offset);
		sub_nm_data.ptr_offset = nm_bsp32(nm_data, fa->size);
		trigger_false_pointer(nm_data, (void *)ptr + offset);
		if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_X86_64)
			break ;
		fa = (void *)fa + sizeof(struct fat_arch);
		trigger_false_pointer(nm_data, (void *)fa);
	}
	f_header = (void *)ptr + offset;
	trigger_false_pointer(nm_data, nm_data->ptr + nm_bsp32(nm_data, fa->size));
	init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
			nm_bsp32(nm_data, fa->size));
	ft_nm(&sub_nm_data, (void *)f_header);
}
