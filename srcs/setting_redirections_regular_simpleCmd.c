/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_regular_simpleCmd.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:48:21 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/08 13:48:24 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_regular_simpleCmd(t_settings *set, t_cmd *cmd)
{
    size_t i;
    size_t j;
    int fdin;
    int fdout;
    int pip[2];

    i = set->i;
    j = set->j;
    fdin = set->fdin;
    fdout = set->fdout;

    if (pipe(pip) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    fdout = pip[1];
    fdin = pip[0]; //+++ ainsi au prochain tour de boucle, fdin (et donc la future entree standart) sera DEJA parametree pour preparer le fdin du processus suivant qui executera la commande du pipe suivant et sera verra donc deja redirigee son entree standard sur la sortie du tube soit pip[0] pour lire a partir de pip[0] ce qui aura ete jete dans pip[1](cmd actuelle)
    if (cmd->simpleCmds[i]->nb_of_tokens_in_simpleCmd == 1 && ft_strcmp(cmd->simpleCmds[i]->cmd_and_args[0], "cat") == 0)
        close(pip[0]);
    // TODO DETERMINER LA PRIORITE : SI INFILE APPARAIT AVANT OUTFILE IL FAUDRA PAS CREER OUTFILE, SI OUTFILE APPARAIT AVANT INFILE IL FAUDRA CREER OUTFILE MEME SIL N EST PAS REMPLI
    if (cmd->simpleCmds[i]->outfile != NULL)
    {
        while (j < cmd->simpleCmds[i]->nb_of_outfile)
        {
            if (j != 0 && fdout) // TODO proteger des pbs a l ouverture
                close(fdout);
            fdout = open(cmd->simpleCmds[i]->outfile[j], O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (fdout == -1)
            {
                perror("minishell");
                exit(1);
                // fermer les pipes, nettoyer la memoire etc...
            }

            j++;
        }
        close(pip[1]);
    }
    j = 0;
    if (cmd->simpleCmds[i]->infile != NULL && i != 0)
    {
        while (j < cmd->simpleCmds[i]->nb_of_infile)
        {

            if (j != 0 && fdin) // TODO proteger des pbs a l ouverture
                close(fdin);
            fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY); // TODO cmt rendre compte du nom du inputfile si on ne le connait pas
            if (fdin == -1)                                       // ft_check open error quand on refactorisera plus tard
            {
                ft_error_msg(cmd->simpleCmds[i]->infile[j]);
                // i++;
                break;
            }
            j++;
        }
        // close(pip[0]);

        j = 0;
    }
    set->i = i;
    set->j = j;
    set->fdin = fdin;
    set->fdout = fdout;
}
