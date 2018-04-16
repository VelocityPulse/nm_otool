/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/16 09:56:17 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

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

typedef struct		s_nmlist64
{
	struct nlist_64		*ptr;
	struct s_nmlist64	*next;
	struct s_nmlist64	*back;
	char				*str;
}					t_nmlist64;

typedef struct		s_nmlist32
{
	struct nlist		*ptr;
	struct s_nmlist32	*next;
	struct s_nmlist32	*back;
	char				*str;
}					t_nmlist32;

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
	char			r;
	char			n;
	char			u;
	char			u_maj;
	void			*header;
	void			*first_load_command;
	t_nmlist64		*nlist64_list;
	t_nmlist32		*nlist32_list;
}					t_data;

void				ft_nm(t_data *nm_data, char *ptr);
void				set_endian(t_data *nm_data, unsigned int magic_number);
void				free_nm_data(t_data *nm_data);

int					trigger_false_pointer(t_data *nm_data, char *ptr);

int					handle_magic64(t_data *nm_data, char *ptr);
int					print_output64(t_data *nm_data);
void				add_nlist64(struct nlist_64 *nl, t_nmlist64 **begin,
		char *str);
void				free_nlist64(t_nmlist64 **begin);
t_nmlist64			*new_nmlist64(struct nlist_64 *nl, t_nmlist64 *n,
		t_nmlist64 *b, char *s);
unsigned long int	nm_bsp64(t_data *nm_data, unsigned long int value);
void				order_nlist64_mode(t_data *nm_data);

int					handle_magic32(t_data *nm_data, char *ptr);
int					print_output32(t_data *nm_data);
void				add_nlist32(struct nlist *nl, t_nmlist32 **begin,
		char *str);
void				free_nlist32(t_nmlist32 **begin);
t_nmlist32			*new_nmlist32(struct nlist *nl, t_nmlist32 *n,
		t_nmlist32 *b, char *s);
unsigned int		nm_bsp32(t_data *nm_data, unsigned int value);
void				order_nlist32_mode(t_data *nm_data);

int					handle_fat64(t_data *nm_data, char *ptr);
int					start_arch64(t_data *nm_data, char *ptr, int offset,
		struct fat_arch_64 *fa);
int					handle_fat32(t_data *nm_data, char *ptr);
int					start_arch32(t_data *nm_data, char *ptr, int offset,
		struct fat_arch *fa);

int					handle_ar(t_data *nm_data, char *ptr);

#endif
