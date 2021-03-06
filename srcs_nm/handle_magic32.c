/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_magic32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:42:51 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/13 12:08:45 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

unsigned int	nm_bsp32(t_data *nm_data, unsigned int value)
{
	if (nm_data->endian == CIGAM)
		return (ft_bswap32(value));
	return (value);
}

static int		check_nlists32(t_data *nm_data, struct nlist *array,
		char *stringtable, int i)
{
	if (!trigger_false_pointer(nm_data, (void *)&array[i]) ||
			!trigger_false_pointer(nm_data, (void *)stringtable +
				nm_bsp32(nm_data, array[i].n_un.n_strx)))
		return (_ERROR_);
	if ((array[i].n_type & N_STAB) == 0)
		add_nlist32(&array[i], &nm_data->nlist32_list,
				stringtable + nm_bsp32(nm_data, array[i].n_un.n_strx));
	return (_SUCCESS_);
}

static int		browse_nlists32(t_data *nm_data, int nsyms, int symoff,
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
	if (!trigger_false_pointer(nm_data, (void *)array) ||
			!trigger_false_pointer(nm_data, stringtable))
		return (_ERROR_);
	i = -1;
	while (++i < nsyms)
	{
		if (!check_nlists32(nm_data, array, stringtable, i))
			return (_ERROR_);
	}
	return (_SUCCESS_);
}

int				handle_magic32(t_data *nm_data, char *ptr)
{
	int						i;
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header *)ptr;
	nm_data->header = (void *)header;
	lc = (void *)ptr + sizeof(struct mach_header);
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)lc))
		return (_ERROR_);
	nm_data->first_load_command = lc;
	while (++i < (int)nm_bsp32(nm_data, header->ncmds))
	{
		if (nm_bsp32(nm_data, lc->cmd) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			browse_nlists32(nm_data, sym->nsyms, sym->symoff, sym->stroff);
			break ;
		}
		lc = (void *)lc + nm_bsp32(nm_data, lc->cmdsize);
		if (!trigger_false_pointer(nm_data, (void *)lc))
			return (_ERROR_);
	}
	order_nlist32_mode(nm_data);
	return (print_output32(nm_data));
}
