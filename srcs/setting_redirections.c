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

extern int g_signal;


void handler_sigquit(int num)
{
	(void)num;
	ft_putstr_fd("Quit (core dumped)\n", 2);
}


int ft_is_flag_n(char *str)
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	if (str[i] == '\0')
	{
		return (flag);
	}
	if (str[i] != '-')
		return (flag); // 0
	if (str[i] == '-')
		i++; // 0
	while (str[i])
	{
		if (str[i] != 'n')
		{
			// printf("flag n : %d\n", flag);

			return (flag);
		}
		i++;
	}
	flag = 1;
	// printf("flag n : %d\n", flag);
	return (1);
}

void ft_echo(t_simpleCmd *simpleCmd)
{
	int j;
	int flag_n;

	flag_n = 0;
	j = 1;
	// on va passer cmd_and_args et si on a le flag n dans un cmd_and_arg[i] on passe notre chemin jusqua tomber sur un argument sans n , comme les suivants meme avec n ne seront plus des flag n alors il faudra les imprimer a l ecran
	if (simpleCmd->echo_no_option == 1)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return;
	}
	while (simpleCmd->cmd_and_args[j] && ft_is_flag_n(simpleCmd->cmd_and_args[j]) == 1)
	{
		flag_n = 1;
		j++;
	}
	while (simpleCmd->cmd_and_args[j])
	{
		ft_putstr_fd(simpleCmd->cmd_and_args[j], STDOUT_FILENO);
		if (flag_n == 0 && simpleCmd->cmd_and_args[j + 1] == NULL)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		else if (simpleCmd->cmd_and_args[j + 1])
		{
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		j++;
	}
}

void ft_redirect_output(t_settings *set)
{
	dup2(set->fdout, STDOUT_FILENO);
	close(set->fdout);
}

void ft_redirect_input(t_settings *set, t_cmd *cmd)
{
	(void)cmd;
	dup2(set->fdin, STDIN_FILENO); // durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
	close(set->fdin);
}

void ft_set_fdin_for_first_simpleCmd(t_settings *set, t_cmd *cmd)
{
	if (cmd->simpleCmds[set->i]->infile != NULL)
		ft_first_simpleCmd_w_infile(set, cmd);
	else
		set->fdin = dup(set->savein);
}


void ft_free_and_exit_child(t_settings *set, t_cmd *cmd, char **envp_t, t_data *data, char *line, t_data_env *data_env)
{
	if(set->pip_exists == 1) // a | b
	{
		close(set->pip[0]);
		close(set->pip[1]); //close ne fait pas d erreur d apres les tests ca n a pas l air de deranger l execuion du prograame du coup on se permet ici de tt fermer
	//	close(set->fdout);
	//	close(set->fdin);
	}
	
	close(set->savein);
	close(set->saveout);

	ft_free_struct_t_settings(&set);
	ft_free_in_child(cmd, data, line);
	ft_free_struct_t_cmd_only(&cmd);
	if (envp_t != NULL)
		ft_free_tab(&envp_t);
	if (data_env != NULL)
		ft_free_struct_t_data_env(&data_env);
}


