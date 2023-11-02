/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 20:49:21 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 20:49:34 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_error(char *const str)
{
	ft_putstr_fd(str, 2);
}

void	ft_error_msg6(char *infile)
{
	ft_error("env: ’");
	ft_error(infile);
	ft_error("’: No such file or directory");
	ft_error("\n");
}

void	ft_error_msg5(char *infile)
{
	ft_error("minishell: ");
	ft_error("cd: ");
	ft_error(infile);
	ft_error(": No such file or directory");
	ft_error("\n");
}

void	ft_error_msg4(char *str)
{
	ft_error("minishell: ");
	ft_error(str);
	ft_error(": too many arguments");
	ft_error("\n");
}
