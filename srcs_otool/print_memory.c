/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 14:09:44 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 16:40:31 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_otool.h"

void	print_hex_text(unsigned char *ptr,
		unsigned char *ptr_offset, int has_ppc)
{
	int		i;
	int		cmp_cpp;

	i = -1;
	cmp_cpp = 0;
	while (++i < 16)
	{
		if (ptr + i < ptr_offset)
		{
			if (!has_ppc)
				ft_printf("%02x ", ptr[i]);
			else
			{
				ft_printf("%02x", ptr[i]);
				if (++cmp_cpp == 4)
				{
					ft_putchar(' ');
					cmp_cpp = 0;
				}
			}
		}
	}
}

void	print_memory64(long int addr, unsigned char *section,
		int size, int has_ppc)
{
	int		i;

	i = 0;
	while (i < size)
	{
		ft_printf("%016llx\t", addr + i);
		print_hex_text(section + i, section + size, has_ppc);
		ft_putchar('\n');
		i += 16;
	}
}

void	print_memory32(long int addr, unsigned char *section,
		int size, int has_ppc)
{
	int				i;

	i = 0;
	while (i < size)
	{
		ft_printf("%08llx\t", addr + i);
		print_hex_text(section + i, section + size, has_ppc);
		ft_putchar('\n');
		i += 16;
	}
}
