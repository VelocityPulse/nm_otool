/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist64_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/05 14:36:34 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/05 17:19:24 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

t_nmlist64		*new_nmlist64(struct nlist_64 *nl, t_nmlist64 *n,
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

void			insert_before_nmlist64(t_nmlist64 *item, struct nlist_64 *nl, char *str)
{
	t_nmlist64		*insert;

	insert = new_nmlist64(nl, item, item->back, str);
	if (item->back)
		item->back->next = insert;
	else
		ft_printf("should not pass here !(there is no back) %s\n", __FUNCTION__);
	item->back = insert;
}

void			manage_doubling(struct nlist_64 *nl, t_nmlist64 *dbl, t_nmlist64 **begin, char *str)
{
	t_nmlist64	*item;

	if (dbl == *begin && dbl->ptr->n_value > nl->n_value)
	{
		// put nl instead begin
		(*begin)->back = new_nmlist64(nl, *begin, NULL, str);
		*begin = (*begin)->back;
		return ;
	}
	if (dbl->ptr->n_value > nl->n_value)
	{
		// put nl before dbl
		insert_before_nmlist64(dbl, nl, str);
		return ;
	}
	item = dbl;
	while (item->next && ft_strcmp(item->next->str, str) == 0 && item->next->ptr->n_value < nl->n_value)
		item = item->next;
	// put nl after item
	item->next = new_nmlist64(nl, item->next, item, str);
	if (item->next->next)
		item->next->next->back = item->next;
}

void			add_nlist64(struct nlist_64 *nl, t_nmlist64 **begin, char *str)
{
	t_nmlist64		*item;
	int				cmp;

		if (*begin == NULL)
			*begin = new_nmlist64(nl, NULL, NULL, str);
		else
		{
			item = *begin;
			t_nmlist64 *last;
			while (item)
			{
				cmp = ft_strcmp(item->str, str);
				if ((cmp) == 0)
				{
					manage_doubling(nl, item, begin, str);
					return ;
				}
				else if (cmp > 0)
				{
					if (item == *begin)
					{
						item->back = new_nmlist64(nl, item, NULL, str);
						*begin = item->back;
						return ;
					}
					insert_before_nmlist64(item, nl, str);
					return ;
				}
				last = item;
				item = item->next;
			}
				last->next = new_nmlist64(nl, NULL, last, str);
		}
}
