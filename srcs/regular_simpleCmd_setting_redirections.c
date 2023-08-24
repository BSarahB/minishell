/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regular_simpleCmd_setting_redirections.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:58 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:36:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void    ft_open_infiles(t_settings *set, t_cmd *cmd)
{
    int flag_random_heredoc;

	flag_random_heredoc = 0;
    while (set->j < cmd->simpleCmds[set->i]->nb_of_infile)
    {

        if(set->j != 0 && set->fdin)//TODO proteger des pbs a l ouverture
			{
				if(flag_random_heredoc == 1)
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
				if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == -1)//-1 ->infile normal, 42 infile heredoc LAST , 1->random infile heredoc
					set->fdin = open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
			}




      //  if (set->j != 0 && set->fdin) // TODO proteger des pbs a l ouverture
      //      close(set->fdin);
        set->fdin = open(cmd->simpleCmds[set->i]->infile[set->j], O_RDONLY); // TODO cmt rendre compte du nom du inputfile si on ne le connait pas
        if (set->fdin == -1)                                       // ft_check open error quand on refactorisera plus tard
        {
            ft_error_msg(cmd->simpleCmds[set->i]->infile[set->j]);
            break;
        }
        (set->j)++;
    }
}

void ft_regular_simpleCmd(t_settings *set, t_cmd *cmd)
{
    int pip[2];
    if (pipe(pip) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    set->fdout = pip[1];
    set->fdin = pip[0]; //+++ ainsi au prochain tour de boucle, fdin (et donc la future entree standart) sera DEJA parametree pour preparer le fdin du processus suivant qui executera la commande du pipe suivant et sera verra donc deja redirigee son entree standard sur la sortie du tube soit pip[0] pour lire a partir de pip[0] ce qui aura ete jete dans pip[1](cmd actuelle)
    if ((cmd->simpleCmds[set->i]->nb_of_tokens_in_simpleCmd == 1) && (cmd->simpleCmds[set->i]->infile == NULL) && (ft_strcmp(cmd->simpleCmds[set->i]->cmd_and_args[0], "cat") == 0)) //&& (cmd->simpleCmds[set->i]->infile == NULL)

        close(pip[0]);
    if (cmd->simpleCmds[set->i]->outfile != NULL)    // TODO DETERMINER LA PRIORITE : SI INFILE APPARAIT AVANT OUTFILE IL FAUDRA PAS CREER OUTFILE, SI OUTFILE APPARAIT AVANT INFILE IL FAUDRA CREER OUTFILE MEME SIL N EST PAS REMPLI
    {
        while (set->j < cmd->simpleCmds[set->i]->nb_of_outfile)
        {
           ft_open_outfiles(set, cmd);
            (set->j)++;
        }
        close(pip[1]);
    }
    set->j = 0;
    if (cmd->simpleCmds[set->i]->infile != NULL && set->i != 0)
    {
        ft_open_infiles(set, cmd); // close(pip[0]);
        set->j = 0;
    }
}
