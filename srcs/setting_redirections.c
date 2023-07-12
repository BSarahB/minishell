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

 int	ft_exit_status(pid_t last_pid)
 {
	pid_t	wpid;
	int		wstatus;
	int		exit_status;

	while(1)
	{
		wpid = wait(&wstatus);
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
	return(exit_status);
 }


//TODO : appeler ft_checkopenerror au bon endroit, sinon la scinder en 2 fonctions
void	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[])
{
	int		savein;
	int		saveout;
	int		fdin;
	int		fdout;
	size_t	i;
	size_t	j;
	int 	ret;
	int 	wstatus;
	int 	exec_return;


	(void)wstatus;
	i = 0;
	j = 0;
//sauvegardes des vrais in et out
	savein = dup(0);//dup(STDIN_FILENO);
	saveout = dup(1);//dup(STDOUT_FILENO);
//on parametre infile + on parametre fdin
	//sil existe ce sera l entree de toute la cmd
	//proteger si il n y a pas de token mais que des espaces
	if(cmd->simpleCmds[i] == NULL)
		return;
	if (cmd->simpleCmds[i]->infile != NULL)
		{
			while(j < cmd->simpleCmds[i]->nb_of_infile)
			{
				if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
					close(fdin);
				fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
				if(fdin == -1) //ft_check open error quand on refactorisera plus tard
				{	
					ft_error_msg(cmd->simpleCmds[i]->infile[j]);
					j = 0;
					if (cmd->simpleCmds[i]->outfile != NULL)
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
						j = 0;
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
		}
	
	//s il n existe pas on prend stdin par defaut
	else
		fdin = dup(savein);


	if (i<cmd->nb_of_simpleCmds)
	{
		//on redirige l input
		dup2(fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
		close(fdin);
	}
	while(i < cmd->nb_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		//on redirige l input
	//	dup2(fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
	//	close(fdin);

		//on parametre fdout
//###SI lastSIMPlCOMMANDE###
		if (i == (cmd->nb_of_simpleCmds) - 1)
		{
			if (cmd->simpleCmds[i]->outfile != NULL)
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
			if (cmd->simpleCmds[i]->infile != NULL)
				{
					while(j < cmd->simpleCmds[i]->nb_of_infile)
					{

						if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
							close(fdin);
						fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
						if(fdin == -1) //ft_check open error quand on refactorisera plus tard
						{	
							ft_error_msg(cmd->simpleCmds[i]->infile[j]);
							i++;//cf
							j = 0;
							break;	
						}
						j++;
					}
					//si j != 0 alors on peut dire que un infile doit faire office d origine de lecture a la place du pip[0] donc on devrait rediriger STDIN sur fdin
					if (j != 0)
					{
						dup2(fdin, STDIN_FILENO);
						close(fdin);
					}
					j = 0;
				}
		}
	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
	//###SI SIMPLE COMMANDE REGULAR)###
		else {
			//on cree le pipe
			int pip[2];


			if (pipe(pip) == -1)
			{
               perror("pipe");
               exit(EXIT_FAILURE);
           	}
			fdout = pip[1];
			fdin = pip[0];//+++ ainsi au prochain tour de boucle, fdin (et donc la future entree standart) sera DEJA parametree pour preparer le fdin du processus suivant qui executera la commande du pipe suivant et sera verra donc deja redirigee son entree standard sur la sortie du tube soit pip[0] pour lire a partir de pip[0] ce qui aura ete jete dans pip[1](cmd actuelle)
			
//TODO DETERMINER LA PRIORITE : SI INFILE APPARAIT AVANT OUTFILE IL FAUDRA PAS CREER OUTFILE, SI OUTFILE APPARAIT AVANT INFILE IL FAUDRA CREER OUTFILE MEME SIL N EST PAS REMPLI
			if (cmd->simpleCmds[i]->outfile != NULL)
				{
					while(j < cmd->simpleCmds[i]->nb_of_outfile)
					{
						if(j != 0 && fdout)//TODO proteger des pbs a l ouverture
							close(fdout);
						fdout = open(cmd->simpleCmds[i]->outfile[j], O_CREAT | O_RDWR | O_TRUNC, 0644);
						if(fdout == -1)
							{
								perror("minishell");
								exit(1);
								//fermer les pipes, nettoyer la memoire etc...
							}
						
						j++;
					}
					close(pip[1]);
				}
			j = 0;
			if  (cmd->simpleCmds[i]->infile != NULL && i != 0)
				{
					while(j < cmd->simpleCmds[i]->nb_of_infile)
					{

						if(j != 0 && fdin)//TODO proteger des pbs a l ouverture
							close(fdin);
						fdin = open(cmd->simpleCmds[i]->infile[j], O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
						if(fdin == -1) //ft_check open error quand on refactorisera plus tard
						{	
							ft_error_msg(cmd->simpleCmds[i]->infile[j]);
							//i++;
							break;	
						}
						j++;
					}
					//close(pip[0]);
				
					j = 0;
				}
		
		}
			//redirection de l output -> on redirige l output avant de creer les enfants pour qu ils en heritent.
		dup2(fdout, STDOUT_FILENO);
		close(fdout);

	//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
		ret = fork();
	
		if (ret == 0) //dans l enfant on execute la commande correspondant a la simplecmd
			{
				//si on est dans l enfant on va pouvoir lancer l execution de sa simpleCommande
				exec_return = ft_execute_cmd(cmd, i, envp);//appel a execve
				if (exec_return == -1 && (errno == 2 || errno == 13))
				{	
					
				//	ft_check_close_error((*ptr).fd2);
					close(savein);
					close(saveout);
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
			i++;
			dup2(fdin, STDIN_FILENO);//durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
			close(fdin);
			
	}

	//restauration des sauvegardes des vrais in et out :
	dup2(savein, 0);
	dup2(saveout, 1);
	close(savein);
	close(saveout);

	
	// gestion de & background ->on attendra pas la derniere commande sil n y est pas.
	//waitpid(ret, &wstatus, 0);//avant d imprimer dans la console, on demande au pere d attendre que les enfants aient fini.
 	//waitpid(0, &wstatus, 0);
 	ft_exit_status(ret);


	//wait(&wstatus); 
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

}
//http://manpagesfr.free.fr/man/man2/wait.2.html
//TODO : toutes les protections et les returns echo $?
//FAIRE mon document pour le minishell avec toutes les commandes -> triim and clear etc. avec ts les process pour preparer en bon et du form les commandes au parsing