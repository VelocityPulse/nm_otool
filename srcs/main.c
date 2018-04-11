/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/11 15:40:21 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	free_nm_data(t_data *nm_data)
{
	free_nlist64(&nm_data->nlist64_list);
	free_nlist32(&nm_data->nlist32_list);
}

void	ft_nm(t_data *nm_data, char *ptr)
{
	int		magic_number;

	magic_number = *(int *)ptr;
	nm_data->ptr = ptr;
	nm_data->endian = MAGIC;
	if (magic_number == FAT_MAGIC_64)
		return ((void)handle_fat64(nm_data, ptr));
	if (magic_number == FAT_MAGIC)
		return ((void)handle_fat32(nm_data, ptr));
	if (magic_number == MH_MAGIC_64)
		return ((void)handle_magic64(nm_data, ptr));
	if (magic_number == MH_MAGIC)
		return ((void)handle_magic32(nm_data, ptr));
	nm_data->endian = CIGAM;
	if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
		return ((void)handle_ar(nm_data, ptr));
	if (magic_number == FAT_CIGAM_64)
		return ((void)handle_fat64(nm_data, ptr));
	if (magic_number == FAT_CIGAM)
		return ((void)handle_fat32(nm_data, ptr));
	if (magic_number == MH_CIGAM_64)
		return ((void)handle_magic64(nm_data, ptr));
	if (magic_number == MH_CIGAM)
		return ((void)handle_magic32(nm_data, ptr));
}

/*
** faire le little-endian avec le ppc-only qui est juste un macho-o tiré de
** audiodevice
**
** les fat contiennent juste plusieurs macho-o simple
**
** fat header : pour dire que c'est un fat
** fat arch : pour dire combien il y a de mach-o
** Structure Universal Binaries (Fat files):
** -----------------------------------
** |   - Fat Header                  |
** |   - Fat Arch                    |
** -----------------------------------
** -----------------------------------
** |   - Mach-O Header               |
** |                                 |
** ----------------------------------
**
** les archies : .a et .so se parsent de la meme maniere
** regroupe tout pleins de mach-o
**
** Structure Archive (build with ar, example with an archive composed of
** 4 object file):
** ----------------------------------
** |   - char [8]magic -> "!<arch>\n"|
** -----------------------------------
** -----------------------------------
** |   - struct ar_hdr               |
** |   - Mach-O                      |
** -----------------------------------
*/

void	init_nm_data(t_data *nm_data, int offset, char *file_name,
		int n_file)
{
	nm_data->ptr_offset = offset;
	nm_data->nlist64_list = NULL;
	nm_data->nlist32_list = NULL;
	nm_data->file_name = file_name;
	nm_data->obj_name = NULL;
	nm_data->n_file = n_file;
}

void	handle_file(char *path, int n_file)
{
	void			*ptr;
	int				fd;
	struct stat		buff;
	t_data			nm_data;

	if ((fd = open(path, O_RDONLY)) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : open fail\n", path));
	if (fstat(fd, &buff) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : stat fail\n", path));
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
			MAP_FAILED)
		return ((void)ft_printf("\nERROR in file [%s] : mmap fail\n", path));
	init_nm_data(&nm_data, buff.st_size, path, n_file);
	ft_nm(&nm_data, ptr);
	free_nm_data(&nm_data);
	if (munmap(ptr, buff.st_size) < 0)
		return ((void)ft_printf("\nERROR in file [%s] : unmmap fail\n", path));

}

int		main(int argc, char **argv)
{
	int				fd;
	int				i;

	if (argc < 2)
	{
		ft_printf("put an argument\n");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (++i < argc)
		handle_file(argv[i], argc - 1);

}
