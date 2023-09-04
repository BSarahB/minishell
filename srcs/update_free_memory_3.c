/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:52:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:14 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, char *line)
{
	ft_free_struct_str(&line);

	if (cmd != NULL)
		{
			ft_free_struct_t_cmd(&cmd);
			
		}
	else
		{
			if(lst_token)
				ft_free_struct_t_list_lst_token(&lst_token);
		}

	ft_free_struct_t_data(&data);
}
