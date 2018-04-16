/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 14:10:16 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

void				ft_nm(t_data *nm_data, char *ptr)
{
	unsigned int	magic_number;

	magic_number = *(unsigned int *)ptr;
	nm_data->ptr = ptr;
	set_endian(nm_data, magic_number);
	if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
		return ((void)handle_fat64(nm_data, ptr));
	if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		return ((void)handle_fat32(nm_data, ptr));
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		return ((void)handle_magic64(nm_data, ptr));
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		return ((void)handle_magic32(nm_data, ptr));
	if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
		return ((void)handle_ar(nm_data, ptr));
	ft_printf("\nERROR in file [%s] : File not receognized\n",
			nm_data->file_name);
}

static void			init_nm_data(t_data *nm_data, int offset, char *file_name,
		int n_file)
{
	nm_data->ptr_offset = offset;
	nm_data->file_name = file_name;
	nm_data->arch_name = NULL;
	nm_data->obj_name = NULL;
	nm_data->has_x86_64 = 0;
	nm_data->has_i386 = 0;
	nm_data->n_file = n_file;
	nm_data->nfat_arch = 0;
}

static void			handle_file(t_data *nm_data, char *path, int n_file)
{
	void			*ptr;
	int				fd;
	struct stat		buff;

	if ((fd = open(path, O_RDONLY)) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : open fail\n", path));
	if (fstat(fd, &buff) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : stat fail\n", path));
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
			MAP_FAILED)
		return ((void)ft_printf("\nERROR in file [%s] : mmap fail\n", path));
	init_nm_data(nm_data, buff.st_size, path, n_file);
	ft_nm(nm_data, ptr);
	if (munmap(ptr, buff.st_size) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : unmmap fail\n", path));
}

static int			handle_option(char *arg, t_data *nm_data, int *n_file)
{
	(void)nm_data;
	if (arg[0] == '-')
	{
		(*n_file)--;
		return (0);
	}
	return (1);
}

int					main(int argc, char **argv)
{
	int				i;
	int				n_file;
	t_data			nm_data;

	ft_bzero((void **)&nm_data, sizeof(t_data));
	if (argc < 2)
	{
		ft_printf("put an argument\n");
		return (EXIT_FAILURE);
	}
	n_file = argc - 1;
	i = 0;
	while (++i < argc)
	{
		if (handle_option(argv[i], &nm_data, &n_file))
			handle_file(&nm_data, argv[i], n_file);
	}
	return (0);
}
