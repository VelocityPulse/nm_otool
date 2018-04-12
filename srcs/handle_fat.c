/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 11:36:52 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/12 14:05:12 by cchameyr         ###   ########.fr       */
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
	sub_nm_data->n_file = nm_data->n_file;
	sub_nm_data->file_name = nm_data->file_name;
	sub_nm_data->obj_name = NULL;
	sub_nm_data->ptr = ptr;
	if (offset != 0)
		sub_nm_data->ptr_offset = offset;
	return (_SUCCESS_);
}

// TODO afficher le nom des architectures (ppc)

static int		start_arch64(t_data *nm_data, char *ptr, int offset,
		struct fat_arch_64 *fa)
{
	struct fat_header	*f_header;
	t_data				sub_nm_data;

	f_header = (void *)ptr + offset;
	if (!trigger_false_pointer(nm_data,
				nm_data->ptr + nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	sub_nm_data.ptr_offset = nm_bsp64(nm_data, fa->size);
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}

int				handle_fat64(t_data *nm_data, char *ptr)
{
	int					i;
	struct fat_arch_64	*fa;
	struct fat_header	*f_header;
	int					offset;

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)fa))
		return (_ERROR_);
	while (++i < (int)nm_bsp64(nm_data, f_header->nfat_arch))
	{
		offset = nm_bsp64(nm_data, fa->offset);
		if (!trigger_false_pointer(nm_data, (void *)ptr + offset))
			return (_ERROR_);
		if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_X86_64)
			return (start_arch64(nm_data, ptr, offset, fa));
		fa = (void *)fa + sizeof(struct fat_arch);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (_SUCCESS_);
}

static int		start_arch32(t_data *nm_data, char *ptr, int offset,
		struct fat_arch *fa)
{
	struct fat_header	*f_header;
	t_data				sub_nm_data;

	f_header = (void *)ptr + offset;
	if (!trigger_false_pointer(nm_data,
				nm_data->ptr + (int)nm_bsp32(nm_data, fa->size)))
		return (_ERROR_);
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

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)fa))
		return (_ERROR_);
	while (++i < (int)nm_bsp32(nm_data, f_header->nfat_arch))
	{
		offset = nm_bsp32(nm_data, fa->offset);
		if (!trigger_false_pointer(nm_data, (void *)ptr + offset))
			return (_ERROR_);
		if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_X86_64)
			return (start_arch32(nm_data, ptr, offset, fa));
		fa = (void *)fa + sizeof(struct fat_arch);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (_SUCCESS_);
}
