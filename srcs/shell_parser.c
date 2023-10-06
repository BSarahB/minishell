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

void	ft_flag_empty_head_list(t_cmd *cmd, t_settings_del *del)
{
	if(cmd->flag_empty_head_list == -1 && del->index == 0)

		cmd->flag_empty_head_list = 1;
}

t_settings_del *ft_del_empty_in_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	//int fdin;

	(void)lst_token;
	lst_token_to_remove = curr->next;
//	if(cmd->simpleCmds[del->index]->nofile == 0 && curr->next->title == redir_in)
//	{
	//	if((fdin = open(lst_token_to_remove2->content, O_RDONLY)) == -1)
	//		cmd->simpleCmds[del->index]->nofile = 1;
	//	else
		//	close(fdin);
	//}
	curr->next =  curr->next->next;//ls
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;

	ft_lstdelone_beta(lst_token_to_remove);
	del->i++;
	
	return(del);
}

t_list	*ft_middle_empty_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	if(curr->next == NULL)
		curr = curr->next;
	else if(curr->next->tag_empty_cmd_before_DQ == 1)
	{
		del = ft_del_empty_in_middle(curr, cmd, lst_token, del);	
	}
	else
		curr = curr->next;	
	return(curr);
}

t_settings_del	*ft_del_empty_in_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	(void)cmd;
	t_list *lst_token_to_remove;

	lst_token_to_remove = curr;	
	if(del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	del->i++;
	return(del);
}

t_list	*ft_head_empty_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list			*next;

	next = NULL;
	ft_flag_empty_head_list(cmd, del);
	next = curr->next;
	if(next && del->index == 0)
		next->prev = NULL;
	if(curr->tag_empty_cmd_before_DQ)
		del = ft_del_empty_in_head(curr, cmd, lst_token, del);
	curr = next;
	return(curr);
}
void	ft_del_empty_token_in_simpleCmd(t_list **alst, size_t index, t_list **lst_token, t_cmd *cmd)
{
	t_list			*curr;
	t_settings_del	*del2;

	curr = *alst;
	if(*alst == NULL)
		return;
	del2 = ft_struct_init_settings_del(&del2);//proteger del si ==NULL
	del2->index = index;	
	curr = *alst;
	//pour $Q solo
	while( curr!= NULL && ft_strcmp(curr->content, "empty")== 0 && curr->tag_empty_cmd_before_DQ == 1)
		{
			curr = ft_head_empty_token(curr, cmd, *lst_token, del2);	
		}
	while (curr != NULL && curr->next != NULL && curr->tag_empty_cmd_before_DQ == 1)
		curr = ft_head_empty_token(curr, cmd, *lst_token, del2);
	if(cmd->flag_empty_head_list == 1)
		{
			*lst_token = curr;
			cmd->flag_empty_head_list = 0;
		}
	*alst = curr;
	while(curr != NULL && (curr->position <= cmd->simpleCmds[del2->index]->end_simpleCmd_pos))
		curr = ft_middle_empty_token(curr, cmd, *lst_token, del2);
	ft_free_struct_t_settings_del(&del2);
}

int ft_check_format_key_value(t_list *tmp)
{
	(void)tmp;
	return 0;
}



int	ft_key_value_is_valid(t_list *tmp)
{

	(void)tmp;
	char *str;
	int i;

	i = 0;
	str = tmp->content;
	if(str[i] == '\0')
	{
		ft_putstr_fd("bash: export: `': not a valid identifier\n", 2); //le echo $? est 1 car il y a une erreur
		return(0);//not valide
	}
	if(str[i])
	{
		if (ft_isalpha(str[i]) == 0 || str[i] != '_')
			{
				ft_putstr_fd("bash: export: `", 2);
				ft_putstr_fd(tmp->content, 2);
				ft_putstr_fd("': not a valid identifier\n", 2); //le echo $? est 1 car il y a une erreur
				return(0);
			} 
	}
	return(1); //-> valide
}




