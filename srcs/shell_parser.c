/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*

 int	ft_lst_token_redir_token_alone(char *token_content)
 {
	(void)token_content;
	//je vais comparer strcmp a > ou < ou autres
	//if(ft_strcmp)
	return(1); //essai avec >
 }
*/

void	ft_aff_abs_cmd_and_args(t_cmd	*cmd)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < cmd->nb_of_simpleCmds)
	{
		j = 0;
		while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
			printf("cmd_and_args[%zu]<%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;

		}
		printf("\n\n\n");
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
			printf("abs_cmd_and_args[%zu]<%s>\n",i, cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
		printf("\n\n\n");
		i++;
	}
}

char	**ft_get_abs_argumentsb(char **abs_c_and_a)
{
	char	slash[2];

	slash[0] = '/';
	slash[1] = 0;
	ft_update_string(&abs_c_and_a[0], ft_strjoin(slash, abs_c_and_a[0]));
	return (abs_c_and_a);
}

int		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	char	**c_and_a;
	char	**abs_c_and_a;

	k = 0;
	c_and_a = malloc(sizeof(*c_and_a) * (simpleCmd->nb_of_tokens_in_simpleCmd + 1));
	abs_c_and_a = malloc(sizeof(*abs_c_and_a) * (simpleCmd->nb_of_tokens_in_simpleCmd + 1));
	if(!c_and_a)
		return(0);
	if(!abs_c_and_a)
		return(0);
	while (k < simpleCmd->nb_of_tokens_in_simpleCmd)
	{
		c_and_a[k] = lst_token->content;
		abs_c_and_a[k] = lst_token->content;
		lst_token = lst_token->next;
		k++;
	}
	c_and_a[k] = NULL;
	simpleCmd->cmd_and_args = c_and_a;
	abs_c_and_a[k] = NULL;
	abs_c_and_a = ft_get_abs_argumentsb(abs_c_and_a);
	simpleCmd->abs_cmd_and_args = abs_c_and_a;
	return(1);
}

size_t	ft_count_final_nb_of_tokens_in_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	size_t	token_in_simpleCmd_nbr;
	t_list	*tmp;
	(void)lst_token;
	(void)simpleCmd;
	(void)k;

	token_in_simpleCmd_nbr = 0;
	tmp = lst_token;
	while (tmp)
	{
		break;
	}
	return (token_in_simpleCmd_nbr);
}

void	ft_delete_redir_token_from_lst_token(t_cmd *cmd, t_simpleCmd *simpleCmd)
{
	t_list *lst_token;

	lst_token = cmd->lst_token;
	//on travaille directement sur la vraie lst_token

	(void)simpleCmd;
	cmd->lst_token = lst_token;
}

void	ft_parse_infile_in_simpleCmd(t_simpleCmd *simpleCmd)
{
	(void)simpleCmd;
}

void	ft_parse_outfile_in_simpleCmd(t_simpleCmd *simpleCmd)
{
	(void)simpleCmd;

}

void	ft_parse_errfile_in_simpleCmd(t_simpleCmd *simpleCmd)
{
	(void)simpleCmd;

}

