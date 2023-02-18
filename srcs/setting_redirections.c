/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:31:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/12 16:32:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[])
{
	int	tmpin;
	int	tmpout;
	int	fdin;
	int fdout;
	int i;
	int ret;
	int wstatus;


	i = 0;
//sauvegardes des vrais in et out
	tmpin = dup(0);//dup(STDIN_FILENO);
	tmpout = dup(1);//dup(STDOUT_FILENO);
//on parametre infile + on parametre fdin
	//sil existe ce sera l entree de toute la cmd
	if (cmd->inputfile != NULL)
		fdin = open(cmd->inputfile, O_RDONLY);//TODO cmt rendre compte du nom du inputfile si on ne le connait pas
	//s il n existe pas on prend stdin par defaut
	else
		fdin = dup(tmpin);

	while(i < cmd->number_of_simpleCmds) //on parcourt ici chaque processus == chaque simple cmd pour les fr heriter des redirections
	{
		//on redirige l input
		dup2(fdin, STDIN_FILENO); // durant la while, a partir de la 2 eme simplecmd  on va heriter du fdin du pipe COMMUN a la simplecmd precedente on avait parametre : fdin = pip[0]; cest cela qui est le coeur des multipipes
		close(fdin);
		//on parametre fdout
		//###SI LAST SIMPLE COMMANDE###
		if (i == (cmd->number_of_simpleCmds) - 1)
		{
			if (cmd->outfile != NULL)
			fdout = open(cmd->outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
		else
			fdout = dup(tmpout);
	}
	//Redirection des vrais in et out dans le processus parent tjrs en bouclant sur les simpleCmds
	//###SI SIMPLE COMMANDE REGULAR)###
		else {
			//on cree le pipe
			int pip[2];

			pipe(pip);
			fdout = pip[1];
			fdin = pip[0];//+++ ainsi au prochain tour de boucle, fdin (et donc la future entree standart) sera DEJA parametree pour preparer le fdin du processus suivant qui executera la commande du pipe suivant et sera verra donc deja redirigee son entree standard sur la sortie du tube soit pip[0] pour lire a partir de pip[0] ce qui aura ete jete dans pip[1](cmd actuelle)
		}
			//redirection de l output -> on redirige l output avant de creer les enfants pour qu ils en heritent.
		dup2(fdout, STDOUT_FILENO);
		close(fdout);

	//Creation des processus : il faudra creer autant de processus que de commandes donc faire dans le while.
		ret = fork();
		if (ret == 0) //dans l enfant on execute la commande correspondant a la simplecmd
			{
				//si on est dans l enfant on va pouvoir lancer l execution de sa simpleCommande
				ft_execute_cmd(cmd, i, envp);//appel a execve
				//il faudra imperativement SORTIR de la pour ne pas que le code du fork s execute derriere dans l enfant avec la boucle while
				exit(1);//ou (0?) voir comment bien sortir
			}

			i++;
	}
	//restauration des sauvegardes des vrais in et out :
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
	// gestion de & background ->on attendra pas la derniere commande sil n y est pas.
	waitpid(-1, &wstatus, 0);//avant d imprimer dans la console, on demande au pere d attendre que les enfants aient fini.
/*  The waitpid() system call suspends execution of the calling thread  un‐
	   til  a  child specified by pid argument has changed state.  By default,
	   waitpid() waits only for terminated children, but this behavior is mod‐
	   ifiable via the options argument, as described below.
-1 : meaning wait for any child process.
*/

}

