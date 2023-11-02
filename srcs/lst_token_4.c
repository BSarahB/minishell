/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_token_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 01:45:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 01:46:10 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_aff_listenv_ptr_sur_char_content(t_listenvp *alst)
{
	printf("list diplayed: \n");
	while (alst)
	{
		printf("<%s>\n", (char *)(alst)->key_value);
		alst = (alst)->next;
	}
	printf("end_list dipslay\n");
}
