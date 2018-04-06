/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 10:49:25 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

char	*get_section_name_64(t_data *nm_data, char n_sect)
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

char	handle_symtab_sect_64(t_data *nm_data, char n_sect)
{
	char	*sectname;

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
//	if (ft_strncmp(sectname, SECT_COMMON, ft_strlen(SECT_COMMON)) == 0)
//		return 'C';
	return ('S');
}

void	print_output64(t_data *nm_data)
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

void	browse_nlists64(t_data *nm_data, int nsyms, int symoff, int stroff)
{
	int					i;
	char				*stringtable;
	char				*ptr;
	struct nlist_64		*array;

	ptr = nm_data->ptr;
	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	trigger_false_pointer(nm_data, (void *)array);
	trigger_false_pointer(nm_data, (stringtable));
	i = -1;
	while (++i < nsyms)
	{
		trigger_false_pointer(nm_data, (void *)&array[i]);
		trigger_false_pointer(nm_data, (void *)stringtable + array[i].n_un.n_strx);
		char type = array[i].n_type & N_TYPE;
		if ((array[i].n_type & N_STAB) == 0)
		{
			add_nlist64(&array[i], &nm_data->nlist64_list,
					stringtable + array[i].n_un.n_strx);
		}
	}
}

void	handle_magic_64(t_data *nm_data, char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	nm_data->header = (void *)header;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	trigger_false_pointer(nm_data, (void *)lc);
	nm_data->first_load_command = lc;
	ncmds = header->ncmds;
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			browse_nlists64(nm_data, sym->nsyms, sym->symoff, sym->stroff);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
	trigger_false_pointer(nm_data, (void *)lc);
	}
	print_output64(nm_data);
}

void	ft_nm(t_data *nm_data, char *ptr)
{
	int		magic_number;

	magic_number = *(int *)ptr;
	nm_data->ptr = ptr;
	//ft_printf("magic number : %x\n", magic_number);
	if (magic_number == MH_MAGIC_64)
	{
		nm_data->arch = ARCH_64;
		handle_magic_64(nm_data, ptr);
	}
	else if (magic_number == MH_MAGIC)
	{
		nm_data->arch = ARCH_32
			; // TODO here
	}
}

int		main(int argc, char **argv)
{
	int				fd;
	char			*ptr;
	struct stat		buff;
	t_data			nm_data;

	if (argc != 2)
	{
		ft_printf("put an argument\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		ft_printf("error on open\n");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buff) < 0)
	{
		ft_printf("error on stat\n");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_printf("mmap error\n");
		return (EXIT_FAILURE);
	}
	nm_data.ptr_offset = buff.st_size;
	nm_data.nlist64_list = NULL;
	nm_data.nlist_list = NULL;
	ft_nm(&nm_data, ptr);
	if (munmap(ptr, buff.st_size) < 0)
	{
		ft_printf("munmap error\n");
		return (EXIT_FAILURE);
	}
}
