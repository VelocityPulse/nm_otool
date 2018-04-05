/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/05 11:35:28 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/05 13:57:39 by cchameyr         ###   ########.fr       */
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

void			insert_nmlist64(t_nmlist64 *item, struct nlist_64 *nl, char *str)
{
	t_nmlist64 *insert;

	insert = new_nmlist64(nl, item, item->back, str);
	item->back->next = insert;
	item->back = insert;
}

void			manage_doubling(struct nlist_64 *nl, t_nmlist64 *dbl, t_nmlist64 **begin, char *str)
{
	t_nmlist64	*item;

	if (dbl == *begin && dbl->ptr->n_value > nl->n_value)
	{
		// put nl instead begin
		return ;
	}
	if (dbl->ptr->n_value > nl->n_value)
	{
		// put nl before dbl
		return ;
	}
	item = dbl;
	while (item->next && ft_strcmp(item->next->str, str) == 0 && item->next->ptr->n_value < nl->n_value)
		item = item->next;
	// put nl after item
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
		while (item->next)
		{
			if ((cmp = ft_strcmp(item->str, str)) == 0)
			{
				manage_doubling(nl, item, begin, str);
				return ;
			}
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
			item = item->next;
		}
		item->next = new_nmlist64(nl, NULL, item, str);
	}
}
