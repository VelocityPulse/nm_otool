/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 17:35:17 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H_

#include "../libft/libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

#include "../../Perso/debug.h"

//# define ARCH_32 1
//# define ARCH_64 2

# define MAGIC 0
# define CIGAM 1

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
	char			endian:1;
	void			*header;
	void			*first_load_command;
	t_nmlist64		*nlist64_list;
	t_nmlist32		*nlist32_list;
}				t_data;

void	trigger_false_pointer(t_data *nm_data, char *ptr);

void	handle_magic64(t_data *nm_data, char *ptr);
void	print_output64(t_data *nm_data);
void	add_nlist64(struct nlist_64 *nl, t_nmlist64 **begin, char *str);
void	free_nlist64(t_nmlist64 **begin);

void	handle_magic32(t_data *nm_data, char *ptr);
void	print_output32(t_data *nm_data);
void	add_nlist32(struct nlist *nl, t_nmlist32 **begin, char *str);
void	free_nlist32(t_nmlist32 **begin);

#endif
