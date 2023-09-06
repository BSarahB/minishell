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
	int *heredoc_track_index;

	heredoc_track_index = NULL;
	infile = NULL;
	infile = malloc(sizeof(*infile) * (simpleCmd->nb_of_infile + 1));
	if(!infile)
		return(NULL);

	simpleCmd->heredoc_track_index = ft_init_ctab(&heredoc_track_index, simpleCmd->nb_of_infile, -1);	
	return(infile);
}

char **ft_malloc_outfile_tab(t_simpleCmd *simpleCmd)
{
	char **outfile;
	int *append_track_index;

	append_track_index = NULL;
	outfile = NULL;
	outfile = malloc(sizeof(*outfile) * (simpleCmd->nb_of_outfile + 1));
	if(!outfile)
		return(NULL);

	simpleCmd->append_track_index = ft_init_ctab(&append_track_index, simpleCmd->nb_of_outfile, -1);
	//outfile[simpleCmd->nb_of_outfile + 1] = 0; // val invalid write
	//TODO proteger if not append_track_index : return NULL
	return(outfile);
}

char **ft_malloc_heredoc_tab(t_simpleCmd *simpleCmd)
{
	char **heredoc;

	heredoc = NULL;
	heredoc = malloc(sizeof(*heredoc) * (simpleCmd->nb_of_heredoc + 1));
	if(!heredoc)
		return(NULL);
	
	return(heredoc);
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
		if(simpleCmd->nb_of_heredoc != 0)
			simpleCmd->heredoc = ft_malloc_heredoc_tab(simpleCmd);
	}
}
