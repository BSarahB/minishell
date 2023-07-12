/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 13:56:21 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/07/03 13:56:25 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_close_error(int fd)
{
	if (close(fd) == -1)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

void	ft_error(char *const str)
{
	ft_putstr_fd(str, 2);
}

void	ft_error_msg(char *infile)
{
	ft_error("minishell: ");
	ft_error(infile);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error(": no such file or directory: ");
	ft_error("\n");
}

	//GESTION DE CAS OU INFILE OU OUTFILE S OUVRE PAS
int	ft_check_open_error(int fdin, int fdout, t_cmd *cmd)
{
	(void)cmd;
	if (fdin == -1)//si c est une erreur sur l ouverture open de infile, 
	{
		return (1);
	}
	if (fdout == -1) //si il y a un pb sur l open de l outfile on degage, on affiche le perror on free on a tt compris, et basta la vista
	{
		perror("minishell");
	//	ft_free_struct_t_cmd(&cmd); 
		return (2);
	}
	return (0);
}