void	ft_parse_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *dynamic_lst_token)
{
	t_list *tmp;
	t_list *tmp_dynamic;
	t_list	*tmp_to_del;


	tmp = cmd->lst_token;
	tmp_dynamic = dynamic_lst_token;

	(void)tmp;
	(void)tmp_dynamic;

	while(tmp->position < tmp_dynamic->position)
	{

		if(tmp->title == redir_in)
		{
			tmp =tmp->next;
			//bash syntax error si suivant le > on a un | ou un token fichier inexistant
			ft_delete_redir_token_from_lst_token()


		}
		else if(tmp->title == redir_out)
		{
			simpleCmd->nb_of_outfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
		else if(tmp->title == redir_err)
		{
			simpleCmd->nb_of_errfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
			tmp = tmp->next;
	}


	ft_parse_infile_in_simpleCmd(simpleCmd);
	ft_parse_outfile_in_simpleCmd(simpleCmd);
	ft_parse_errfile_in_simpleCmd(simpleCmd);
}

char **ft_malloc_errfile_tab(t_simpleCmd *simpleCmd)
{
	char **errfile;

	errfile = malloc(sizeof(*errfile) * (simpleCmd->nb_of_errfile + 1));
	if(!errfile)
		return(NULL);
	return(errfile);
}

char **ft_malloc_infile_tab(t_simpleCmd *simpleCmd)
{
	char **infile;

	infile = malloc(sizeof(*infile) * (simpleCmd->nb_of_infile + 1));
	if(!infile)
		return(NULL);
	return(infile);
}

char **ft_malloc_outfile_tab(t_simpleCmd *simpleCmd)
{
	char **outfile;

	outfile = malloc(sizeof(*outfile) * (simpleCmd->nb_of_outfile + 1));
	if(!outfile)
		return(NULL);
	return(outfile);
}
void	ft_malloc_redir_file_tabs_of_simpleCmd(t_simpleCmd *simpleCmd)
{
	if(simpleCmd->nb_of_redir_token != 0)
	{
		if(simpleCmd->nb_of_outfile != 0)
			simpleCmd->outfile = ft_malloc_outfile_tab(simpleCmd);
		if(simpleCmd->nb_of_infile != 0)
			simpleCmd->infile = ft_malloc_infile_tab(simpleCmd);
		if(simpleCmd->nb_of_errfile != 0)
			simpleCmd->errfile = ft_malloc_errfile_tab(simpleCmd);
	}
}

 void	ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *dynamic_lst_token)
 {
	 t_list *tmp;
	 t_list *tmp_dynamic;

	tmp = cmd->lst_token;
	tmp_dynamic = dynamic_lst_token;
	(void)tmp;
	(void)tmp_dynamic;
	while(tmp->position < tmp_dynamic->position)
	{
		if(tmp->title == redir_in)
		{
			simpleCmd->nb_of_infile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp =tmp->next;
		}
		else if(tmp->title == redir_out)
		{
			simpleCmd->nb_of_outfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
		else if(tmp->title == redir_err)
		{
			simpleCmd->nb_of_errfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
			tmp = tmp->next;
	}
 }

t_list	*ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **dynamic_lst_token)
{
	(void)cmd;
	(void)simpleCmd;
	t_list *tmp;

	tmp = *dynamic_lst_token;
	while(tmp)
	{
		if(tmp->title == operator)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			*dynamic_lst_token = tmp;
			break;
		}

		tmp = tmp->next;
	}
	return(tmp);
}

int	ft_parse_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token)
{
	int		exec_return;
	size_t	i;
	t_list	*dynamic_lst_token;

	(void)line;
	(void)envp;
	exec_return = 0;
	i = 0;
	dynamic_lst_token = lst_token;
	while (i < cmd->nb_of_simpleCmds)
	{
		if(cmd->nb_of_simpleCmds >1)
			dynamic_lst_token = ft_get_end_simpleCmd_pos(cmd, cmd->simpleCmds[i], &dynamic_lst_token); //voir si je le retourne ou sije le change juste en memmoire (supprimer )
		ft_count_nb_of_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i],dynamic_lst_token);
		ft_malloc_redir_file_tabs_of_simpleCmd(cmd->simpleCmds[i]);
		if(cmd->simpleCmds[i]->nb_of_redir_token > 0)
			ft_parse_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i], dynamic_lst_token);//ft_lstdelone(t_list *lst, void (*del)(void *))
		ft_delete_redir_token_from_lst_token(cmd, cmd->simpleCmds[i]);
		ft_count_final_nb_of_tokens_in_simpleCmd(lst_token,cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(lst_token, cmd->simpleCmds[i]);
		i++;
	}

	ft_aff_abs_cmd_and_args(cmd);
	return (exec_return);
}

//TODO : bien REGLER le JEU de la liste dynamique de token et la end_cimplecmd pos comme borne
// a chaque depart, je vais avoir une end_simpleCmd_pos, ce sera ma borne [ et cette derniere deviendra au maillon suivant le dynamic lst












































	 /*
	 char	**ft_get_abs_arguments(int i, char **c_and_a_copy, size_t nb_of_tokens_in_simpleCmd)
	 {
		 (void)blocks;
		 char	**abs_c_and_a;
		 char	slash[2];


		 slash[0] = '/';
		 slash[1] = 0;
	 //	abs_c_and_a = ft_split(blocks[i], ' ');
		 abs_c_and_a = c_and_a_copy;

		 //abs_c_and_a = malloc(sizeof(*abs_c_and_a) * (nb_of_tokens_in_simpleCmd + 1));
		 if(!abs_c_and_a)
		 return(NULL);
		 ft_update_string(&abs_c_and_a[0], ft_strjoin(slash, abs_c_and_a[0]));
		 return (abs_c_and_a);
	 }
	 */
	 /*
	 char	**ft_get_abs_argumentsb(size_t i, char **abs_c_and_a)
	 {
		 char	slash[2];
		 (void)i;

		 slash[0] = '/';
		 slash[1] = 0;
		 ft_update_string(&abs_c_and_a[0], ft_strjoin(slash, abs_c_and_a[0]));
		 //ft_strjoin(slash, abs_c_and_a[0]);
		 return (abs_c_and_a);
	 }
	 */

 /*
size_t	ft_count_nb_of_tokens_in_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd)
{
//je parcours la liste chainee jusqu a ce que je tombe sur un opeartor,
	//si je tombe sur > token redir title seul (get content compare) le token d apres est considere comme outfile
	//si je tombe sur < token redir title seul (get content compare) le token d apres est considere comme infile
	// et on skippe ces 2 tokens

	size_t	k;
	size_t	token_in_simpleCmd_nbr;
	t_list	*tmp;

	token_in_simpleCmd_nbr = 0;
	tmp = lst_token;

	while (tmp)
	{
		if (tmp->title == operator)
				{
					simpleCmd->number_of_tokens_in_simpleCmd = token_in_simpleCmd_nbr;
					lst_token++;
					break;
		}
		k = 0;
		while (tmp && tmp->title != operator)
		{
			if (tmp->title != redir_in || tmp->title != redir_out)
			{
				k++;
				break;
			}
			else if ((tmp->title == redir_in || tmp->title == redir_out) && ft_lst_token_redir_token_alone(tmp->content))
			{

				//ft_remove_token_redir_from_list()
			//	void	ft_lstdelone(t_list *lst, void (*del)(void *))
				//{
					//if (lst && del)
					//{
						//del(lst->content);
						//free(lst);
						//lst = NULL;
					//}
				//}


				tmp = tmp->next;
				tmp = tmp->next;//sil existe l >


			}
			else if((tmp->title == redir_in || tmp->title == redir_out) && ft_lst_token_redir_token_alone(tmp->content) == 0)
			{
				//ft_remove_token_redir_from_list()
				tmp = tmp->next;

			}
		}
		if (k != 0)
			token_in_simpleCmd_nbr++;
		//
		tmp = tmp->next;

	}
	return (token_in_simpleCmd_nbr);
}



	 */


























