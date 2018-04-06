/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 12:03:24 by cchameyr         ###   ########.fr       */
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

# define ARCH_32 1
# define ARCH_64 2

typedef struct	s_nmlist64
{
	struct nlist_64		*ptr;
	struct s_nmlist64	*next;
	struct s_nmlist64	*back;
	char				*str;
}				t_nmlist64;

typedef struct	s_nmlist
{
	struct nlist		*ptr;
	struct s_nmlist		*next;
	struct s_nmlist		*back;
	char				*str;
}				t_nmlist;

typedef struct	s_data
{
	char			*ptr;
	int				ptr_offset;
	char			arch;
	void			*header;
	void			*first_load_command;
	t_nmlist64		*nlist64_list;
	t_nmlist		*nlist_list;
}				t_data;

void	trigger_false_pointer(t_data *nm_data, char *ptr);

void	print_output64(t_data *nm_data);
void	handle_magic64(t_data *nm_data, char *ptr);

void	add_nlist64(struct nlist_64 *nl, t_nmlist64 **begin, char *str);

#endif
