/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/09 16:05:08 by cchameyr         ###   ########.fr       */
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
		;
	if (magic_number == FAT_MAGIC)
		;
	if (magic_number == MH_MAGIC_64)
		return ((void)handle_magic64(nm_data, ptr));
	if (magic_number == MH_MAGIC)
		return ((void)handle_magic32(nm_data, ptr));
	nm_data->endian = CIGAM;
	if (magic_number == FAT_CIGAM_64)
		;
	if (magic_number == FAT_CIGAM)
		return ((void)handle_fat32(nm_data, ptr));
	if (magic_number == MH_CIGAM_64)
		return ((void)handle_magic64(nm_data, ptr));
	if (magic_number == MH_CIGAM)
		return ((void)handle_magic32(nm_data, ptr));
}

/*
 * TODO
 * faire le little-endian avec le ppc-only qui est juste un macho-o tiré de 
 * audiodevice.
 *
 * les fat contiennent juste plusieurs macho-o simple
 *
 * fat header : pour dire que c'est un fat 
 * fat arch : pour dire combien il y a de mach-o
 * Structure Universal Binaries (Fat files):
 -----------------------------------
 |   - Fat Header                  |
 |   - Fat Arch                    |
 -----------------------------------
 -----------------------------------
 |   - Mach-O Header               |
 |                                 |
 ----------------------------------
 *
 * les archies : .a et .so se parsent de la meme maniere
 * regroupe tout pleins de mach-o
 *
 *
 * Structure Archive (build with ar, example with an archive composed of 4 object file):
 ----------------------------------
 |   - char [8]magic -> "!<arch>\n"|
 -----------------------------------
 -----------------------------------
 |   - struct ar_hdr               |
 |   - Mach-O                      |
 -----------------------------------
 *
 *
*/

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
	nm_data.nlist32_list = NULL;
	ft_nm(&nm_data, ptr);
	if (munmap(ptr, buff.st_size) < 0)
	{
		ft_printf("munmap error\n");
		return (EXIT_FAILURE);
	}
	free_nm_data(&nm_data);
}
