/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 16:31:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/12 16:32:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redirect_output(t_settings *set)
{
	dup2(set->fdout, STDOUT_FILENO);
	close(set->fdout);
}

void	ft_redirect_input(t_settings *set)
{
	dup2(set->fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
	close(set->fdin);
}

void	ft_set_fdin_for_first_simpleCmd(t_settings *set, t_cmd *cmd)
{
	if (cmd->simpleCmds[set->i]->infile != NULL)
		ft_first_simpleCmd_w_infile(set, cmd);
	else
		set->fdin = dup(set->savein);
}

void	ft_child_process(t_settings *set, t_cmd *cmd, char *envp[], int ret, t_data *data, t_list *lst_token, char *line)
{
	int	exec_return;

	if(ret == 0)
	{			
		exec_return = ft_execute_cmd(cmd, set->i, envp, set);
		if (exec_return == -1 && (errno == 2 || errno == 13))
		{	
		//	ft_check_close_error((*ptr).fd2);
			close(set->savein);
			close(set->saveout);
		}
		ft_free_struct_t_settings(&set);
		ft_free(cmd, lst_token, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		//il faudra imperativement SORTIR de la pour ne pas que le code du fork s execute derriere dans l enfant avec la boucle while
		exit(1);//ou (0?) voir comment bien sortir
	}
	if (ret == -1)
	{
	//ft_free_t_struct(&ptr);
		perror (" pb fork ");
		exit(EXIT_FAILURE);
	}
}

int	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, t_list *lst_token, char *line)
{
	int 		ret;
	int 		exit_status;
	t_settings	*set;
	
	set = ft_struct_init_settings(&set); //todo proteger si set ==NULL
	if(cmd->simpleCmds[set->i] == NULL)
		return 0;//TODO rectifier le bon exit status
	ft_save_in_and_out(set);//sauvegardes des vrais in et out
	ft_set_fdin_for_first_simpleCmd(set, cmd);//on parametre infile + on parametre fdin
	while(set->i < cmd->nb_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		ft_redirect_input(set);		//on redirige l input	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
		if (set->i == (cmd->nb_of_simpleCmds) - 1)  		//on parametre fdout -> facto avec ft_set_fdout //###SI lastSIMPlCOMMANDE###
			ft_last_simpleCmd(set, cmd);
		else 		//###SI SIMPLE COMMANDE REGULAR)### cat|ls
			ft_regular_simpleCmd(set, cmd);
		if(cmd->simpleCmds[set->i]->nofile != 1)//if(set->nofile != 1)
		{
			ft_redirect_output(set);
				//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
			ret = fork();//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
			ft_child_process(set, cmd, envp, ret, data, lst_token, line);
		}
		(set->i)++;
		//set->nofile = 0;
	}
	exit_status = ft_exit_status(ret, set);
	ft_restore_original_in_and_out(set);	//restauration des sauvegardes des vrais in et out 
	ft_free_struct_t_settings(&set);
	return(exit_status);
	return 0;
}
//int 	wstatus;
//waitpid(ret, &wstatus, 0);