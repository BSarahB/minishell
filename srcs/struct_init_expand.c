/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:13:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/19 10:13:46 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_expand	*ft_struct_init_expand(t_expand **exp)
{
	*exp = (t_expand *)malloc(sizeof (t_expand));
	if (!(*exp))
		return (0);
	(*exp)->quoting_rule_adequate = 0;
	(*exp)->quoting_rule = 0;
	(*exp)->i = 0;
	(*exp)->j = 0;
	(*exp)->flag_expand_here = 0;
	(*exp)->flag_expand_in_token = 0;
	(*exp)->start_expand_pos = 0;
	(*exp)->end_expand_pos = 0;
	(*exp)->flag_dollar_quest = 0;
	(*exp)->flag_dollar_to_remove = 0;
	return (*exp);
}
