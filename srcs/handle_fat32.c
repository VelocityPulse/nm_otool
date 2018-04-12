/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat32.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:17:28 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/12 18:16:29 by cchameyr         ###   ########.fr       */
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
	sub_nm_data->nfat_arch = nm_data->nfat_arch;
	sub_nm_data->file_name = nm_data->file_name;
	sub_nm_data->obj_name = NULL;
	sub_nm_data->ptr = ptr;
	if (offset != 0)
		sub_nm_data->ptr_offset = offset;
	return (_SUCCESS_);
}

int				start_arch32(t_data *nm_data, char *ptr, int offset,
		struct fat_arch *fa)
{
	struct fat_header	*f_header;
	t_data				sub_nm_data;

	f_header = (void *)ptr + offset;
	if (!trigger_false_pointer(nm_data,
				nm_data->ptr + nm_bsp32(nm_data, fa->size)))
		return (_ERROR_);
	sub_nm_data.ptr_offset = nm_bsp32(nm_data, fa->size);
	if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_POWERPC)
		sub_nm_data.arch_name = "ppc";
	else if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_I386)
		sub_nm_data.arch_name = "i386";
	else if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_X86_64)
		sub_nm_data.arch_name = "x86_64";
	if (!init_sub_nm_data(nm_data, &sub_nm_data, (void *)f_header,
				nm_bsp32(nm_data, fa->size)))
		return (_ERROR_);
	ft_nm(&sub_nm_data, (void *)f_header);
	return (_SUCCESS_);
}

static int		check_arch32(t_data *nm_data, struct fat_arch *fa, char *ptr)
{
	int		ret;
	int		offset;
	static int			iarch64_parsed = 0;
	static int			ppc_parsed = 0;

	ret = _SUCCESS_;
	offset = nm_bsp32(nm_data, fa->offset);
	if (!trigger_false_pointer(nm_data, (void *)ptr + offset))
		return (_ERROR_);
	if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_X86_64)
	{
		if (ppc_parsed == 0 && nm_data->nfat_arch == 2)
			nm_data->nfat_arch--;
		ret = start_arch32(nm_data, ptr, offset, fa);
		iarch64_parsed = 1;
	}
	else if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_I386 &&
			iarch64_parsed == 0)
	{
		nm_data->nfat_arch++;
		ret = start_arch32(nm_data, ptr, offset, fa);
	}
	else if (nm_bsp32(nm_data, fa->cputype) == CPU_TYPE_POWERPC)
	{
		ppc_parsed = 1;
		ret = start_arch32(nm_data, ptr, offset, fa);
	}
		return (ret);
}

// tester /usr/bin/appletviewer ..... its false
//TODO make the arch64 parsed for 64

int				handle_fat32(t_data *nm_data, char *ptr)
{
	int					i;
	int					ret;
	struct fat_arch		*fa;
	struct fat_header	*f_header;

	DEBUG
	f_header = (struct fat_header *)ptr;
	fa = (void *)ptr + sizeof(struct fat_header);
	ret = _SUCCESS_;
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)fa))
		return (_ERROR_);
	nm_data->nfat_arch = nm_bsp32(nm_data, f_header->nfat_arch);
	while (++i < (int)nm_bsp32(nm_data, f_header->nfat_arch))
	{
		ret = check_arch32(nm_data, fa, ptr);
		fa = (void *)fa + sizeof(struct fat_arch);
		if (!trigger_false_pointer(nm_data, (void *)fa))
			return (_ERROR_);
	}
	return (ret);
}
