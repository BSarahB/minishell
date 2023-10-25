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

int     ft_set_fdin_for_regular_simpleCmd(t_settings *set, t_cmd *cmd, int flag_random_heredoc)
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
        if(cmd->simpleCmds[set->i]->heredoc_track_index[set->j] == 2)
            set->fdin = -2;
	}
    return(flag_random_heredoc);
}

void    ft_open_infiles(t_settings *set, t_cmd *cmd)
{
    int flag_random_heredoc;

	flag_random_heredoc = 0;
    while (set->j < cmd->simpleCmds[set->i]->nb_of_infile)
    {
        flag_random_heredoc = ft_set_fdin_for_regular_simpleCmd(set, cmd, flag_random_heredoc);
        if (set->fdin == -1 || set->fdin == -2)                                       // ft_check open error quand on refactorisera plus tard
        {
            cmd->simpleCmds[set->i]->nofile = 1;
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
            if(cmd->simpleCmds[set->i]->outfile != NULL && cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile != 0)
			{
				size_t k = 0;
				while(k < cmd->simpleCmds[set->i]->nb_of_outfile_before_nofile)//while(j < cmd->simpleCmds[i]->nb_of_outfile)
				{
					ft_open_outfiles_in_last_but_not_first_simpleCmd(set, cmd, k);
					k++;
				}	                     
			}
            break;
        }
        (set->j)++;
    }
    if(cmd->simpleCmds[set->i]->nofile != 1)
        ft_redirect_input(set, cmd);
}




void ft_create_pipe2(t_settings *set)
{
    
    if (pipe(set->pip) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    set->fdout = set->pip[1];
    set->fdin = set->pip[0];
	set->pip_exists = 1;

}
void ft_regular_simpleCmd(t_settings *set, t_cmd *cmd)
{
    set->j = 0;
    if (cmd->simpleCmds[set->i]->infile != NULL && set->i != 0)
    {
        ft_open_infiles(set, cmd); // close(pip[0]);
        set->j = 0;
    }

    ft_create_pipe2(set);
     //+++ ainsi au prochain tour de boucle, fdin (et donc la future entree standart) sera DEJA parametree pour preparer le fdin du processus suivant qui executera la commande du pipe suivant et sera verra donc deja redirigee son entree standard sur la sortie du tube soit pip[0] pour lire a partir de pip[0] ce qui aura ete jete dans pip[1](cmd actuelle)
    if ((cmd->simpleCmds[set->i]->nb_of_tokens_in_simpleCmd == 1) && (cmd->simpleCmds[set->i]->infile == NULL) && (ft_strcmp(cmd->simpleCmds[set->i]->cmd_and_args[0], "cat") == 0))//&& (cmd->simpleCmds[set->i]->infile == NULL)
        {
            if ((ft_strcmp(cmd->simpleCmds[set->i + 1]->cmd_and_args[0], "wc") != 0))
                close(set->pip[0]);
            else
                 close(set->pip[1]);
        }
   
    if (cmd->simpleCmds[set->i]->outfile != NULL && (cmd->simpleCmds[set->i]->nofile == 0))
	{
        while (set->j < cmd->simpleCmds[set->i]->nb_of_outfile)
        {
           ft_open_outfiles(set, cmd);
            (set->j)++;
        }
        close(set->pip[1]);
    }
    if (cmd->simpleCmds[set->i]->nofile == 1 || (cmd->simpleCmds[set->i] == NULL || cmd->simpleCmds[set->i]->cmd_and_args == NULL))
    {
        close(set->pip[1]);
        ft_redirect_output(set);
    }
   
    set->j = 0;
}
 /*
    set->j = 0;
    if (cmd->simpleCmds[set->i]->infile != NULL && set->i != 0)
    {
        ft_open_infiles(set, cmd); // close(pip[0]);
        set->j = 0;
    }
    */
