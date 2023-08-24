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


void	ft_first_token_in_simpleCmd_is_redir()
{

	
}


void	ft_del_and_parse_redir_token_in_simpleCmd(t_list **alst, t_simpleCmd *simpleCmd, t_list **lst_token, t_cmd *cmd)
{
	t_list *curr;
	size_t i;
	size_t j;
	size_t k;
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int		fdin;

	(void)lst_token;
	curr = *alst;
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
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, i, 0, cmd);
 			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, i, 1, cmd);
			i++;
		}
		if(curr->title == redir_out || curr->title == redir_append)
		{
			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_outfile_before_nofile++;
			if(curr->title == redir_out)
				curr->next->title = redir_out;
			if(curr->title == redir_append)
				curr->next->title = redir_append;
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, j, 0, cmd);
 			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, j, 1, cmd);
			j++;
		}
		if(curr->title == redir_err)
		{
			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_errfile_before_nofile++;

			curr->next->title = redir_err;
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, k, 0, cmd);
 			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, k, 1, cmd);
			k++;
		}	
		curr = curr->next->next;	
	}
	*lst_token = curr;
	ft_aff_list_ptr_sur_char_content(*lst_token);
	*alst = curr;
	while(curr != NULL && (curr->position < simpleCmd->end_simpleCmd_pos) && (curr->next != NULL && curr->next->position < simpleCmd->end_simpleCmd_pos))
	{
		if(curr->next->title == redir_in || curr->next->title == redir_heredoc)
		{
			//bash syntax error si suivant le > on a un | ou un token fichier inexistant
			if(curr->next->title == redir_in)
				curr->next->next->title = redir_in;
			if(curr->next->title == redir_heredoc)
				curr->next->next->title = redir_heredoc;		
			lst_token_to_remove = curr->next;
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, i, 0, cmd);
			lst_token_to_remove2 = curr->next->next;

			if(simpleCmd->nofile == 0 && curr->next->next->title == redir_in) 
			{
				if((fdin = open(lst_token_to_remove2->content, O_RDONLY) == -1))
					simpleCmd->nofile = 1;
			}
			curr->next = curr->next->next->next;
			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, i, 1, cmd);
			i++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;
		}
		else if(curr->next->title == redir_out || curr->next->title == redir_append)
		{
			if(curr->next->title == redir_out)
				curr->next->next->title = redir_out;
			if(curr->next->title == redir_append)
				curr->next->next->title = redir_append;	
			lst_token_to_remove = curr->next;
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, j, 0, cmd);//semble etre normalise NANI DANS DEBUGGOR CONTENT DE CURR NEXT EST MIS EN ERROR CANNOT ACCESS
			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_outfile_before_nofile++;

			lst_token_to_remove2 = curr->next->next;
			curr->next = curr->next->next->next;
			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, j, 1, cmd);
			j++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;
		}
		else if(curr->next->title == redir_err)
		{
			lst_token_to_remove = curr->next;
			ft_lstdelone(&lst_token_to_remove, &parse, simpleCmd, k, 0, cmd);
			curr->next->next->title = redir_err;

			if(simpleCmd->nofile == 0)
					simpleCmd->nb_of_errfile_before_nofile++;

			lst_token_to_remove2 = curr->next->next;
			curr->next = curr->next->next->next;
			ft_lstdelone(&lst_token_to_remove2, &parse, simpleCmd, k, 1, cmd);
			k++;
			if(curr->next == NULL)
				simpleCmd->end_simpleCmd_pos = curr->position;
		}
		else
			curr = curr->next;
	}

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
	printf(" index de heredoc_track_index : i = %d et de infile : %s\n", i, simpleCmd->infile[i]);

}

void	ft_get_last_heredoc_position(t_cmd *cmd)
{
	int	simpleCmd_index;
	simpleCmd_index = 0;
	if(cmd->nb_of_heredocs != 0)
	{
		//1 recuperer l index de la simpleCmd qui a le plus de heredoc
		simpleCmd_index = ft_get_max_heredoc_index(cmd->heredocs_track_index, cmd->nb_of_simpleCmds);
		printf("simpleCmd_index = %d\n", simpleCmd_index);
		//2 parcourir l infile prendre le last infile et lui mettre le tag de 42 dans le heredoc_track_index de la simpleCmd
		ft_tag_last_heredoc_in_infile(cmd->simpleCmds[simpleCmd_index]);

		//3 mettre dans l execution (setting redirections) la condition pour empecher les heredocs d etre ouverts comme des infile, SAUF le Last qui a le tag. auquel cas il faudra l ouvrir  
	}
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
			ft_del_and_parse_redir_token_in_simpleCmd(&start_lst_token, cmd->simpleCmds[i], &lst_token, cmd);
		ft_aff_list_ptr_sur_char_content(lst_token);
		ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		if(start_lst_token != NULL)
			start_lst_token = ft_readjust_start_lst_token(start_lst_token, cmd, i);
		i++;
	}
	ft_get_last_heredoc_position(cmd);
	cmd->lst_token = lst_token;
	ft_aff_abs_cmd_and_args(cmd);
	return (exec_return);
}
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);