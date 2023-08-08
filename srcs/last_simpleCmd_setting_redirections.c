/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_last_simpleCmd.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:44:31 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/08 13:44:34 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd->simpleCmds[set->i]->infile != NULL && set->i != 0)
		{
			while(set->j < cmd->simpleCmds[set->i]->nb_of_infile)
			{

				if(set->j != 0 && set->fdin)//TODO proteger des pbs a l ouverture
					close(set->fdin);
				set->fdin = open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
				if(set->fdin == -1) //ft_check open error quand on refactorisera plus tard
				{
					ft_error_msg(cmd->simpleCmds[set->i]->infile[set->j]);
					if(cmd->simpleCmds[set->i]->outfile != NULL && cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile != 0)
					{
						size_t k = 0;
						while(k < cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
						{
							if(k != 0 && set->fdout)//TODO proteger des pbs a l ouverture
								close(set->fdout);
							set->fdout = open(cmd->simpleCmds[set->i]->outfile[set->j], O_CREAT | O_RDWR | O_TRUNC, 0644);
							// if(fdout == -1) gerer les erreurs d ouverture ici avec perror
							if(set->fdout == -1)
								{
									perror("minishell");
									exit(1);
									//fermer les pipes, nettoyer la memoire etc...
								}
							k++;
						}
					}
					(set->i)++;
					set->j = 0;
					break;
				}
				set->j++;
			}
			//si j != 0 alors on peut dire que un infile doit faire office d origine de lecture a la place du pip[0] donc on devrait rediriger STDIN sur fdin
			if(set->j != 0)
			{
				dup2(set->fdin, STDIN_FILENO);
				close(set->fdin);
			}
			set->j = 0;
		}
}