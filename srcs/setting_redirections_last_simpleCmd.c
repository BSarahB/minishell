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
	size_t 	i;
	size_t 	j;
	int		fdin;
	int		fdout;
	int 	saveout;

	i = set->i;
	j = set->j;
	fdin = set->fdin;
	fdout = set->fdout;
	saveout = set->saveout;
	j = 0;
					if (cmd->simpleCmds[i]->outfile != NULL && (cmd->simpleCmds[i]->nofile == 0))
						{
							while(j < cmd->simpleCmds[i]->nb_of_outfile)
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
						}
					else
						fdout = dup(saveout);
					j = 0;
					if (cmd->simpleCmds[i]->infile != NULL && i != 0)
						{
							while(j < cmd->simpleCmds[i]->nb_of_infile)
							{

								if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
									close(fdin);
								fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
								if(fdin == -1) //ft_check open error quand on refactorisera plus tard
								{
									ft_error_msg(cmd->simpleCmds[i]->infile[j]);
									if(cmd->simpleCmds[i]->outfile != NULL && cmd->simpleCmds[i]->nb_of_outfile_before_nofile != 0)
									{
										size_t k = 0;

										while(k < cmd->simpleCmds[i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
										{
											if(k != 0 && fdout)//TODO proteger des pbs a l ouverture
												close(fdout);
											fdout = open(cmd->simpleCmds[i]->outfile[j], O_CREAT | O_RDWR | O_TRUNC, 0644);
											// if(fdout == -1) gerer les erreurs d ouverture ici avec perror
											if(fdout == -1)
												{
													perror("minishell");
													exit(1);
												//fermer les pipes, nettoyer la memoire etc...
												}
											k++;
										}
									}
									i++;
									j = 0;
									break;
								}
								j++;
							}
							//si j != 0 alors on peut dire que un infile doit faire office d origine de lecture a la place du pip[0] donc on devrait rediriger STDIN sur fdin
							if(j != 0)
							{
								dup2(fdin, STDIN_FILENO);
								close(fdin);
							}
							j = 0;
						}
			set->i = i;
			set->j = j;
			set->fdin = fdin;
			set->fdout = fdout;
			set->saveout = saveout;
}