/*mode push = 1 (le split sur | version pour tester les multiples pipes)
int	ft_split_line_in_s_cmd(t_cmd *cmd, char *line, char **envp)
{

	char	**c_and_a;
	char	**abs_c_and_a;
	int		exec_return;
	size_t		i;
	int		j;

	(void)envp;
	(void)line;
	exec_return = 0;
	i = 0;
	j = 0;

	while (i < cmd->nb_of_simpleCmds)
	{
		c_and_a = ft_split(cmd->blocks[i], ' ');
		printf("%s\n", c_and_a[0]);
		abs_c_and_a = ft_get_abs_arguments(i, cmd->blocks);
		cmd->simpleCmds[i]->cmd_and_args = c_and_a;
		cmd->simpleCmds[i]->abs_cmd_and_args = abs_c_and_a;
		i++;
	}
	i = 0;
	while (i < cmd->nb_of_simpleCmds)
	{
		j = 0;
	//	printf("blocks[%zu]: %s\n",i, cmd->blocks[i]);
	//	printf("pr cmd_and_args de simple_Cmd %zu on a : \n", i);
		while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
//			printf("%s*", cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;

		}
//		printf("\n");
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
///			printf("%s*", cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
	//	printf("\n");
		i++;
	}
	return (exec_return);
}


int	ft_parse_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token)
{
	char	**c_and_a;
	char	**abs_c_and_a;
	int		exec_return;
	size_t	i;
	int		j;
	size_t	k;
	size_t	nb_of_tokens_in_simpleCmd;


	(void)line;
	(void)envp;
	exec_return = 0;
	i = 0;
	j = 0;
	k = 0;

	while (i < cmd->nb_of_simpleCmds)
	{
		nb_of_tokens_in_simpleCmd = ft_count_nb_of_tokens_in_simpleCmd(lst_token, cmd->simpleCmds[i]);
		//si redirection dans la cmd_and_Args preciser/mentionner la redir dans la simple_Cmd
		c_and_a = malloc(sizeof(*c_and_a) * (nb_of_tokens_in_simpleCmd + 1));
		abs_c_and_a = malloc(sizeof(*abs_c_and_a) * (nb_of_tokens_in_simpleCmd + 1));

		if(!c_and_a)
			return(0);

		if(!abs_c_and_a)
			return(0);
		//je parcours ma liste chainee et remplis : lst_token->content est mis dans c_and_a[i]	
		//on termine 0 c_and_a
		k = 0;
		while (k < nb_of_tokens_in_simpleCmd)
		{
			c_and_a[k] = lst_token->content;
			abs_c_and_a[k] = lst_token->content;
			lst_token = lst_token->next;
			k++;
		}

		c_and_a[k] = NULL;
		cmd->simpleCmds[i]->cmd_and_args = c_and_a;

		abs_c_and_a[k] = NULL;

		abs_c_and_a = ft_get_abs_argumentsb(i, abs_c_and_a);
		cmd->simpleCmds[i]->abs_cmd_and_args = abs_c_and_a;
		i++;
	}
	i = 0;
	while (i < cmd->nb_of_simpleCmds)
	{
		j = 0;
		while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
			printf("cmd_and_args[%zu]<%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;

		}
		printf("\n\n\n");
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
			printf("abs_cmd_and_args[%zu]<%s>\n",i, cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
		printf("\n\n\n");
		i++;
	}
	return (exec_return);
}
*/
