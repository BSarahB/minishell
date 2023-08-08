/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_first_simpleCmd_w_in          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:16:55 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/08 13:20:07 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_first_simpleCmd_w_infile(t_settings *set, t_cmd *cmd)
{
	size_t 	i;
	size_t 	j;
	int		fdin;
	int		fdout;

	i = set->i;
	j = set->j;
	fdin = set->fdin;
	fdout = set->fdout;
	while(j < cmd->simpleCmds[i]->nb_of_infile)
	{
		if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
			close(fdin);
		fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
		if(fdin == -1) //ft_check open error quand on refactorisera plus tard
		{	
			//ft_error_msg(cmd->simpleCmds[i]->infile[j]);
			cmd->simpleCmds[i]->nofile = 1;
			j = 0;
			if (cmd->simpleCmds[i]->outfile != NULL && cmd->simpleCmds[i]->nb_of_outfile_before_nofile != 0)
			{
				while(j < cmd->simpleCmds[i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
				{
					if(j != 0 && fdout)//TODO proteger des pbs a l ouverture
						close(fdout);
					fdout = open(cmd->simpleCmds[i]->outfile[j], O_CREAT | O_RDWR | O_TRUNC, 0644);
					// if(fdout == -1) gerer les erreurs d ouverture ici avec perror
					if(fdout == -1)
						{
							perror("minishell");
							exit(1);
						//fermer les pipes, nettoyer la memoire etc...
						}
					j++;
				}
				//on reteste l ouverture du fichier si jamais il a ete cree en tant que outfile juste avant....cat >outfile2001 <outfile2001
				j = 0;
				while(j < cmd->simpleCmds[i]->nb_of_infile)
				{
					if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
						close(fdin);
					fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
					if(fdin == -1)
						{
							ft_error_msg(cmd->simpleCmds[i]->infile[j]);
							cmd->simpleCmds[i]->nofile = 1;
						}
					j++;
				}

			}
			else
			{
				ft_error_msg(cmd->simpleCmds[i]->infile[j]);

			}

			if(cmd->nb_of_simpleCmds > 1)
			{
				//on cree le pipe
				int pip[2];
				if (pipe(pip) == -1)
				{
          			perror("pipe");
          			exit(EXIT_FAILURE);
          		}
				//fdout = pip[1];
				fdin = pip[0];
				//dup2(fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
				//close(fdin);
				close(pip[1]);

			}

			i++;
			break;	
		}
		j++;
	}
	j = 0;
	set->i = i;
	set->j = j;
	set->fdin = fdin;
	set->fdout = fdout;
}