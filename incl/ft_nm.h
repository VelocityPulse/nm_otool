/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:50:08 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/04 14:27:13 by cchameyr         ###   ########.fr       */
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

typedef struct	s_data
{
	char	*ptr;
	char	arch;
	void	*header;
	void	*first_load_command;
}				t_data;

#endif
