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
int	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[])
{
	int 	ret;
	int 	exec_return;
	t_settings	*set;

	set = ft_struct_init_settings(&set);
//sauvegardes des vrais in et out
	set->savein = dup(0);//dup(STDIN_FILENO);
	set->saveout = dup(1);//dup(STDOUT_FILENO);
//on parametre infile + on parametre fdin
	//sil existe ce sera l entree de toute la cmd
	//proteger si il n y a pas de token mais que des espaces
	if(cmd->simpleCmds[set->i] == NULL)
		return 0;//TODO rectifier le bon exit status
	if (cmd->simpleCmds[set->i]->infile != NULL)
		ft_first_simpleCmd_w_infile(set, cmd);
	//s il n existe pas on prend stdin par defaut
	else
		set->fdin = dup(set->savein);
	while(set->i < cmd->nb_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		//on redirige l input
		dup2(set->fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
		close(set->fdin);
		//on parametre fdout
//###SI lastSIMPlCOMMANDE###
		if (set->i == (cmd->nb_of_simpleCmds) - 1)
			ft_last_simpleCmd(set, cmd);
	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
	//###SI SIMPLE COMMANDE REGULAR)### cat|ls
		else {		
			ft_regular_simpleCmd(set, cmd);
		}
		dup2(set->fdout, STDOUT_FILENO);
		close(set->fdout);
	//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
		ret = fork();
		if (ret == 0) //dans l enfant on execute la commande correspondant a la simplecmd
			{			
				//si on est dans l enfant on va pouvoir lancer l execution de sa simpleCommande
				exec_return = ft_execute_cmd(cmd, set->i, envp);//appel a execve
				if (exec_return == -1 && (errno == 2 || errno == 13))
				{	
				//	ft_check_close_error((*ptr).fd2);
					close(set->savein);
					close(set->saveout);
				}
				//il faudra imperativement SORTIR de la pour ne pas que le code du fork s execute derriere dans l enfant avec la boucle while
				exit(1);//ou (0?) voir comment bien sortir
			}
			if (ret == -1)
			{
				//ft_free_t_struct(&ptr);
				perror (" pb fork ");
				exit(EXIT_FAILURE);//j avais mis 1 dans pipex cd la macro
			}
			(set->i)++;
	}
	ft_restore_original_in_and_out(set);
	//restauration des sauvegardes des vrais in et out :
	
	return(ft_exit_status(ret,set));
}





























	//wait(&wstatus); 
	// gestion de & background ->on attendra pas la derniere commande sil n y est pas.
	//waitpid(ret, &wstatus, 0);//avant d imprimer dans la console, on demande au pere d attendre que les enfants aient fini.
 	//waitpid(0, &wstatus, 0);

/*L'appel système waitpid() suspend l'exécution du processus appelant jusqu'à ce que le fils spécifié par son pid ait changé d'état. Par défaut, waitpid() n'attend que les fils terminés, mais ce comportement est modifiable avec l'argument options comme décrit plus loin.

La valeur de pid peut être l'une des suivantes :

< -1
attendre la fin de n'importe lequel des processus fils dont le GID du processus est égal à la valeur absolue de pid.
-1
attendre n'importe lequel des processus fils.
0
attendre n'importe lequel des processus fils dont le GID du processus est égal à celui du processus appelant.
> 0
attendre n'importe lequel des processus fils dont le PID est égal à pid.

*/
/*  The waitpid() system call suspends execution of the calling thread  un‐
	   til  a  child specified by pid argument has changed state.  By default,
	   waitpid() waits only for terminated children, but this behavior is mod‐
	   ifiable via the options argument, as described below.
-1 : meaning wait for any child process.
*/


//http://manpagesfr.free.fr/man/man2/wait.2.html
//TODO : toutes les protections et les returns echo $?
//FAIRE mon document pour le minishell avec toutes les commandes -> triim and clear etc. avec ts les process pour preparer en bon et du form les commandes au parsing
//~ ls -la <nofile |ls -la >outfilenani <nofile |wc -l
//minishell: nofile: no such file or directory:
//minishell: nofile: no such file or directory:
//0
//TODO : BUGG
 //~ ls -la | grep c  <nofile | wc -l  -->ici specificite et diff  A CAUSE DE GREP QUI VIENT chercher dans le pip[0]
//minishell: nofile: no such file or directory:
//24
//parfois on a 0 ->bon comportement si cmd2 est wc -l
//TODO : wc -l <infile >outfile | wc -l  >outfile | wc -l >outfile ->bash me donne 2 , mon programme me donne 0. cmportement indefini




//TODO bug creation des outfiles before nofile:
// ls | ls >o1 >o2 <nofile o2 n est pas cree....