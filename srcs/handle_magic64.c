/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_magic64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:35:40 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/12 17:32:02 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

unsigned long int	nm_bsp64(t_data *nm_data, unsigned long int value)
{
	if (nm_data->endian == CIGAM)
		return (ft_bswap64(value));
	return (value);
}

static int			check_nlist64(t_data *nm_data, struct nlist_64 *array,
		char *stringtable, int i)
{
	if (!trigger_false_pointer(nm_data, (void *)&array[i]) ||
			!trigger_false_pointer(nm_data, (void *)stringtable +
				(int)nm_bsp64(nm_data, array[i].n_un.n_strx)))
		return (_ERROR_);
	if ((array[i].n_type & N_STAB) == 0)
		add_nlist64(&array[i], &nm_data->nlist64_list,
				stringtable + nm_bsp64(nm_data, array[i].n_un.n_strx));
	return (_SUCCESS_);

}

static int			browse_nlists64(t_data *nm_data, int nsyms, int symoff,
		int stroff)
{
	int					i;
	char				*stringtable;
	char				*ptr;
	struct nlist_64		*array;

	ptr = nm_data->ptr;
	array = (void *)ptr + nm_bsp64(nm_data, symoff);
	stringtable = (void *)ptr + nm_bsp64(nm_data, stroff);
	nsyms = nm_bsp64(nm_data, nsyms);
	if (!trigger_false_pointer(nm_data, (void *)array) ||
			!trigger_false_pointer(nm_data, stringtable))
		return (_ERROR_);
	i = -1;
	while (++i < nsyms)
	{
		if (!check_nlist64(nm_data, array, stringtable, i))
			return (_ERROR_);
	}
	return (_SUCCESS_);
}

int				handle_magic64(t_data *nm_data, char *ptr)
{
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	nm_data->header = (void *)header;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	if ((i = -1) && !trigger_false_pointer(nm_data, (void *)lc))
		return (_ERROR_);
	nm_data->first_load_command = lc;
	while (++i < (int)nm_bsp64(nm_data, header->ncmds))
	{
		if (nm_bsp64(nm_data, lc->cmd) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			browse_nlists64(nm_data, sym->nsyms, sym->symoff, sym->stroff);
			break ;
		}
		lc = (void *)lc + nm_bsp64(nm_data, lc->cmdsize);
		if (!trigger_false_pointer(nm_data, (void *)lc))
			return (_ERROR_);
	}
	return (print_output64(nm_data));
}
