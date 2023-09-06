/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:00:26 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:00:36 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 int	ft_exit_status(pid_t last_pid, t_settings *set)
 {
	pid_t	wpid;
	int		wstatus;
	int		exit_status;

	exit_status = 0;
	while(1)
	{
		wpid = waitpid(-1, &wstatus, 0);
		//wpid = wait(&wstatus);
		if(wpid < 0)
			break;
		if(wpid == last_pid)
		{
			if(WIFEXITED(wstatus))
				exit_status = WEXITSTATUS(wstatus);
			else
				exit_status = 128 + WTERMSIG(wstatus);
		}
	}
	set->exit_status = exit_status;
	return(exit_status);
 }