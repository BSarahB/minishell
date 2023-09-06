/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:06:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:06:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_save_in_and_out(t_settings *set)
{
	set->savein = dup(0);//dup(STDIN_FILENO);
	set->saveout = dup(1);//dup(STDOUT_FILENO);
}

void	ft_restore_original_in_and_out(t_settings *set)
{
	dup2(set->savein, 0);
	dup2(set->saveout, 1);
	close(set->savein);
	close(set->saveout);
}