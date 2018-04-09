/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/09 15:46:38 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H_

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>

# include "../../Perso/debug.h"

# define MAGIC 0
# define CIGAM 1

# define UNMAPPED 0
# define MAPPED 1

typedef struct	s_nmlist64
{
	struct nlist_64		*ptr;
	struct s_nmlist64	*next;
	struct s_nmlist64	*back;
	char				*str;
}				t_nmlist64;

typedef struct	s_nmlist32
{
	struct nlist		*ptr;
	struct s_nmlist32		*next;
	struct s_nmlist32		*back;
	char				*str;
}				t_nmlist32;

typedef struct	s_data
{
	char			*ptr;
	int				ptr_offset;
	char			endian:2;
	char			mapped:2;
	void			*header;
	void			*first_load_command;
	t_nmlist64		*nlist64_list;
	t_nmlist32		*nlist32_list;
}				t_data;

void				ft_nm(t_data *nm_data, char *ptr);

void				trigger_false_pointer(t_data *nm_data, char *ptr);

void				handle_magic64(t_data *nm_data, char *ptr);
void				print_output64(t_data *nm_data);
void				add_nlist64(struct nlist_64 *nl, t_nmlist64 **begin,
		char *str);
void				free_nlist64(t_nmlist64 **begin);
unsigned long int	nm_bsp64(t_data *nm_data, unsigned long int value);

void				handle_magic32(t_data *nm_data, char *ptr);
void				print_output32(t_data *nm_data);
void				add_nlist32(struct nlist *nl, t_nmlist32 **begin,
		char *str);
void				free_nlist32(t_nmlist32 **begin);
unsigned int		nm_bsp32(t_data *nm_data, unsigned int value);

void				handle_fat32(t_data *nm_data, char *ptr);

#endif
