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

void ft_reconnect_lst_token(t_list *lst_token, size_t position)
{
	t_list *curr;
	t_list *next;
	t_list *prev;

	curr = lst_token;
	next = NULL;
	prev = NULL;
	while(curr)
	{
		if(curr->position == position)
		{
			next = curr->next;
			prev = curr->prev;
			prev->next = next;
			if(next != NULL)
				next->prev = prev;
		}
		curr = curr->next;
	}
}

t_settings_del *ft_redir_err_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	//TODO REGLER L ORDRE COMME POUR LES OUT ET IN
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	ft_lstdelone(&lst_token_to_remove);
	curr->next->next->title = redir_err;

	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_errfile_before_nofile++;

	lst_token_to_remove2 = curr->next->next;
	curr->next = curr->next->next->next;
	ft_lstdelone2(&lst_token_to_remove2,
	 cmd->simpleCmds[del->index], del->k, cmd);
	del->k++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_settings_del *ft_redir_out_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	lst_token_to_remove2 = curr->next->next;
	if(curr->next->title == redir_out)
		curr->next->next->title = redir_out;
	if(curr->next->title == redir_append)
		curr->next->next->title = redir_append;
	curr->next = curr->next->next->next;
	ft_lstdelone(&lst_token_to_remove);//semble etre normalise NANI DANS DEBUGGOR CONTENT DE CURR NEXT EST MIS EN ERROR CANNOT ACCESS
	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_outfile_before_nofile++;
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->j, cmd);
	del->j++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_settings_del *ft_redir_in_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int fdin;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	lst_token_to_remove2 = curr->next->next;
	if(curr->next->title == redir_in)
		curr->next->next->title = redir_in;
	if(curr->next->title == redir_heredoc)
		curr->next->next->title = redir_heredoc;
	if(cmd->simpleCmds[del->index]->nofile == 0 && curr->next->next->title == redir_in)
	{
		if((fdin = open(lst_token_to_remove2->content, O_RDONLY)) == -1)
			cmd->simpleCmds[del->index]->nofile = 1;
		else
			close(fdin);
	}
	curr->next =  curr->next->next->next;//ls
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->i, cmd);
	del->i++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_settings_del 	*ft_redir_err_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	//TODO REGLER L ORDRE COMME POUR LES OUT ET IN
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;

	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_errfile_before_nofile++;

	curr->next->title = redir_err;
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->k, cmd);
	del->k++;
	return(del);
}

t_settings_del *ft_redir_out_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;
	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_outfile_before_nofile++;
	if(curr->title == redir_out)
		curr->next->title = redir_out;
	if(curr->title == redir_append)
		curr->next->title = redir_append;
	if(del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
		ft_reconnect_lst_token(lst_token, lst_token_to_remove2->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->j, cmd);
	del->j++;
	return(del);
}

t_settings_del	*ft_redir_in_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int fdin;

	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;
	if(curr->title == redir_in)
		curr->next->title = redir_in;
	if(curr->title == redir_heredoc)
		curr->next->title = redir_heredoc;
	if(cmd->simpleCmds[del->index]->nofile == 0  && curr->next->title == redir_in) //J AI RETIRE UN ->next
	{
		if((fdin = open(lst_token_to_remove2->content, O_RDONLY)) == -1) //TODO revenir ici
			cmd->simpleCmds[del->index]->nofile = 1;
		else
			close(fdin);
	}
	if(del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
		ft_reconnect_lst_token(lst_token, lst_token_to_remove2->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->i, cmd);
	del->i++;
	return(del);
}

void	ft_flag_head_list(t_cmd *cmd, t_settings_del *del)
{
	if(cmd->flag_head_list == -1 && del->index == 0)
		cmd->flag_head_list = 1;
}


t_list	*ft_middle_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	if(curr->next->title == redir_in || curr->next->title == redir_heredoc)
		del = ft_redir_in_middle(curr, cmd, lst_token, del);
	else if(curr->next->title == redir_out || curr->next->title == redir_append)
		del = ft_redir_out_middle(curr, cmd, lst_token, del);
	else if(curr->next->title == redir_err)
		del = ft_redir_err_middle(curr, cmd, lst_token, del);
	else
		curr = curr->next;	
	return(curr);
}

t_list	*ft_head_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list			*next;

	next = NULL;
	ft_flag_head_list(cmd, del);
	next = curr->next->next;
	if(next && del->index == 0)
		next->prev = NULL;
	if(curr->title == redir_in || curr->title == redir_heredoc)
		del = ft_redir_in_head(curr, cmd, lst_token, del);
	else if(curr->title == redir_out || curr->title == redir_append)
		del = ft_redir_out_head(curr, cmd, lst_token, del);
	else if(curr->title == redir_err)
		del = ft_redir_err_head(curr, cmd, lst_token, del);
	curr = next;
	return(curr);
}

void	ft_del_and_parse_redir_token_in_simpleCmd(t_list **alst, size_t index, t_list **lst_token, t_cmd *cmd)
{
	t_list			*curr;
	t_settings_del	*del;

	curr = *alst;
	if(*alst == NULL)
		return;
	del = ft_struct_init_settings_del(&del);//proteger del si ==NULL
	del->index = index;
	curr = *alst;
	while (curr !=NULL && curr->next != NULL && (curr->title == redir_out || curr->title == redir_in || curr->title == redir_heredoc || curr->title == redir_err))
		curr = ft_head_redir_token(curr, cmd, *lst_token, del);
	if(cmd->flag_head_list == 1)
		{
			*lst_token = curr;
			cmd->flag_head_list = 0;
		}
	*alst = curr;
	while(curr != NULL && (curr->position < cmd->simpleCmds[del->index]->end_simpleCmd_pos) && (curr->next != NULL && curr->next->position < cmd->simpleCmds[del->index]->end_simpleCmd_pos))
		curr = ft_middle_redir_token(curr, cmd, *lst_token, del);
	ft_free_struct_t_settings_del(&del);
}

