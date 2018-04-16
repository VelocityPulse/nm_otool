/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_magic32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:42:51 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 16:38:44 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

unsigned int	nm_bsp32(t_data *nm_data, unsigned int value)
{
	if (nm_data->endian == CIGAM)
		return (ft_bswap32(value));
	return (value);
}

static int		browse_section32(t_data *nm_data, struct segment_command *segm)
{
	struct section			*sect;
	int						j;

	sect = (struct section*)(segm + 1);
	j = 0;
	while (j < (int)nm_bsp32(nm_data, segm->nsects))
	{
		if (!trigger_false_pointer(nm_data, (void *)sect))
			return (_ERROR_);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			if (!trigger_false_pointer(nm_data,
						nm_data->ptr + nm_bsp32(nm_data, sect->offset)))
				return (_ERROR_);
			print_memory32(nm_bsp32(nm_data, sect->addr), (unsigned char *)
					nm_data->ptr + nm_bsp32(nm_data, sect->offset),
					nm_bsp32(nm_data, sect->size), nm_data->has_ppc);
		}
		sect++;
		j++;
	}
	return (_SUCCESS_);
}

static int		browse_segment32(t_data *nm_data)
{
	int						i;
	int						ncmds;
	struct load_command		*lc;
	struct segment_command	*segm;

	ncmds = nm_bsp32(nm_data, ((struct mach_header *)nm_data->header)->ncmds);
	lc = (struct load_command *)nm_data->first_load_command;
	i = -1;
	while (++i < ncmds)
	{
		if (!trigger_false_pointer(nm_data, (void *)lc))
			return (_ERROR_);
		if (nm_bsp32(nm_data, lc->cmd) == LC_SEGMENT)
		{
			segm = (struct segment_command *)lc;
			if (ft_strcmp(segm->segname, SEG_TEXT) == 0)
				browse_section32(nm_data, segm);
		}
		lc = (void *)lc + nm_bsp32(nm_data, lc->cmdsize);
	}
	return (_SUCCESS_);
}

int				handle_magic32(t_data *nm_data, char *ptr)
{
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)ptr;
	nm_data->header = (void *)header;
	if (nm_data->arch_name && nm_data->nfat_arch > 1)
		ft_printf("%s (architecture %s):\n", nm_data->file_name,
				nm_data->arch_name);
	else
		ft_printf("%s:\n", nm_data->file_name);
	ft_printf("Contents of (__TEXT,__text) section\n");
	lc = (void *)ptr + sizeof(struct mach_header);
	if (!trigger_false_pointer(nm_data, (void *)lc))
		return (_ERROR_);
	nm_data->first_load_command = lc;
	return (browse_segment32(nm_data));
}
