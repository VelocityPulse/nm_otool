/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat64.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:44:22 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 16:20:08 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

static int			init_sub_nm_data(t_data *nm_data, t_data *sub_nm_data,
		char *ptr, int offset)
{
	if (!trigger_false_pointer(nm_data, ptr))
		return (_ERROR_);
	sub_nm_data->n_file = nm_data->n_file;
	sub_nm_data->nfat_arch = nm_data->nfat_arch;
	sub_nm_data->file_name = nm_data->file_name;
	sub_nm_data->has_x86_64 = nm_data->has_x86_64;
	sub_nm_data->has_i386 = nm_data->has_i386;
	sub_nm_data->obj_name = NULL;
	sub_nm_data->ptr = ptr;
	if (offset != 0)
		sub_nm_data->ptr_offset = offset;
	return (_SUCCESS_);
}

int					start_arch64(t_data *nm_data, char *ptr, int offset,
		struct fat_arch_64 *fa)
{
	struct fat_header_64	*f_header;
	t_data					sub_nm_data;

	f_header = (void *)ptr + offset;
	ft_bzero((void **)&sub_nm_data, sizeof(t_data));
	if (!trigger_false_pointer(nm_data,
				nm_data->ptr + nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	sub_nm_data.ptr_offset = nm_bsp64(nm_data, fa->size);
	if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_POWERPC)
	{
		sub_nm_data.arch_name = " (architecture ppc)";
		sub_nm_data.has_ppc = 1;
	}
	else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_I386)
		sub_nm_data.arch_name = " (architecture i386)";
	else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_X86_64)
		sub_nm_data.arch_name = " (architecture x86_64)";
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp64(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}

static int			check_arch64(t_data *nm_data, struct fat_arch_64 *fa,
		char *ptr)
{
	int				offset;
	static int		arch64_printed = 0;

	offset = nm_bsp64(nm_data, fa->offset);
	if (!trigger_false_pointer(nm_data, (void *)ptr + offset))
		return (_ERROR_);
	if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_X86_64 && !arch64_printed)
	{
		arch64_printed = 1;
		return (start_arch64(nm_data, ptr, offset, fa));
	}
	else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_I386 &&
			!nm_data->has_x86_64)
		return (start_arch64(nm_data, ptr, offset, fa));
	else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_POWERPC)
		return (start_arch64(nm_data, ptr, offset, fa));
	return (_SUCCESS_);
}

static int			count_arch_to_print64(t_data *nm_data,
		struct fat_header *f_header, struct fat_arch_64 *fa)
{
	int					i;

	i = -1;
	while (++i < (int)nm_bsp64(nm_data, f_header->nfat_arch))
	{
		if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_X86_64 &&
				!nm_data->has_x86_64)
		{
			nm_data->has_x86_64 = 1;
			nm_data->has_i386 == 1 ? nm_data->nfat_arch-- :
				nm_data->nfat_arch++;
		}
		else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_I386 &&
				!nm_data->has_x86_64)
		{
			nm_data->has_i386 = 1;
			nm_data->nfat_arch++;
		}
		else if (nm_bsp64(nm_data, fa->cputype) == CPU_TYPE_POWERPC)
			nm_data->nfat_arch++;
		fa = (void *)fa + sizeof(struct fat_arch_64);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (_SUCCESS_);
}

int					handle_fat64(t_data *nm_data, char *ptr)
{
	int					i;
	int					ret;
	int					nfat_arch;
	struct fat_arch_64	*fa;
	struct fat_header	*f_header;

	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	ret = _SUCCESS_;
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)fa))
		return (_ERROR_);
	if (!count_arch_to_print64(nm_data, f_header, fa))
		return (_ERROR_);
	nfat_arch = (int)nm_bsp64(nm_data, f_header->nfat_arch);
	while (++i < nfat_arch)
	{
		ret = check_arch64(nm_data, fa, ptr);
		fa = (void *)fa + sizeof(struct fat_arch_64);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (ret);
}