t_list 	*ft_readjust_start_lst_token(t_list *start_lst_token, t_cmd *cmd, size_t i)
{
	while(start_lst_token->position < cmd->simpleCmds[i]->end_simpleCmd_pos)
		start_lst_token = start_lst_token->next;//il faut ramener a end_token_pos
	if(start_lst_token->position == cmd->simpleCmds[i]->end_simpleCmd_pos)
		start_lst_token = start_lst_token->next;
	return(start_lst_token);
}


char	**ft_malloc_heredocs_tab(t_cmd *cmd)
{
	char **heredocs;

	heredocs = NULL;
	heredocs = malloc(sizeof(*heredocs) * (cmd->nb_of_heredocs + 1));
	if(!heredocs)
		return(NULL);
	return(heredocs);
}

void	ft_count_nbr_of_heredocs_in_cmd_line(t_cmd *cmd)
{
	t_list *tmp;

	tmp = cmd->lst_token;
	while(tmp != NULL)
	{
		if(tmp->title == redir_heredoc)
			{
				cmd->nb_of_heredocs++;
				tmp =tmp->next;
			}
		tmp = tmp->next;
	}
}

void	ft_malloc_heredocs_of_cmd(t_cmd *cmd)
{
	ft_count_nbr_of_heredocs_in_cmd_line(cmd);
	if(cmd->nb_of_heredocs != 0)
		cmd->heredocs = ft_malloc_heredocs_tab(cmd);
}


int ft_get_last_heredoc_index(int *tab, int len)
{
	int simpleCmd_index;

	len = len - 1;
	simpleCmd_index = len;
	if(!len)
		return (0);
	while(len >= 0)
	{
		if(tab[len] == 0)
			len = len -1;
		else
		{
			simpleCmd_index = len;
			break;
		}
	}
	return (simpleCmd_index);
}

int ft_get_max_heredoc_index(int *tab, int len)
{
	int max_value;
	int simpleCmd_index;

	if(!len)
		return (0);
	max_value = tab[--len];
	simpleCmd_index = len;
	while(len--)
	{
		if(tab[len]>max_value)
			{
				max_value = tab[len];
				simpleCmd_index = len;
			}
	}
	return (simpleCmd_index);
}

void	ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd)
{
	int i;

	i = 0;
	//TODO proteger si heredoc_track_index a un pb
	while(simpleCmd->heredoc_track_index[i]!= '\0')
		i++;
	
	while(i)
	{
		i--;
		if(simpleCmd->heredoc_track_index[i] == 1 )//simpleCmd->heredoc_track_index[i] != '\0' && 
			{
				simpleCmd->heredoc_track_index[i] = 42;
				break;
			}
	}
	//printf(" index de heredoc_track_index : i = %d et de infile : %s\n", i, simpleCmd->infile[i]);

}

void	ft_get_last_heredoc_position(t_cmd *cmd)
{
	int	simpleCmd_index;
	simpleCmd_index = 0;
	if(cmd->nb_of_heredocs != 0)
	{
		//1 recuperer l index de la simpleCmd qui a le plus de heredoc
	//	simpleCmd_index = ft_get_max_heredoc_index(cmd->heredocs_track_index, cmd->nb_of_simpleCmds);
		//printf("simpleCmd_index = %d\n", simpleCmd_index);
		simpleCmd_index = ft_get_last_heredoc_index(cmd->heredocs_track_index, cmd->nb_of_simpleCmds);

		//2 parcourir l infile prendre le last infile et lui mettre le tag de 42 dans le heredoc_track_index de la simpleCmd
		ft_tag_last_heredoc_in_infile(cmd->simpleCmds[simpleCmd_index]);

		//3 mettre dans l execution (setting redirections) la condition pour empecher les heredocs d etre ouverts comme des infile, SAUF le Last qui a le tag. auquel cas il faudra l ouvrir  
	}
}


t_list	*ft_lst_first(t_list *lst)
{

	if(!lst)
		return (NULL);
	while (lst->prev)
		lst = lst->prev;
	return(lst);

}

int		ft_parse_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token)
{
	int		exec_return;
	size_t	i;
	t_list	*start_lst_token;

	(void)line;
	(void)envp;
	exec_return = 0;
	i = 0;
	start_lst_token = lst_token;
	ft_malloc_heredocs_of_cmd(cmd);
	while (i < cmd->nb_of_simpleCmds && start_lst_token != NULL)
	{
		ft_get_end_simpleCmd_pos(cmd, cmd->simpleCmds[i], &start_lst_token); 
		ft_count_nb_of_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i], start_lst_token, i);
		ft_malloc_redir_file_tabs_of_simpleCmd(cmd->simpleCmds[i]);
		if(cmd->simpleCmds[i]->nb_of_redir_token > 0)
			ft_del_and_parse_redir_token_in_simpleCmd(&start_lst_token, i, &lst_token, cmd);
		ft_aff_list_ptr_sur_char_content(lst_token);
		ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		if(start_lst_token != NULL)
			start_lst_token = ft_readjust_start_lst_token(start_lst_token, cmd, i);
		i++;
	}
	ft_get_last_heredoc_position(cmd);
	lst_token = ft_lst_first(lst_token);
	cmd->lst_token = lst_token;
	ft_aff_abs_cmd_and_args(cmd);
	return (exec_return);
}
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);
