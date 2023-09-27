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
	while (curr !=NULL && curr->next != NULL && (curr->title == redir_out || curr->title == redir_append || curr->title == redir_in || curr->title == redir_heredoc || curr->title == redir_err))
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
		start_lst_token = start_lst_token->next;
	if(start_lst_token->position == cmd->simpleCmds[i]->end_simpleCmd_pos)
		start_lst_token = start_lst_token->next;
	return(start_lst_token);
}

t_list	*ft_lst_first(t_list *lst)
{
	if(!lst)
		return (NULL);
	while (lst->prev)
		lst = lst->prev;
	return(lst);
}

int		ft_parse_tokens_in_s_cmd(t_cmd *cmd, t_list *lst_token)
{
	int		exec_return;
	size_t	i;
	t_list	*start_lst_token;

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
		ft_retokenize_and_dequote_token(cmd, start_lst_token, cmd->simpleCmds[i]);
		ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		if(start_lst_token != NULL)
			start_lst_token = ft_readjust_start_lst_token(start_lst_token, cmd, i);
		
		//boucler sur les tokens jusqu a la end_simpleCmd_pos
		//ft_retokenize_and_dequote(cmd);
		printf("list_durant_le parsing avec i = %zu\n", i);
		ft_aff_list_ptr_sur_char_content(lst_token);
		i++;
	}
	ft_get_last_heredoc_position(cmd);
	lst_token = ft_lst_first(lst_token);
	cmd->lst_token = lst_token;
	return (exec_return);
}
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);
	//ft_aff_abs_cmd_and_args(cmd);
	//ft_aff_list_ptr_sur_char_content(lst_token);