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

void	ft_error(char *const str)
{
	ft_putstr_fd(str, 2);
}


void	ft_error_msg6(char *infile)
{

	ft_error("env: ’");
	ft_error(infile);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error("’: No such file or directory");
	ft_error("\n");
}

void	ft_error_msg5(char *infile)
{
	ft_error("minishell: ");
	ft_error("cd: ");
	ft_error(infile);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error(": No such file or directory");
	ft_error("\n");
}


void	ft_error_msg4(char *str)
{
	ft_error("minishell: ");
	ft_error(str);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error(": too many arguments");
	ft_error("\n");
}

void	ft_error_msg3(char *str)
{
	ft_error("minishell: ");
	ft_error(str);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error(": ambiguous redirect");
	ft_error("\n");
}

void	ft_error_msg2b(char *str)
{
	ft_error("minishell: ");
	ft_error("syntax error near unexpected token `");
	ft_error(str);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error("'");
	ft_error("\n");
}

void	ft_error_msg2(char *str)
{
	ft_error("minishell: ");
	ft_error("syntax error near unexpected token ");
	ft_error(str);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error("\n");
}

void	ft_error_msg(char *infile)
{
	ft_error("minishell: ");
	ft_error(infile);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error(": No such file or directory");
	ft_error("\n");
}
