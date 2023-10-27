/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_simpleCmd_w_infile_setting_redirect          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:36:31 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:36:37 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_create_pipe(t_settings *set)
{
    int pip[2];
	if (pipe(pip) == -1)
	{
        perror("pipe");
		//il faut tout free ici
        exit(EXIT_FAILURE);
    }
	set->fdin = pip[0];
	set->pip_exists = 1;
	close(pip[1]);
}

void    ft_outfiles_before_nofile(t_settings *set, t_cmd *cmd)
{
	if(set->fdin == -1)
		{
			ft_error_msg(cmd->simpleCmds[set->i]->infile[set->j]);
			cmd->simpleCmds[set->i]->exit_code = 1;
		}
	if(set->fdin == -2)
		{
			ft_error_msg3(cmd->simpleCmds[set->i]->infile[set->j]);
			cmd->simpleCmds[set->i]->exit_code = 1;
		}
	set->j = 0;
	while(set->j < cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
	{
        ft_open_outfiles(set, cmd);
		set->j++;
		if(set->fdout != -1)
			{
				close(set->fdout);
				break;
			}
	}	
}

int ft_set_fdin_for_first_s_cmd_w_infile(t_settings *set, t_cmd *cmd, int flag_random_heredoc)
{
	if(set->j != 0 && set->fdin)//TODO proteger des pbs a l ouverture
	{
		if(flag_random_heredoc == 1)
			{
				flag_random_heredoc = 0;
				close(set->fdin);
			}
		else
			close(set->fdin);
	}
	if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 1)
	{
		flag_random_heredoc = 1;
		//set->fdin = 42;
		set->fdin = open(".heredoc", O_RDONLY);
	}
	if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 42)
		set->fdin = open(".heredoc", O_RDONLY); //open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);
	else
	{
		if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == -1)//-1 ->infile normal, 42 infile heredoc LAST , 1->random infile heredoc
			set->fdin = open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
		if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 2)
			set->fdin = -2;	
	}
	return(flag_random_heredoc);
}

void	ft_first_simpleCmd_w_infile(t_settings *set, t_cmd *cmd)
{
	int flag_random_heredoc;

	flag_random_heredoc = 0;
	while(set->j < cmd->simpleCmds[set->i]->nb_of_infile)
	{
		flag_random_heredoc = ft_set_fdin_for_first_s_cmd_w_infile(set, cmd, flag_random_heredoc);
		if(set->fdin == -1 || set->fdin ==-2) //ft_check open error quand on refactorisera plus tard
		{	
            cmd->simpleCmds[set->i]->nofile = 1;
	        if (cmd->simpleCmds[set->i]->outfile != NULL && cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile != 0)
                ft_outfiles_before_nofile(set, cmd);
			else{
				if(set->fdin == -1)
					{
						ft_error_msg(cmd->simpleCmds[set->i]->infile[set->j]);
						cmd->simpleCmds[set->i]->exit_code = 1;
					}
				if(set->fdin == -2)
					{
						ft_error_msg3(cmd->simpleCmds[set->i]->infile[set->j]);
						cmd->simpleCmds[set->i]->exit_code = 1;
					}
			}
			if(cmd->nb_of_simpleCmds > 1)
                ft_create_pipe(set);
			(set->i)++;
			break;	
		}
		(set->j)++;
	}
	set->j = 0;
}
