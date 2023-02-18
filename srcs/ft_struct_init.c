/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_struct_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:11:19 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/13 14:11:21 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memset(void *b, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		((char *)b)[i++] = c;
	return (b);
}


char	*ft_init_cstring(char **str, size_t len, char init_value)
{
	*str = malloc(sizeof(char) * (len + 1));
	if (!(*str))
		return (NULL);
	if (len > 0)
		ft_memset(*str, init_value, len);
	(*str)[len] = '\0';
	return (*str);
}

t_simpleCmd	*ft_struct2_init(t_simpleCmd **simpleCmd, char init_value)
{
	(void)init_value;
	*simpleCmd = (t_simpleCmd *)malloc(sizeof(t_simpleCmd));
	if (!(*simpleCmd))
		return (0);

	(*simpleCmd)->number_of_arguments = 0;
//	(*simpleCmd)->errnum = 0;

	(*simpleCmd)->cmd_and_args = NULL;
	(*simpleCmd)->abs_cmd_and_args = NULL;

	return (*simpleCmd);
}


t_cmd	*ft_struct_init(t_cmd **cmd, char init_value, size_t simpleCmds_nbr)
{

	size_t k;

	k = 0;
	t_simpleCmd	*simpleCmd;

	(void)init_value;

	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!(*cmd))
		return (NULL);

	//(*cmd)->simpleCmd = malloc(sizeof(t_simpleCmd));
	(*cmd)->simpleCmds = malloc(sizeof(t_simpleCmd) * (simpleCmds_nbr + 1));
	if (!((*cmd)->simpleCmds))
		return (NULL);
	while(k < simpleCmds_nbr)
	{
		(*cmd)->simpleCmds[k] = ft_struct2_init(&simpleCmd, 0);
		k++;
	}

	(*cmd)->number_of_simpleCmds = simpleCmds_nbr;
	(*cmd)->background = 0;

	(*cmd)->path_tab = 0;
	(*cmd)->outfile = NULL;
	(*cmd)->inputfile = NULL;
	(*cmd)->errfile = NULL;
//	(*cmd)->outfile = ft_init_cstring(&((*cmd)->outfile), 0, init_value);
//	(*cmd)->inputfile = ft_init_cstring(&((*cmd)->inputfile), 0, init_value);
//	(*cmd)->errfile = ft_init_cstring(&((*cmd)->errfile), 0, init_value);
	return (*cmd);
}