void ft_child_process(t_settings *set, t_cmd *cmd, char **envp_t, t_data *data, char *line, t_data_env *data_env)
{
	int exec_return;
	int exit_code;
	(void)data;
	(void)line;
	t_listenvp *tmp;
	t_listenvp *tmp2;
	t_listenvp *tmp3;

	exec_return = 0;
	exit_code = 0;
	//	int i;
	//	i = 0;
	tmp = NULL;
	tmp2 = NULL;
	tmp3 = NULL;

	(void)tmp3;
	g_signal = IN_S_CMD;
	signal(SIGQUIT, handler_sigquit);
	if (cmd->simpleCmds[set->i]->is_builtin == 0)
		exec_return = ft_execute_cmd(cmd, (int)set->i, envp_t, set);
	//si exec_return == 0 comme pour une commande vide

	if (exec_return == -1 && (errno == 2 || errno == 13)) //cmd not found
	{		//set->errnum = 127;
		if ((set->i < cmd->nb_of_simpleCmds) && (set->i != (cmd->nb_of_simpleCmds) - 1))
			close(set->pip[0]);
	
	close(set->savein);
	close(set->saveout);
	ft_free_struct_t_settings(&set);
	ft_free_in_child(cmd, data, line);
	ft_free_struct_t_cmd_only(&cmd);
	if (envp_t != NULL)
		ft_free_tab(&envp_t);
	if (data_env != NULL)
		ft_free_struct_t_data_env(&data_env);
	//	ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
	exit(127);
	}

	//BUILTIN UNSET 
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 4)
	{
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
		exit(0);
	}

	// BUITLIN EXIT
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 6)
	{

		if (set->i == (cmd->nb_of_simpleCmds) - 1)
			{
				printf("exit__code_DOMINANT \n"); // donc stocker l exit code dans variable globale//	printf("exit_code = %d \n", simpleCmd->exit_code)
		 // ou (0?) voir comment bien sortir mettre ca apres le pb du fork
			}
		exit_code = cmd->simpleCmds[set->i]->exit_code;
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
		exit(exit_code);
		
	}
	// BUITLIN ECHO
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 0) // modifier pour ==0 pour faire le builtin echo
	{
		ft_echo(cmd->simpleCmds[set->i]);
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
		exit(0);
	}
	// BUILTIN PWD
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 2) // modifier pour ==0 pour faire le builtin echo
	{
		if (cmd->simpleCmds[set->i]->pwd != NULL)
		{
			ft_putstr_fd(cmd->simpleCmds[set->i]->pwd, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
		exit(0);
	}
	// BUILTIN ENV
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 5)
	{
		exit_code = cmd->simpleCmds[set->i]->exit_code;
		if(exit_code != 127)
		{
			tmp2 = data_env->lst_envp;
			while (tmp2)
			{
				if (ft_strncmp(tmp2->key_value, "?=", 2) != 0)
				{
					ft_putstr_fd(tmp2->key_value, STDOUT_FILENO);
					ft_putstr_fd("\n", STDOUT_FILENO);
				}
			tmp2 = tmp2->next;
			}
		}
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);	
		exit(exit_code);
	}
	// BUILTIN CD
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->builtin == 1) // modifier pour ==0 pour faire le builtin echo
	{
		if (cmd->simpleCmds[set->i]->oldpwd != NULL)
		{
			ft_putstr_fd(cmd->simpleCmds[set->i]->oldpwd, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		exit_code = cmd->simpleCmds[set->i]->exit_code;
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);	
		exit(exit_code); //mettre 1 si le cd n est pas bon
	}
//BUILTIN EXPORT AVEC OPTION
	if (cmd->simpleCmds[set->i]->is_builtin == 1 && cmd->simpleCmds[set->i]->export_no_option == 0  && cmd->simpleCmds[set->i]->builtin == 3) // modifier pour ==0 pour faire le builtin echo //TODO VIRER SI NON NECESSAIRE
	{
		exit_code = cmd->simpleCmds[set->i]->exit_code;
		ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
		exit(exit_code);
	}
	// BUITIN EXPORT SANS OPTION ET SOLO
	data_env->lst_envp_d = ft_add_double_quote_to_envp_d(data_env->lst_envp_d);
	tmp = data_env->lst_envp_d;

	while (tmp && cmd->simpleCmds[set->i]->builtin != 5)
	{
		if (ft_strncmp(tmp->key_value, "?=", 2) != 0)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key_value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	ft_free_and_exit_child(set, cmd, envp_t, data, line, data_env);
	exit(0);
}

int	ft_setting_redirections_and_pipes(t_cmd *cmd, char **envp_t, t_data *data, char *line, t_data_env *data_env)
{
	int 		exit_status;
	//static int 		exit_code;
	t_settings	*set;

	//exit_code = -6;
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
					g_signal = IN_S_CMD;
					signal(SIGQUIT, handler_sigquit);
					set->ret = fork();//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
					if (set->ret == -1)
					{
						//TODO FREE
						perror (" pb fork ");
						exit(EXIT_FAILURE);
					}
					if(set->ret == 0)
					{
						
						ft_child_process(set, cmd, envp_t, data, line, data_env);

					}

				//	;
				//	ft_aff_list_envp_sur_char_content(data_env->lst_envp);
			}
		}
		(set->i)++; //a | b i ==1


	}
//condition ?
	//close(set->fdout);
	//close(set->fdin);
	exit_status = ft_exit_status(set->ret, set);
	if(set->i >= 1 && cmd->simpleCmds[set->i -1]->nofile == 1)
		exit_status =  cmd->simpleCmds[set->i -1]->exit_code;
	if(exit_status == 141)
		exit_status = 0;
	printf("exit_status : %d\n", exit_status);
	data_env->lst_envp = ft_get_exit_status(&data_env->lst_envp, "?=", exit_status);
	ft_restore_original_in_and_out(set);	//restauration des sauvegardes des vrais in et out
	ft_free_struct_t_settings(&set);
	return(exit_status);
}
//int 	wstatus;
//waitpid(ret, &wstatus, 0);
// a | b
//   a->SET fdin =1 REDIRIGE FDIN STDIN -> fdin + set fdout = pip[1] REDIRIGE FDOUT ***********[SET FDIN = pip[0]****** pip1 | pip 0  b *********** REDIRIGER FDIN SET FDOUT REDIRIGE STODOUT
