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

void	ft_save_in_and_out(t_settings *set)
{
	set->savein = dup(0);//dup(STDIN_FILENO);
	set->saveout = dup(1);//dup(STDOUT_FILENO);
}

void	ft_set_fdin_for_first_simpleCmd(t_settings *set, t_cmd *cmd)
{
	if (cmd->simpleCmds[set->i]->infile != NULL)
		ft_first_simpleCmd_w_infile(set, cmd);
	//s il n existe pas on prend stdin par defaut
	else
		set->fdin = dup(set->savein);
}

void	ft_child_process(t_settings *set, t_cmd *cmd, char *envp[], int ret, t_data *data)
{
	int	exec_return;

	if(ret == 0) //dans l enfant on execute la commande correspondant a la simplecmd
	{			
	//si on est dans l enfant on va pouvoir lancer l execution de sa simpleCommande
		exec_return = ft_execute_cmd(cmd, set->i, envp);//appel a execve
		if (exec_return == -1 && (errno == 2 || errno == 13))
		{	
		//	ft_check_close_error((*ptr).fd2);
			close(set->savein);
			close(set->saveout);

		}

		ft_free_struct_t_data(&data);
		ft_free_struct_t_settings(&set);
		ft_free_struct_t_cmd(&cmd);

		//il faudra imperativement SORTIR de la pour ne pas que le code du fork s execute derriere dans l enfant avec la boucle while
		exit(1);//ou (0?) voir comment bien sortir
	}
	if (ret == -1)
	{
	//ft_free_t_struct(&ptr);
		perror (" pb fork ");
		exit(EXIT_FAILURE);//j avais mis 1 dans pipex cd la macro
	}
}

 int	ft_exit_status(pid_t last_pid, t_settings *set)
 {
	pid_t	wpid;
	int		wstatus;
	int		exit_status;

	while(1)
	{
		wpid = waitpid(-1, &wstatus, 0);
		//wpid = wait(&wstatus);
		if(wpid < 0)
			break;
		if(wpid == last_pid)
		{
			if(WIFEXITED(wstatus))
				exit_status = WEXITSTATUS(wstatus);
			else
				exit_status = 128 + WTERMSIG(wstatus);
		}
	}
	set->exit_status = exit_status;
	return(exit_status);
 }


void	ft_restore_original_in_and_out(t_settings *set)
{
	dup2(set->savein, 0);
	dup2(set->saveout, 1);
	close(set->savein);
	close(set->saveout);
}

//TODO : appeler ft_checkopenerror au bon endroit, sinon la scinder en 2 fonctions
int	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data)
{
	int 	ret;
	int 	exit_status;
	t_settings	*set;

	set = ft_struct_init_settings(&set);
	if(cmd->simpleCmds[set->i] == NULL)
		return 0;//TODO rectifier le bon exit status
	ft_save_in_and_out(set);//sauvegardes des vrais in et out
	ft_set_fdin_for_first_simpleCmd(set, cmd);//on parametre infile + on parametre fdin
	while(set->i < cmd->nb_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		ft_redirect_input(set);		//on redirige l input	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
		//on parametre fdout		//	ft_set_fdout
//###SI lastSIMPlCOMMANDE###
		if (set->i == (cmd->nb_of_simpleCmds) - 1)
			ft_last_simpleCmd(set, cmd);
		else 		//###SI SIMPLE COMMANDE REGULAR)### cat|ls
			ft_regular_simpleCmd(set, cmd);
		ft_redirect_output(set);	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
		ret = fork();//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
		ft_child_process(set, cmd, envp, ret, data);
		(set->i)++;
	}
	ft_restore_original_in_and_out(set);	//restauration des sauvegardes des vrais in et out 
	exit_status = ft_exit_status(ret,set);
	ft_free_struct_t_settings(&set);
	return(exit_status);
}