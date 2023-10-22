/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:46:20 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/13 11:46:33 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_builtin(void)
{
	
}


void	ft_cd_builtin(void)
{
	
}


void	ft_pwd_builtin(void)
{
	
}

void    ft_close(int *fd)
{
    if (fd)
    {
        if (*fd > 2)
        {
            close(*fd);
            *fd = 0;
        }
    }
}

void    ft_close_pipe(int pipe[2])
{
    ft_close(&pipe[0]);
    ft_close(&pipe[1]);
}
