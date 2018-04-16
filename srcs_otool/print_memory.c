/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 14:09:44 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 14:59:25 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

void	print_hex_text(unsigned char *ptr, unsigned char *ptr_offset)
{
	int		i;


	i = -1;
	while (++i < 16)
	{
		if (ptr + i < ptr_offset)
		{
			ft_printf("%02x ", ptr[i]);
		}
	}
}

void	print_memory64(long int addr, void *begin, int size)
{
	int				i;
	unsigned char	*adr;

	adr = (unsigned char *)begin;
	i = 0;
	while (i < size)
	{
		ft_printf("%016llx\t", addr + i);
		print_hex_text(adr + i, adr + size);
		ft_putchar('\n');
		i += 16;
	}
}

void	print_memory32(long int addr, void *begin, int size)
{
	int				i;
	unsigned char	*adr;

	adr = (unsigned char *)begin;
	i = 0;
	while (i < size)
	{
		ft_printf("%08llx\t", addr + i);
		print_hex_text(adr + i, adr + size);
		ft_putchar('\n');
		i += 16;
	}

}
