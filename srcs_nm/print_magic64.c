/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_magic64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 13:35:51 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 17:31:18 by cchameyr         ###   ########.fr       */
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
	while (j < (int)nm_bsp64(nm_data, segm->nsects))
	{
		if (!trigger_false_pointer(nm_data, (void *)sect))
			return ((void *)-1);
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
	int					ncmd;
	char				*str;
	struct load_command	*lc;

	ncmd = nm_bsp64(nm_data, ((struct mach_header_64 *)nm_data->header)->ncmds);
	lc = (struct load_command *)nm_data->first_load_command;
	i = -1;
	n_checked = 0;
	while (++i < ncmd)
	{
		if (!trigger_false_pointer(nm_data, (void *)lc))
			return ((char *)-1);
		if (nm_bsp64(nm_data, lc->cmd) == LC_SEGMENT_64)
		{
			str = browse_section64(lc, nm_data, &n_checked, n_sect);
			if (str == (char *)-1)
				return ((char *)-1);
			if (str != NULL)
				return (str);
		}
		lc = (void *)lc + nm_bsp64(nm_data, lc->cmdsize);
	}
	return (NULL);
}

static char		handle_symtab_sect64(t_data *nm_data, char n_sect)
{
	char	*sectname;

	sectname = browse_segment64(nm_data, n_sect);
	if (sectname == (char *)-1)
		return (-1);
	if (!sectname)
	{
		ft_printf("ERROR in file [%s] : segment not found\n",
				nm_data->file_name);
		return (-1);
	}
	if (ft_strncmp(sectname, SECT_TEXT, ft_strlen(SECT_TEXT)) == 0)
		return ('T');
	if (ft_strncmp(sectname, SECT_DATA, ft_strlen(SECT_DATA)) == 0)
		return ('D');
	if (ft_strncmp(sectname, SECT_BSS, ft_strlen(SECT_BSS)) == 0)
		return ('B');
	return ('S');
}

static int		help_print64(t_data *nm_data, t_nmlist64 *list, int type)
{
	if (type == N_UNDF)
		type = 'U';
	else if (type == N_ABS)
		type = 'A';
	else if (type == N_SECT)
		type = handle_symtab_sect64(nm_data, list->ptr->n_sect);
	else if (type == N_INDR)
		type = 'I';
	if (type == -1)
		return (_ERROR_);
	if (!(list->ptr->n_type & N_EXT))
		type += 32;
	if ((type == 'U' || type == 'u') && !nm_data->u_maj)
	{
		ft_printf("                 %c ", type);
		print_by_security_endl(nm_data, list->str);
	}
	else if ((type != 'U' && type != 'u') && !nm_data->u)
	{
		ft_printf("%016llx %c ", nm_bsp64(nm_data, list->ptr->n_value), type);
		print_by_security_endl(nm_data, list->str);
	}
	return (_SUCCESS_);
}

int				print_output64(t_data *nm_data)
{
	t_nmlist64	*list;

	list = nm_data->nlist64_list;
	if (nm_data->arch_name != NULL && nm_data->has_i386 &&
			nm_data->nfat_arch == 1)
		ft_printf("%s:\n", nm_data->file_name);
	if (nm_data->obj_name != NULL)
	{
		ft_printf("\n%s(", nm_data->file_name);
		print_by_security(nm_data, nm_data->obj_name);
		ft_putstr("):\n");
	}
	else if (nm_data->arch_name != NULL && nm_data->nfat_arch > 1)
		ft_printf("\n%s (for architecture %s):\n", nm_data->file_name,
				nm_data->arch_name);
	else if (nm_data->n_file > 1)
		ft_printf("\n%s:\n", nm_data->file_name);
	while (list)
	{
		if (!help_print64(nm_data, list, list->ptr->n_type & N_TYPE))
			return (_ERROR_);
		list = list->next;
	}
	return (_SUCCESS_);
}
