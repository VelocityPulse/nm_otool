/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:14 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/02 16:30:31 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

void	handle_magic_64(char *ptr)
{
	
}

void	ft_nm(char *ptr)
{
	int		magic_number;

	magic_number = *(int *)ptr;
	ft_printf("magic number : %x\n", magic_number);
	if (magic_number == MH_MAGIC_64) 
		; // TODO here
	else if (magic_number == MH_MAGIC)
		; // TODO here
}

int		main(int argc, char **argv)
{
	int				fd;
	char			*ptr;
	struct stat		buff;

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
	ft_nm(ptr);
	if (munmap(ptr, buff.st_size) < 0)
	{
		ft_printf("munmap error\n");
		return (EXIT_FAILURE);
	}






















}
