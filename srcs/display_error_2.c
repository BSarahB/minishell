/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:39:39 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 14:39:52 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_error_msg3(char *str)
{
	ft_error("minishell: ");
	ft_error(str);
	ft_error(": ambiguous redirect");
	ft_error("\n");
}

void	ft_error_msg2c(char *str)
{
	ft_error("minishell: ");
	ft_error("syntax error near unexpected token `");
	ft_error(str);
	ft_error(str);
	ft_error("'");
	ft_error("\n");
}

void	ft_error_msg2b(char *str)
{
	ft_error("minishell: ");
	ft_error("syntax error near unexpected token `");
	ft_error(str);
	ft_error("'");
	ft_error("\n");
}

void	ft_error_msg2(char *str)
{
	ft_error("minishell: ");
	ft_error("syntax error near unexpected token ");
	ft_error(str);
	ft_error("\n");
}

void	ft_error_msg(char *infile)
{
	ft_error("minishell: ");
	ft_error(infile);
	ft_error(": No such file or directory");
	ft_error("\n");
}
