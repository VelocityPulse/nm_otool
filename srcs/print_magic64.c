/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_magic64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:35:51 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/11 12:17:38 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

static char		*browse_section64(struct load_command *lc, t_data *nm_data,
		int *n_checked, char n_sect)
{
	struct segment_command_64	*segm;
	struct section_64			*sect;
	int							j;

	segm = (struct segment_command_64 *)lc;
	sect = (struct section_64*)(segm + 1);
	j = 0;
	while (j < nm_bsp64(nm_data, segm->nsects))
	{
		trigger_false_pointer(nm_data, (void *)sect);
		(*n_checked)++;
		if (*n_checked == n_sect)
			return (sect->sectname);
		sect++;
		j++;
	}
	return (NULL);
}

static char		*browse_segment64(t_data *nm_data, char n_sect)
{
	int					i;
	int					n_checked;
	int					ncmds;
	char				*str;
	struct load_command	*lc;

	ncmds = nm_bsp64(nm_data, ((struct mach_header_64 *)nm_data->header)
			->ncmds);
	lc = (struct load_command *)nm_data->first_load_command;
	i = -1;
	n_checked = 0;
	while (++i < ncmds)
	{
		trigger_false_pointer(nm_data, (void *)lc);
		if (nm_bsp64(nm_data, lc->cmd) == LC_SEGMENT_64)
		{
			if ((str = browse_section64(lc, nm_data, &n_checked, n_sect)) !=
					NULL)
				return (str);
		}
		lc = (void *)lc + nm_bsp64(nm_data, lc->cmdsize);
	}
	return (NULL);
}

static char		handle_symtab_sect64(t_data *nm_data, char n_sect)
{
	char    *sectname;

	if ((sectname = browse_segment64(nm_data, n_sect)) == NULL)
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

static void		print_title64(t_data *nm_data)
{
	if (nm_data->obj_name != NULL)
		ft_printf("\n%s(%s):\n", nm_data->file_name, nm_data->obj_name);
	else if (nm_data->n_file > 1)
		ft_printf("\n%s:\n", nm_data->file_name);
}

void			print_output64(t_data *nm_data)
{
	t_nmlist64	*list;
	char		type;

	list = nm_data->nlist64_list;
	print_title64(nm_data);
	while (list)
	{
		type = list->ptr->n_type & N_TYPE;
		if (type == N_UNDF)
			type = 'U';
		else if (type == N_ABS)
			type = 'A';
		else if (type == N_SECT)
			type = handle_symtab_sect64(nm_data, list->ptr->n_sect);
		else if (type == N_INDR)
			type = 'I';
		if (!(list->ptr->n_type & N_EXT))
			type += 32;
		if (type == 'U' || type == 'u')
			ft_printf("                 %c %s\n", type, list->str);
		else
			ft_printf("%016llx %c %s\n", nm_bsp64(nm_data, list->ptr->n_value),
					type, list->str);
		list = list->next;
	}
}
