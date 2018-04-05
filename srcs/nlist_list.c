/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/05 11:35:28 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/05 12:18:25 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

t_nmlist64		*new_nmlist64(struct nlist64 *nl, t_nmlist64 *n,
		t_nmlist64 *b, char *s)
{
	t_nmlist64		*item;

	item = (t_nmlist64 *)ft_memalloc(sizeof(t_nmlist64));
	item->next = n;
	item->back = b;
	item->ptr = nl;
	item->str = s;
	return (item);
}

void			insert_nmlist64(t_nmlist64 *item, struct nlist64 *nl, char *str)
{
	t_nmlist64 *back_item;

	back_item = item->back;
	
}

void			add_nlist64(struct nlist64 *nl, t_nmlist64 **begin, char *str)
{
	t_nmlist64		*item;
	int				cmp;

	if (*begin == NULL)
		*begin = new_nmlist64(nl, NULL, NULL, str);
	else
	{
		item = *begin;
		while (item->next)
		{
			if ((cmp = ft_strcmp(item->str, str)) == 0)
				; // function doubling
			else if (cmp < 1)
				; // continuing
			else if (cmp > 1)
			{
				if (item == *begin)
				{
					item->back = new_nmlist64(nl, item, NULL, str);
					*begin = item->back;
				}
				insert_nmlist64(item, nl, str);
				return ;
			}
		}
	}
}
