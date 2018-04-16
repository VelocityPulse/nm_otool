/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 14:58:39 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <ar.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <sys/mman.h>
# include <sys/stat.h>

# define MAGIC 0
# define CIGAM 1

# define UNMAPPED 0
# define MAPPED 1

typedef struct		s_data
{
	char			*ptr;
	int				ptr_offset;
	char			endian:2;
	char			mapped:2;
	int				n_file;
	int				nfat_arch;
	char			*file_name;
	char			*arch_name;
	char			*obj_name;
	char			has_x86_64;
	char			has_i386;
	void			*header;
	void			*first_load_command;
}					t_data;

void				ft_nm(t_data *nm_data, char *ptr);
void				set_endian(t_data *nm_data, unsigned int magic_number);

int					trigger_false_pointer(t_data *nm_data, char *ptr);

void				print_memory64(long int addr, void *begin, int size);
void				print_memory32(long int addr, void *begin, int size);

int					handle_magic64(t_data *nm_data, char *ptr);
unsigned long int	nm_bsp64(t_data *nm_data, unsigned long int value);

int					handle_magic32(t_data *nm_data, char *ptr);
unsigned int		nm_bsp32(t_data *nm_data, unsigned int value);

int					handle_fat64(t_data *nm_data, char *ptr);
int					start_arch64(t_data *nm_data, char *ptr, int offset,
		struct fat_arch_64 *fa);
int					handle_fat32(t_data *nm_data, char *ptr);
int					start_arch32(t_data *nm_data, char *ptr, int offset,
		struct fat_arch *fa);

int					handle_ar(t_data *nm_data, char *ptr);

#endif
