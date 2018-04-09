/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_magic32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:42:51 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/09 10:18:15 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

unsigned int	nm_bsp32(t_data *nm_data, unsigned int value)
{
	if (nm_data->endian == CIGAM)
		return (ft_bswap32(value));
	return (value);
}

static void		browse_nlists32(t_data *nm_data, int nsyms, int symoff,
		int stroff)
{
	int					i;
	char				*stringtable;
	char				*ptr;
	struct nlist		*array;

	ptr = nm_data->ptr;
	array = (void *)ptr + nm_bsp32(nm_data, symoff);
	stringtable = (void *)ptr + nm_bsp32(nm_data, stroff);
	nsyms = nm_bsp32(nm_data, nsyms);
	trigger_false_pointer(nm_data, (void *)array);
	trigger_false_pointer(nm_data, (stringtable));
	i = -1;
	while (++i < nsyms)
	{
		trigger_false_pointer(nm_data, (void *)&array[i]);
		trigger_false_pointer(nm_data, (void *)stringtable +
				nm_bsp32(nm_data, array[i].n_un.n_strx));
		char type = array[i].n_type & N_TYPE;
		if ((array[i].n_type & N_STAB) == 0)
		{
			add_nlist32(&array[i], &nm_data->nlist32_list,
					stringtable + nm_bsp32(nm_data, array[i].n_un.n_strx));
		}
	}
}

void	handle_magic32(t_data *nm_data, char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header *)ptr;
	nm_data->header = (void *)header;
	lc = (void *)ptr + sizeof(struct mach_header);
	trigger_false_pointer(nm_data, (void *)lc);
	nm_data->first_load_command = lc;
	ncmds = nm_bsp32(nm_data, header->ncmds);
	i = -1;
	while (++i < ncmds)
	{
		if (nm_bsp32(nm_data, lc->cmd) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			browse_nlists32(nm_data, sym->nsyms, sym->symoff, sym->stroff);
			break;
		}
		lc = (void *) lc + nm_bsp32(nm_data, lc->cmdsize);
		trigger_false_pointer(nm_data, (void *)lc);
	}
	print_output32(nm_data);
}
