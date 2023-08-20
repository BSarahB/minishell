/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser2_malloc_redir_files.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:28:12 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:28:43 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void *ft_memset2(void *b, int n, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
		((int *)b)[i++] = n;
	return (b);
}

int *ft_init_ctab(int **int_tab, size_t len, int init_value)
{
	*int_tab = malloc(sizeof(int) * (len + 1));
	if (!(*int_tab))
		return (NULL);
	if (len > 0)
		ft_memset2(*int_tab, init_value, len);
	(*int_tab)[len] = '\0';
	return (*int_tab);
}

char **ft_malloc_errfile_tab(t_simpleCmd *simpleCmd)
{
	char **errfile;

	errfile = malloc(sizeof(*errfile) * (simpleCmd->nb_of_errfile + 1));
	if(!errfile)
		return(NULL);
	return(errfile);
}

char **ft_malloc_infile_tab(t_simpleCmd *simpleCmd)
{
	char **infile;

	infile = malloc(sizeof(*infile) * (simpleCmd->nb_of_infile + 1));
	if(!infile)
		return(NULL);
	return(infile);
}

char **ft_malloc_outfile_tab(t_simpleCmd *simpleCmd)
{
	char **outfile;
	int *append_track_index;
	
	outfile = NULL;
	outfile = malloc(sizeof(*outfile) * (simpleCmd->nb_of_outfile + 1));
	if(!outfile)
		return(NULL);

	simpleCmd->append_track_index = ft_init_ctab(&append_track_index, simpleCmd->nb_of_outfile, -1);
	//outfile[simpleCmd->nb_of_outfile + 1] = 0; // val invalid write
	return(outfile);
}




void	ft_malloc_redir_file_tabs_of_simpleCmd(t_simpleCmd *simpleCmd)
{
	if(simpleCmd->nb_of_redir_token != 0)
	{
		if(simpleCmd->nb_of_outfile != 0)
			simpleCmd->outfile = ft_malloc_outfile_tab(simpleCmd);	
		if(simpleCmd->nb_of_infile != 0)
			simpleCmd->infile = ft_malloc_infile_tab(simpleCmd);
		if(simpleCmd->nb_of_errfile != 0)
			simpleCmd->errfile = ft_malloc_errfile_tab(simpleCmd);
	}
}
