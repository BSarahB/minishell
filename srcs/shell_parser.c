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
		while(cmd->simpleCmds[i]->nb_of_tokens_in_simpleCmd == 0 &&  cmd->simpleCmds[i+1] != NULL)
			i++;//protections pour >a | ls |grep c
		if(cmd->simpleCmds[i] != NULL && cmd->simpleCmds[i]->nb_of_tokens_in_simpleCmd != 0) 
		{
			while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
		//	printf("cmd_and_args de la simpleCmd[%zu] : <%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;

		}
		//printf("\n\n\n");
		
			
		j= 0;
	
		
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
		//	printf("abs_cmd_and_args[%zu]<%s>\n",i, cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
	//	printf("\n\n\n");
		i++;
		}
		else
			break;

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
	if(simpleCmd->nb_of_tokens_in_simpleCmd == 0)
		return(0);
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
	{
		simpleCmd->outfile[i] = content; //ft_strdup(content);
		//if (i == simpleCmd->nb_of_outfile -1) //val error invalid write
		//	simpleCmd->outfile[i + 1] = 0;
	} 
	if(title == redir_in)
		simpleCmd->infile[i] = content; //ft_strdup(content);
	if(title == redir_err)
		simpleCmd->errfile[i] = content; //ft_strdup(content);

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


void	ft_del_and_parse2_redir_token_in_simpleCmd(t_list **alst, t_simpleCmd *simpleCmd, t_list **lst_token)
{
	t_list *curr;
	//t_list *tmp;
	size_t i;
	size_t j;
	size_t k;
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int		fdin;


	(void)lst_token;
	curr = *alst;
//	tmp = start_lst_token;
	i = 0;
	j = 0;
	k = 0;
	if(*alst == NULL)
		return;
	curr = *alst;
	while (curr !=NULL && curr->next != NULL && (curr->title == redir_out || curr->title == redir_in || curr->title == redir_err))
	{
		lst_token_to_remove = curr;
		lst_token_to_remove2 = curr->next;
		if(curr->title == redir_in)
		{
			curr->next->title = redir_in;
			if(simpleCmd->nofile == 0)
			{
				if((fdin = open(lst_token_to_remove2->content, O_RDONLY) == -1))
					simpleCmd->nofile = 1;
			}
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, i, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, i, 1);
			i++;
		}
		if(curr->title == redir_out)
		{
			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_outfile_before_nofile++;
			curr->next->title = redir_out;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, j, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, j, 1);
			j++;
		}
		if(curr->title == redir_err)
		{
			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_errfile_before_nofile++;

			curr->next->title = redir_err;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, k, 0);
 			ft_lstdelone(lst_token_to_remove2, &parse, simpleCmd, k, 1);
			k++;
		}	
		curr = curr->next->next;	
	}

	*lst_token = curr;
	//printf("affiche list avant del");
	ft_aff_list_ptr_sur_char_content(*lst_token);

	*alst = curr;
	while(curr != NULL && (curr->position < simpleCmd->end_simpleCmd_pos) && (curr->next != NULL && curr->next->position < simpleCmd->end_simpleCmd_pos))
	{
		if(curr->next->title == redir_in)
		{
			//bash syntax error si suivant le > on a un | ou un token fichier inexistant
			lst_token_to_remove = curr->next;
			ft_lstdelone(lst_token_to_remove, &parse, simpleCmd, i, 0);
			curr->next->next->title = redir_in;
			lst_token_to_remove2 = curr->next->next;

			if(simpleCmd->nofile == 0)
			{
				if((fdin = open(lst_token_to_remove2->content, O_RDONLY) == -1))
					simpleCmd->nofile = 1;
			}
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

			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_outfile_before_nofile++;

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
			curr->next->next->title = redir_err;

			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_errfile_before_nofile++;

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
	//outfile[simpleCmd->nb_of_outfile + 1] = 0; // val invalid write
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
	(void)cmd;
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
		ft_get_end_simpleCmd_pos(cmd, cmd->simpleCmds[i], &start_lst_token); 
		ft_count_nb_of_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i],start_lst_token);
		ft_malloc_redir_file_tabs_of_simpleCmd(cmd->simpleCmds[i]);
		if(cmd->simpleCmds[i]->nb_of_redir_token > 0)
			ft_del_and_parse2_redir_token_in_simpleCmd(&start_lst_token, cmd->simpleCmds[i], &lst_token);
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);
		ft_aff_list_ptr_sur_char_content(lst_token);
		ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		if(start_lst_token != NULL)
		{	
			while(start_lst_token->position < cmd->simpleCmds[i]->end_simpleCmd_pos)
				start_lst_token = start_lst_token->next;//il faut ramener a end_token_pos
			if(start_lst_token->position == cmd->simpleCmds[i]->end_simpleCmd_pos)
				start_lst_token = start_lst_token->next;
		}
		i++;
	}

	ft_aff_abs_cmd_and_args(cmd);
	return (exec_return);
}
