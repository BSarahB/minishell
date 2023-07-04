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
			printf("cmd_and_args de la simpleCmd[%zu] : <%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
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

void	ft_count_final_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	size_t	token_in_simpleCmd_nbr;
	t_list	*tmp;
	(void)simpleCmd;
	(void)k;

	token_in_simpleCmd_nbr = 0;
	tmp = start_lst_token;

	
	while (tmp !=NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
	{
		token_in_simpleCmd_nbr++;

		tmp = tmp->next;
	}
	
	if(tmp !=NULL && tmp->position == simpleCmd->end_simpleCmd_pos && tmp->title != operator)
	{
		token_in_simpleCmd_nbr++;
		simpleCmd->nb_of_tokens_in_simpleCmd = token_in_simpleCmd_nbr;
		return;
	}
	simpleCmd->nb_of_tokens_in_simpleCmd = token_in_simpleCmd_nbr;
}


void	parse(char *content, t_simpleCmd *simpleCmd, size_t i, int title)
{
	if(title == redir_out)
		simpleCmd->outfile[i] = content;
	if(title == redir_in)
		simpleCmd->infile[i] = content;
	if(title == redir_err)
		simpleCmd->errfile[i] = content;

}

void	ft_lstdelone(t_list *lst, void(*parse)(char *content, t_simpleCmd *simpleCmd, size_t i, int title), t_simpleCmd *simpleCmd, size_t i, int redir)
{
	 if(lst && parse)
	 {
		if(redir == 1)
			parse(lst->content, simpleCmd, i, lst->title);
		//free ts les mallocs ici de str etc...
		lst->content = NULL;	
		free(lst);
		lst = NULL;
	 }
}


void	ft_del_and_parse2_redir_token_in_simpleCmd(t_list **alst, t_simpleCmd *simpleCmd, t_list **lst_token_addr)
{
	t_list *curr;
	//t_list *tmp;
	size_t i;
	size_t j;
	size_t k;
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;


	(void)lst_token_addr;
	curr = *alst;
//	tmp = start_lst_token;
	i = 0;
	j = 0;

	k = 0;
	if(*alst == NULL)
		return;

	//ft_tag_redir_title_token(); //cf si necessaire car je risque de segfault si je vais chercher un next->next si jamais.... a verifier, je pourrais ainsi dans le tag redir ainsi gerer les bash syntax .... 

	//je m occupe du 1 er maillon, car il pourrait tres bien etre une redirection >outfile 

	//TODO redir _in et err
	while(((*alst)->title == redir_out || (*alst)->title == redir_in || (*alst)->title == redir_err) && (*alst)->next != NULL) 
	{
		lst_token_to_remove = *alst;
		lst_token_to_remove2 = (*alst)->next;
		if((*alst)->title == redir_in)
		{
			(*alst)->next->title = redir_in;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, i, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, i, 1);
			i++;
		}
		if((*alst)->title == redir_out)
		{
			(*alst)->next->title = redir_out;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, j, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, j, 1);
			j++;
		}
		if((*alst)->title == redir_err)
		{
			(*alst)->next->title = redir_err;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, k, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, k, 1);
			k++;
		}	
		*alst = (*alst)->next->next;
 	
		
	}
	curr = *alst;
	*lst_token_addr = *alst;
	i = 0;
	while((curr->position < simpleCmd->end_simpleCmd_pos) && (curr->next != NULL && curr->next->position < simpleCmd->end_simpleCmd_pos))
	{
		if(curr->next->title == redir_in)
		{
			//bash syntax error si suivant le > on a un | ou un token fichier inexistant
			lst_token_to_remove = curr->next;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, i, 0);
			curr->next->next->title = redir_in;
			lst_token_to_remove2 = curr->next->next;
			curr->next = curr->next->next->next;
			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, i, 1);
			i++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;
		}
		else if(curr->next->title == redir_out)
		{
			lst_token_to_remove = curr->next;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, j, 0);
			curr->next->next->title = redir_out;
			lst_token_to_remove2 = curr->next->next;
			curr->next = curr->next->next->next;
			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, j, 1);
			j++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;
		}
		else if(curr->next->title == redir_err)
		{
			lst_token_to_remove = curr->next;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, k, 0);
			curr->next->next->title = redir_out;
			lst_token_to_remove2 = curr->next->next;
			curr->next = curr->next->next->next;
			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, k, 1);
			k++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;

		}
		else
			curr = curr->next;
	}

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

 void	ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *start_lst_token)
 {
	 t_list *tmp;
	(void)cmd;	//tmp = cmd->lst_token;
	 tmp = start_lst_token;
	while(tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
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

	//if tmp == NULL -> reactualiser la end_simpleCmd_pos ici semble premature
 }

void	ft_get_end_simpleCmd_pos2(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **start_lst_token)
{
	(void)cmd;
	(void)simpleCmd;
	t_list *tmp;

	tmp = *start_lst_token;
	while(tmp)
	{
		if(tmp->title == operator)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		if(tmp->next == NULL)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		tmp = tmp->next;
	}
}

void	ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **start_lst_token)
{
	(void)cmd;
	(void)simpleCmd;
	t_list *tmp;

	tmp = *start_lst_token;
	while(tmp)
	{
		if(tmp->title == operator)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		if(tmp->next == NULL)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		tmp = tmp->next;
	}
}

int	ft_parse_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token)
{
	int		exec_return;
	size_t	i;
	t_list	*start_lst_token;

	(void)line;
	(void)envp;
	exec_return = 0;
	i = 0;
	start_lst_token = lst_token;
	while (i < cmd->nb_of_simpleCmds && start_lst_token != NULL)
	{
		ft_get_end_simpleCmd_pos(cmd, cmd->simpleCmds[i], &start_lst_token); //voir si je le retourne ou sije le change juste en memmoire (supprimer )
		ft_count_nb_of_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i],start_lst_token);
		ft_malloc_redir_file_tabs_of_simpleCmd(cmd->simpleCmds[i]);
		if(cmd->simpleCmds[i]->nb_of_redir_token > 0)
			ft_del_and_parse2_redir_token_in_simpleCmd(&start_lst_token, cmd->simpleCmds[i],&(cmd->lst_token));
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);
	//	ft_aff_list_ptr_sur_char_content(lst_token);
		ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token,cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		while(start_lst_token->position < cmd->simpleCmds[i]->end_simpleCmd_pos)
			start_lst_token = start_lst_token->next;//il faut ramener a end_token_pos
		if(start_lst_token->position == cmd->simpleCmds[i]->end_simpleCmd_pos)
			start_lst_token = start_lst_token->next;
		i++;
	}

	ft_aff_abs_cmd_and_args(cmd);
	return (exec_return);
}





































































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
