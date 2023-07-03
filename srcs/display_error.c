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

/*
int	ft_check_open_error(t_struct *ptr)
{
	if ((*ptr).fd1 == -1)
	{
		return (1);
	}
	if ((*ptr).fd2 == -1)
	{
		perror("minishell");
		ft_free_t_struct(&ptr);
		return (2);
	}
	return (0);
}
*/
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

void	ft_error_msg(char **argv)
{
	ft_error("minishell: ");
	ft_error("no such file or directory: ");
	ft_error(argv[1]);//ON DOIT REMPLACER ARGV[1] par le nom d INFILE (normalement cela est la 1 ere simple_cmd) (verifier l impact d un infile dans un block)
	ft_error("\n");
}
/*
//SI PB D OUVERTURE DE FD1 ou FD2 (donc association a la simpleCmd fd1(pour scmd 1) et fd2 pour simpleCmd2) cf plus tard
if(ret == 2 || ret == 1)
{ 
	ft_check_fork_fd1();

}

void	ft_check_fork_fd1(t_struct *ptr, char **argv, char **envp, int ret)
{
	pid_t	retour_fd1;

	if (ret == 2)//si pb sur ouverture d outfile on degage(on a deja free on att compris)
		exit(1);

		//GESTION DU CAS D ERREUR D OUVERTURE DE INFILE
	if (ret == 1)//si pb sur ouverture de infile
	{
		ft_error_msg(argv);//on va avoir le message comme quoi infile ne fonctionne pas , dans bash ici bash: infile: Permission denied dans pipex j avais mis pipex: no such file or directory: infile


		ft_create_pipe(ptr);//RETURN VALUE On success, zero is returned.  On error, -1 is returned, errno  is  set appropriately, and pipefd is left unchanged.
		
	void	ft_create_pipe(t_struct *ptr) //TODO CQFD
	{
		if (pipe((*ptr).p)) //si le pipe echoue
		{
			ft_free_t_struct(&ptr);
			perror("pipe");
			exit (0);
		}
	}


}
*/
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

