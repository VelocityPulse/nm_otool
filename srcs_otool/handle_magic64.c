/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_magic64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:35:40 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 16:27:24 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

unsigned long int	nm_bsp64(t_data *nm_data, unsigned long int value)
{
	if (nm_data->endian == CIGAM)
		return (ft_bswap64(value));
	return (value);
}

static int			browse_section64(t_data *nm_data,
		struct segment_command_64 *segm)
{
	struct section_64			*sect;
	int							j;

	sect = (struct section_64*)(segm + 1);
	j = 0;
	while (j < (int)nm_bsp64(nm_data, segm->nsects))
	{
		if (!trigger_false_pointer(nm_data, (void *)sect))
			return (_ERROR_);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			if (!trigger_false_pointer(nm_data,
						nm_data->ptr + nm_bsp64(nm_data, sect->offset)))
				return (_ERROR_);
			print_memory64(nm_bsp64(nm_data, sect->addr), (unsigned char *)
					nm_data->ptr + nm_bsp64(nm_data, sect->offset),
					nm_bsp64(nm_data, sect->size), nm_data->has_ppc);
		}
		sect++;
		j++;
	}
	return (_SUCCESS_);
}

static int		browse_segment64(t_data *nm_data)
{
	int							i;
	int							ncmd;
	struct load_command			*lc;
	struct segment_command_64	*segm;

	ncmd = nm_bsp64(nm_data, ((struct mach_header_64 *)nm_data->header)->ncmds);
	lc = (struct load_command *)nm_data->first_load_command;
	i = -1;
	while (++i < ncmd)
	{
		if (!trigger_false_pointer(nm_data, (void *)lc))
			return (_ERROR_);
		if (nm_bsp64(nm_data, lc->cmd) == LC_SEGMENT_64)
		{
			segm = (struct segment_command_64 *)lc;
			if (ft_strcmp(segm->segname, SEG_TEXT) == 0)
				browse_section64(nm_data, segm);
		}
		lc = (void *)lc + nm_bsp64(nm_data, lc->cmdsize);
	}
	return (_SUCCESS_);
}

int					handle_magic64(t_data *nm_data, char *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;

	header = (struct mach_header_64 *)ptr;
	nm_data->header = (void *)header;
	if (nm_data->arch_name && nm_data->nfat_arch > 1)
		ft_printf("%s (architecture %s):\n", nm_data->file_name, nm_data->arch_name);
	else
		ft_printf("%s:\n", nm_data->file_name);
	ft_printf("Contents of (__TEXT,__text) section\n");
	lc = (void *)ptr + sizeof(struct mach_header_64);
	if (!trigger_false_pointer(nm_data, (void *)lc))
		return (_ERROR_);
	nm_data->first_load_command = lc;
	return (browse_segment64(nm_data));
}
