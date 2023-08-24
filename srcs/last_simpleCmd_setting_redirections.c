/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_simpleCmd_setting_redirections.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:28 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:35:37 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"


void	ft_open_infiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd)
{
	int flag_random_heredoc;

	flag_random_heredoc = 0;
	while(set->j < cmd->simpleCmds[set->i]->nb_of_infile)
	{	
		if(set->j != 0 && set->fdin)//TODO proteger des pbs a l ouverture
			{if(flag_random_heredoc == 1)
				flag_random_heredoc = 0;
			else
				close(set->fdin);
			}
		if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 1)
			flag_random_heredoc = 1;	
		if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 42)
			set->fdin = open(".heredoc", O_RDONLY); //open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);
		else
			{
				if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == -1)
					set->fdin = open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
			}
		if(set->fdin == -1) //ft_check open error quand on refactorisera plus tard
		{
			ft_error_msg(cmd->simpleCmds[set->i]->infile[set->j]);
			if(cmd->simpleCmds[set->i]->outfile != NULL && cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile != 0)
			{
				size_t k = 0;
				while(k < cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
					{
						ft_open_outfiles_in_last_but_not_first_simpleCmd(set, cmd, k);
						k++;
					}	
			}
			(set->i)++; //TODO voir si completer idem avec first_simple_cmd
			set->j = 0;
			break;
			}
		set->j++;
	}
}

void	ft_last_simpleCmd(t_settings *set, t_cmd *cmd)
{
	set->j = 0;
	if (cmd->simpleCmds[set->i]->outfile != NULL && (cmd->simpleCmds[set->i]->nofile == 0))
		{
			while(set->j < cmd->simpleCmds[set->i]->nb_of_outfile)
			{
				ft_open_outfiles(set, cmd);
				(set->j)++;
			}
		}
	else
		set->fdout = dup(set->saveout);
	set->j = 0;
	if (cmd->simpleCmds[set->i]->infile != NULL && set->i != 0)//simpleCmd est la derniere mais pas la 1 ere (d ou le i!=0))
		{
			ft_open_infiles_in_last_but_not_first_simpleCmd(set, cmd);
			//si j != 0 alors on peut dire que un infile doit faire office d origine de lecture a la place du pip[0] donc on devrait rediriger STDIN sur fdin
			if(set->j != 0)
			{
				dup2(set->fdin, STDIN_FILENO);
				close(set->fdin);
			}
			set->j = 0;
		}
}
