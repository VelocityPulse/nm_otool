/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nlist32_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 14:28:59 by cchameyr          #+#    #+#             */
/*   Updated: 2018/04/06 15:31:44 by cchameyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_nm.h"

t_nmlist32			*new_nmlist32(struct nlist *nl, t_nmlist32 *n,
		t_nmlist32 *b, char *s)
{
	t_nmlist32		*item;

	item = (t_nmlist32 *)ft_memalloc(sizeof(t_nmlist32));
	item->next = n;
	item->back = b;
	item->ptr = nl;
	item->str = s;
	return (item);
}

static void			insert_before_nmlist32(t_nmlist32 *item,
		struct nlist *nl, char *str)
{
	t_nmlist32		*insert;

	insert = new_nmlist32(nl, item, item->back, str);
	item->back->next = insert;
	item->back = insert;
}

/*
** manage_doubling
** first if  : put nl instead begin
** second if : put nl before dbl
** third if  : put nl after item
*/

static void			manage_doubling(struct nlist *nl, t_nmlist32 *dbl,
		t_nmlist32 **begin, char *str)
{
	t_nmlist32	*item;

	if (dbl == *begin && dbl->ptr->n_value > nl->n_value)
	{
		(*begin)->back = new_nmlist32(nl, *begin, NULL, str);
		return ((void)(*begin = (*begin)->back));
	}
	if (dbl->ptr->n_value > nl->n_value)
		return ((void)insert_before_nmlist32(dbl, nl, str));
	item = dbl;
	while (item->next && ft_strcmp(item->next->str, str) == 0 &&
			item->next->ptr->n_value < nl->n_value)
		item = item->next;
	item->next = new_nmlist32(nl, item->next, item, str);
	if (item->next->next)
		item->next->next->back = item->next;
}

static void			manage_insert(struct nlist *nl, t_nmlist32 *item,
		t_nmlist32 **begin, char *str)
{
	if (item == *begin)
	{
		item->back = new_nmlist32(nl, item, NULL, str);
		return ((void)(*begin = item->back));
	}
	return ((void)insert_before_nmlist32(item, nl, str));
}

void				add_nlist32(struct nlist *nl, t_nmlist32 **begin,
		char *str)
{
	t_nmlist32		*item;
	t_nmlist32		*last;
	int				cmp;

	if (*begin == NULL)
		*begin = new_nmlist32(nl, NULL, NULL, str);
	else
	{
		item = *begin;
		while (item)
		{
			if ((cmp = ft_strcmp(item->str, str)) == 0)
				return ((void)manage_doubling(nl, item, begin, str));
			else if (cmp > 0)
				return ((void)manage_insert(nl, item, begin, str));
			last = item;
			item = item->next;
		}
		last->next = new_nmlist32(nl, NULL, last, str);
	}
}
