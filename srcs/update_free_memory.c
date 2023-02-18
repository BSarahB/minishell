/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:10:57 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/13 14:10:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i])
	{
		ft_free_struct_str(&(*tab)[i]);
		i++;
	}
	if (*tab != NULL)
	{
		free(*tab);
		*tab = NULL;
	}
}
