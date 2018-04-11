/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 11:36:52 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/11 16:39:50 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

static int		init_sub_nm_data(t_data *nm_data, t_data *sub_nm_data,
		char *ptr, int offset)
{
	if (!trigger_false_pointer(nm_data, ptr))
		return (_ERROR_);
	sub_nm_data->nlist64_list = NULL;
	sub_nm_data->nlist32_list = NULL;
	sub_nm_data->ptr = ptr;
	if (offset !=  0)
		sub_nm_data->ptr_offset = offset;
	return (_SUCCESS_);
}
// TODO afficher le nom des architectures (ppc)
// TODO grep trigger security and check if they return false



int				start_arch64(t_data *nm_data, char *ptr, int offset,
		struct fat_arch_64 *fa)
{
	struct	fat_header	*f_header;
	t_data				sub_nm_data;

	f_header = (void *)ptr + offset;
	if (!trigger_false_pointer(nm_data, nm_data->ptr + nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	sub_nm_data.ptr_offset = nm_bsp64(nm_data, fa->size);
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}


// TODO CONTINUE TO CONVERT IN 64

int				handle_fat64(t_data *nm_data, char *ptr)
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
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	f_header = (void *)ptr + offset;
	trigger_false_pointer(nm_data, nm_data->ptr + nm_bsp64(nm_data, fa->size));
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}

int				start_arch32(t_data *nm_data, char *ptr, int offset,
		struct fat_arch *fa)
{
	struct	fat_header	*f_header;
	t_data				sub_nm_data;

	f_header = (void *)ptr + offset;
	trigger_false_pointer(nm_data, nm_data->ptr + nm_bsp32(nm_data, fa->size));
	sub_nm_data.ptr_offset = nm_bsp32(nm_data, fa->size);
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp32(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}

int				handle_fat32(t_data *nm_data, char *ptr)
{
	int					i;
	struct fat_arch		*fa;
	struct fat_header	*f_header;
	int					offset;
	int					ret;

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	if (!trigger_false_pointer(nm_data, (void *)fa) && (i = -1))
		return (_ERROR_);
	while (++i < nm_bsp32(nm_data, f_header->nfat_arch))
	{
		offset = nm_bsp32(nm_data, fa->offset);
		if (!trigger_false_pointer(nm_data, (void *)ptr + offset))
			return (_ERROR_);
		if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_X86_64)
		{
			ret = start_arch32(nm_data, ptr, offset, fa);
			break;
		}
		fa = (void *)fa + sizeof(struct fat_arch);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (ret);
}