void ft_check_export(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	t_list *tmp;
	int flag_export;

	flag_export = -1;
	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if(flag_export == -1)
		{
			if(flag_export == -1 && (ft_strcmp("export", tmp->content) == 0))
			{
				flag_export = 1;
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = export; // == 3
				if(tmp->next == NULL)
		   			simpleCmd->export_no_option = 1;
				else
					tmp = tmp->next;
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->export_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
			}
			else
				flag_export = 0;
		}
	}

	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->export_no_option == 0 && flag_export == 1)
	{ 
		if(simpleCmd->nofile == 1)
			break;
    	if(simpleCmd->export_solo == 1)
        {
			if (ft_key_value_is_valid(tmp) == 1)
			{
				ft_check_format_key_value(tmp);
			}

			//1) KEY seule pas de =VALUE et valid identifier:
			//remplir lst_envp_d en ajoutant le maillon (pas dans lst_envp)
			//si key=VALUE ->on met key=value dans lst_envp && lst_envp_d
        }
		tmp = tmp->next;
	}
	/*
	if(tmp != NULL && tmp->position == simpleCmd->end_simpleCmd_pos && tmp->title != operator && simpleCmd->export_x == 0 && flag_export == 1)
	{
		//if
        //{
          // if()
        //}
		
	}
*/
}


int		ft_parse_tokens_in_s_cmd(t_cmd *cmd, t_list *lst_token)
{
	int		exec_return;
	size_t	i;
	t_list	*start_lst_token;
	t_list	*start_lst_token_retokenized;


	exec_return = 0;
	i = 0;
	start_lst_token = lst_token;
	ft_malloc_heredocs_of_cmd(cmd);
	while (i < cmd->nb_of_simpleCmds && start_lst_token != NULL)
	{
	//	printf("start_lst_token------->  <%s>\n", start_lst_token->content);
		ft_get_end_simpleCmd_pos(cmd, cmd->simpleCmds[i], &start_lst_token);
	//	printf(" END SCMD lst_token------->  <%s>\n",((ft_lstfind(cmd->lst_token, cmd->simpleCmds[i]->end_simpleCmd_pos))->content));//ne marche pas quand on a un empty token qu on delete de la liste chainee

		ft_count_nb_of_redir_token_in_simpleCmd(cmd, cmd->simpleCmds[i], start_lst_token, i);
		ft_malloc_redir_file_tabs_of_simpleCmd(cmd->simpleCmds[i]);
		if(cmd->simpleCmds[i]->nb_of_redir_token > 0)
			ft_del_and_parse_redir_token_in_simpleCmd(&start_lst_token, i, &lst_token, cmd);
		ft_del_empty_token_in_simpleCmd(&start_lst_token, i, &lst_token, cmd);	
		start_lst_token_retokenized = ft_retokenize_and_dequote_token_1(cmd, start_lst_token, cmd->simpleCmds[i]);
		//delete de la liste chainee les tokens empty avant dequote pour pouvoir faire le bon compte et le parsing des tokens
		//[peut etre quil ne faut pas delete, sous peine de creer un pb au niveau des pipe nrmalement non. mais checker d abord pour etre sure]
		//ft_count_final_nb_of_tokens_in_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
	//	ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(start_lst_token, cmd->simpleCmds[i]);
		//ft_count_final_nb_of_tokens_in_simpleCmd_retokenized(start_lst_token_retokenized, cmd->simpleCmds[i]);
		ft_check_export(cmd, start_lst_token_retokenized, cmd->simpleCmds[i]);
		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd_from_retokenized_lst(start_lst_token_retokenized, cmd->simpleCmds[i]);

		if(start_lst_token != NULL)
			start_lst_token = ft_readjust_start_lst_token(start_lst_token, cmd, i);

	//	if(start_lst_token != NULL)
		//	printf("start_lst_token------->  <%s>\n", start_lst_token->content);
	
		//printf("list_durant_le parsing avec i = %zu\n", i);
		//ft_aff_list_ptr_sur_char_content(lst_token);
		i++;
	}
	ft_get_last_heredoc_position(cmd);
	lst_token = ft_lst_first(lst_token);
	cmd->lst_token_retokenized = ft_lst_first(cmd->lst_token_retokenized);
//	ft_aff_list_ptr_sur_char_content3(cmd->lst_token_retokenized);

	cmd->lst_token = lst_token;
	
	
	return (exec_return);
}
	//	printf("outfile de %zu: [%s] +  [%s]  + [%s]\n",i, cmd->simpleCmds[i]->outfile[0],cmd->simpleCmds[i]->outfile[1],cmd->simpleCmds[i]->outfile[2]);
	//	printf("infile de %zu: [%s] +  [%s]  +[%s]\n",i, cmd->simpleCmds[i]->infile[0],cmd->simpleCmds[i]->infile[1], cmd->simpleCmds[i]->infile[2]);
	//ft_aff_abs_cmd_and_args(cmd);
	//ft_aff_list_ptr_sur_char_content(lst_token);