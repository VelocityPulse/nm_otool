/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/04 14:28:32 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

char	handle_symtab_sect_64(t_data *nm_data, char n_sect)
{
	
	return (0);
}

void	print_output(t_data *nm_data, int nsyms, int symoff, int stroff)
{
	int					i;
	char				*stringtable;
	char				*ptr;
	struct nlist_64		*array;

	ptr = nm_data->ptr;
	array = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	i = -1;
	while (++i < nsyms)
	{
		char type = array[i].n_type & N_TYPE;
		if (type == N_UNDF)
			type = 'U';
		else if (type == N_ABS)
			type = 'A';
		else if (type == N_SECT)
			type = handle_symtab_sect_64(nm_data, array[i].n_sect);
		else if (type == N_PBUD)
			;
		else if (type == N_INDR)
			type = 'I';
		if (type == 'U')
			ft_printf("                 %c %s\n", type, stringtable + array[i].n_un.n_strx);
		else
			ft_printf("%016llx %c %s\n", array[i].n_value, type, stringtable + array[i].n_un.n_strx);
	}
}

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
		if (lc->cmd == LC_SEGMENT_64)
		{
			struct segment_command_64 *segm;
			segm = (struct segment_command_64 *)lc;
			struct section_64 *sect = (void *)segm + sizeof(*segm);
			j = -1;
			while (++j < segm->nsects)
			{
				n_checked++;
				if (n_checked == n_sect)
					return (sect->sectname);
				sect = (void *)sect + sect->size;
			}
		}
		lc = (void *)lc + lc->cmdsize;
	}
	return (NULL);
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
	nm_data->first_load_command = lc;
	ncmds = header->ncmds;
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			print_output(nm_data, sym->nsyms, sym->symoff, sym->stroff);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
	}
}



void	ft_nm(t_data *nm_data, char *ptr)
{
	int		magic_number;

	magic_number = *(int *)ptr;
	nm_data->ptr = ptr;
	ft_printf("magic number : %x\n", magic_number);
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
	ft_nm(&nm_data, ptr);
	if (munmap(ptr, buff.st_size) < 0)
	{
		ft_printf("munmap error\n");
		return (EXIT_FAILURE);
	}






















}
