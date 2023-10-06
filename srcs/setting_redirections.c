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


void ft_echo(void)
{

	printf("bonjour\n");
	printf("bonjour\n");

}

void	ft_redirect_output(t_settings *set)
{
	dup2(set->fdout, STDOUT_FILENO);
	close(set->fdout);
}

void	ft_redirect_input(t_settings *set, t_cmd *cmd)
{
	(void)cmd;
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

void	ft_child_process(t_settings *set, t_cmd *cmd, char *envp[], t_data *data, char *line, t_data_env *data_env)
{
	int	exec_return;
	(void)data;
	(void)line;
	exec_return = 0;
	int i;
	i = 0;

	if(set->ret == 0)
	{	
		if(cmd->simpleCmds[set->i]->is_builtin == 0)
			exec_return = ft_execute_cmd(cmd, set->i, envp, set);
		if (exec_return == -1 && (errno == 2 || errno == 13))
		{	
			close(set->savein);
			close(set->saveout);
			if((set->i < cmd->nb_of_simpleCmds) && (set->i != (cmd ->nb_of_simpleCmds) - 1))
				close(set->pip[0]);	
			ft_free_struct_t_settings(&set);
			ft_free_in_child(cmd, data, line);
			ft_free_struct_t_cmd_only(&cmd);
			if(envp != NULL)
				ft_free_tab(&envp);
			if(data_env != NULL)
				ft_free_struct_t_data_env(&data_env);	
			exit(1);//ou (0?) voir comment bien sortir mettre ca apres le pb du fork	
		}
		if(cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 2) //modifier pour ==0 pour faire le builtin echo
		{
			ft_echo();
			close(set->savein);
		close(set->saveout);
		ft_free_struct_t_settings(&set);
		ft_free_in_child(cmd, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		if(envp != NULL)
			ft_free_tab(&envp);
		if(data_env != NULL)
			ft_free_struct_t_data_env(&data_env);	
		exit(1);//ou (0?) voir comment bien sortir mettre ca apres le pb du fork
		}
		if(cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->export_no_option == 0) //modifier pour ==0 pour faire le builtin echo
		{
			
		close(set->savein);
		close(set->saveout);
		ft_free_struct_t_settings(&set);
		ft_free_in_child(cmd, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		if(envp != NULL)
			ft_free_tab(&envp);
		if(data_env != NULL)
			ft_free_struct_t_data_env(&data_env);	
		exit(1);//ou (0?) voir comment bien sortir mettre ca apres le pb du fork
		}
		while(envp[i])
		{
			//printf("declare -x %s\n", envp[i]);
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(envp[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			i++;
		}
		close(set->savein);
		close(set->saveout);
		ft_free_struct_t_settings(&set);
		ft_free_in_child(cmd, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		if(envp != NULL)
			ft_free_tab(&envp);
		if(data_env != NULL)
			ft_free_struct_t_data_env(&data_env);	
		exit(1);//ou (0?) voir comment bien sortir mettre ca apres le pb du fork
	}
	if (set->ret == -1)
	{
	//TODO FREE 
		perror (" pb fork ");
		exit(EXIT_FAILURE);
	}
}

int	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, char *line, t_data_env *data_env)
{
	int 		exit_status;
	t_settings	*set;

	//ft_aff_tab_envp(envp);
	set = ft_struct_init_settings(&set); //todo proteger si set ==NULL
	if(cmd->simpleCmds[set->i] == NULL)//mettre cela dans la while car on pourrait tres bien tomber sur la 2 eme simplecmd dans laquelle on aurait la simpleCmd == null
		{
			ft_free_struct_t_settings(&set);
			return 0;
		}//TODO rectifier le bon exit status
	ft_save_in_and_out(set);//sauvegardes des vrais in et out
	ft_set_fdin_for_first_simpleCmd(set, cmd);//on parametre infile + on parametre fdin
	while(set->i < cmd->nb_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		ft_redirect_input(set, cmd);		//on redirige l input	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
		if (set->i == (cmd->nb_of_simpleCmds) - 1)  		//on parametre fdout -> facto avec ft_set_fdout //###SI lastSIMPlCOMMANDE###
			ft_last_simpleCmd(set, cmd);
		else 		//###SI SIMPLE COMMANDE REGULAR)### cat|ls
			ft_regular_simpleCmd(set, cmd);
		if(cmd->simpleCmds[set->i]->nofile != 1)//if(set->nofile != 1)
		{
			if(cmd->simpleCmds[set->i] != NULL && cmd->simpleCmds[set->i]->cmd_and_args != NULL)
			{
				
					ft_redirect_output(set);
				//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
					set->ret = fork();//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
					ft_child_process(set, cmd, envp, data, line, data_env);
				
			}
		}
		(set->i)++;
	}
	exit_status = ft_exit_status(set->ret, set);
	ft_restore_original_in_and_out(set);	//restauration des sauvegardes des vrais in et out 
	ft_free_struct_t_settings(&set);
	return(exit_status);
}
//int 	wstatus;
//waitpid(ret, &wstatus, 0);