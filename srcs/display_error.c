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

int		find_length(int n)
{
	int length;

	length = (n <= 0) ? 1 : 0;
	while (n != 0)
	{
		length++;
		n = n / 10;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		length;
	long	nb;

	nb = n;
	length = find_length(n);
	if (!(result = (char *)malloc(sizeof(*result) * length + 1)))
		return (NULL);
	result[length] = '\0';
	if (nb < 0)
		result[0] = '-';
	else if (nb == 0)
		result[0] = '0';
	if (nb < 0)
		nb = -nb;
	while (nb != 0)
	{
		--length;
		result[length] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (result);
}

void	ft_error_heredoc(char *const str, int line_count)
{
	char *result;

	if(line_count == 0)
		line_count = 1;
	ft_error("minishell: warning: here-document at line ");
	result = ft_itoa(line_count);
	ft_error(result);//TODO determiner le nombre de lignes quil reste dans le fichier ->comment compter les lignes du fichier.
	ft_error(" delimited by end-of-file (wanted `");
	ft_error(str);
	ft_error("')\n");
	free(result);
}

int	ft_check_close_error(int fd)
{
	if (close(fd) == -1)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}
	//GESTION DE CAS OU INFILE OU OUTFILE S OUVRE PAS
int	ft_check_open_error(int fdin, int fdout, t_cmd *cmd)
{
	(void)cmd;
	if (fdin == -1)
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
