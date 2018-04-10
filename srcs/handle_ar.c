/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 11:08:05 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/10 16:36:52 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"


void	handle_ar(t_data *nm_data, char *ptr)
{
	struct ar_hdr	*ar;
	char			*obj_name;
	int				jump;
	t_data			sub_nm_data;

	ptr += SARMAG;
	ar = (struct ar_hdr *)ptr + SARMAG;
	while (ptr < nm_data->ptr + nm_data->ptr_offset)
	{
		ar = (struct ar_hdr *)ptr;
		jump = 0;
		if (ft_strncmp(ar->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		{
			obj_name = (void *)ar + sizeof(struct ar_hdr);
			jump += ft_atoi(ar->ar_name + ft_strlen(AR_EFMT1));
		}
		else
			obj_name = ar->ar_name;
		jump += sizeof(struct ar_hdr);
		sub_nm_data.ptr = ptr + jump;
		sub_nm_data.obj_name = obj_name;
		sub_nm_data.ptr_offset = ft_atoi(ar->ar_size);
		if (ft_strcmp(obj_name, SYMDEF_SORTED) != 0)
		{
			ft_nm(&sub_nm_data, sub_nm_data.ptr);
			free_nm_data(&sub_nm_data);
		}
		ptr += ft_atoi(ar->ar_size) + sizeof(struct ar_hdr);
	}
}
