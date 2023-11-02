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

int	find_length(int n)
{
	int		length;

	if (n <= 0)
		length = 1;
	else
		length = 0;
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
	result = (char *)malloc(sizeof(*result) * length + 1);
	if (!result)
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
	char	*result;

	if (line_count == 0)
		line_count = 1;
	ft_error("minishell: warning: here-document at line ");
	result = ft_itoa(line_count);
	ft_error(result);
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

int	ft_check_open_error(int fdin, int fdout, t_cmd *cmd)
{
	(void)cmd;
	if (fdin == -1)
		return (1);
	if (fdout == -1)
	{
		perror("minishell");
		return (2);
	}
	return (0);
}
