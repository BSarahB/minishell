/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:45:05 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/13 11:45:28 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo_builtin()
{

}

void ft_execute_builtin(t_cmd *cmd, int i, char **builtin)
{
	if(strcmp(builtin[0], cmd->simpleCmds[i]->cmd_and_args[0]) == 0)
		ft_echo_builtin();

}

int	ft_check_builtin(t_cmd *cmd, int i, char **builtin)
{
	int	j;

	j = 0;
	while(builtin[j])
	{
		if(strcmp(builtin[j], cmd->simpleCmds[i]->cmd_and_args[0]) == 0)
			return(1);
		j++;
	}
	return(0);
}