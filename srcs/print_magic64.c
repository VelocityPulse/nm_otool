/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_magic_64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 11:15:04 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 11:52:57 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

static char		*get_section_name_64(t_data *nm_data, char n_sect)
{
	int					i;
	int					j;
	int					n_checked;
	int					ncmds;
	struct load_command	*lc;

	ncmds = ((struct mach_header_64 *)nm_data->header)->ncmds;
	lc = (struct load_command *)nm_data->first_load_command;
	i = -1;
	n_checked = 0;
	while (++i < ncmds)
	{
		trigger_false_pointer(nm_data, (void *)lc);
		if (lc->cmd == LC_SEGMENT_64)
		{
			struct segment_command_64 *segm;
			segm = (struct segment_command_64 *)lc;
			struct section_64 *sect = (struct section_64*)(segm + 1);
			j = 0;
			while (j < segm->nsects)
			{
				trigger_false_pointer(nm_data, (void *)sect);
				n_checked++;
				if (n_checked == n_sect)
					return (sect->sectname);
				sect++;
				j++;
			}
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (NULL);
}

static char		handle_symtab_sect_64(t_data *nm_data, char n_sect)
{
	char    *sectname;

	if ((sectname = get_section_name_64(nm_data, n_sect)) == NULL)
	{
		ft_putstr("ft_nm error : n_sect not found");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(sectname, SECT_TEXT, ft_strlen(SECT_TEXT)) == 0)
		return 'T';
	if (ft_strncmp(sectname, SECT_DATA, ft_strlen(SECT_DATA)) == 0)
		return 'D';
	if (ft_strncmp(sectname, SECT_BSS, ft_strlen(SECT_BSS)) == 0)
		return 'B';
	return 'S';
}

void			print_output64(t_data *nm_data)
{
	t_nmlist64	*list;
	char		type;

	list = nm_data->nlist64_list;
	while (list)
	{
		type = list->ptr->n_type & N_TYPE;
		if (type == N_UNDF)
			type = 'U';
		else if (type == N_ABS)
			type = 'A';
		else if (type == N_SECT)
			type = handle_symtab_sect_64(nm_data, list->ptr->n_sect);
		else if (type == N_PBUD)
			;
		else if (type == N_INDR)
			type = 'I';
		if (!(list->ptr->n_type & N_EXT))
			type += 32;
		if (type == 'U' || type == 'u')
			ft_printf("                 %c %s\n", type, list->str);
		else
			ft_printf("%016llx %c %s\n", list->ptr->n_value, type, list->str);
		list = list->next;
	}
}
