/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 11:08:05 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/11 14:59:05 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

static void		handle_ar_obj(t_data *nm_data, void *ptr, int ptr_offset,
		char *obj_name)
{
	t_data		sub_nm_data;

	trigger_false_pointer(nm_data, ptr);
	trigger_false_pointer(nm_data, ptr + ptr_offset);
	trigger_false_pointer(nm_data, obj_name);
	sub_nm_data.ptr = ptr;
	sub_nm_data.ptr_offset = ptr_offset;
	sub_nm_data.file_name = nm_data->file_name;
	sub_nm_data.obj_name = obj_name;
	sub_nm_data.n_file = ++nm_data->n_file;
	sub_nm_data.nlist64_list = NULL;
	sub_nm_data.nlist32_list = NULL;
	if (ft_strcmp(obj_name, SYMDEF_SORTED) != 0)
	{
		ft_nm(&sub_nm_data, sub_nm_data.ptr);
		free_nm_data(&sub_nm_data);
	}
}

void			handle_ar(t_data *nm_data, char *ptr)
{
	struct ar_hdr	*ar;
	char			*obj_name;
	int				jump;
	int				padding_offset;

	ptr += SARMAG;
	while (ptr < nm_data->ptr + nm_data->ptr_offset)
	{
		ar = (struct ar_hdr *)ptr;
		trigger_false_pointer(nm_data, (void *)ar);
		jump = 0;
		padding_offset = 0;
		if (ft_strncmp(ar->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		{
			obj_name = (void *)ar + sizeof(struct ar_hdr);
			jump += ft_atoi((char *)ar + ft_strlen(AR_EFMT1));
			padding_offset = jump;
		}
		else
			obj_name = ar->ar_name;
		jump += sizeof(struct ar_hdr);
		handle_ar_obj(nm_data, ptr + jump,
				ft_atoi(ar->ar_size) - padding_offset, obj_name);
		ptr += ft_atoi(ar->ar_size) + sizeof(struct ar_hdr);
	}
}
