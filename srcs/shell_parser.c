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

void	ft_del_empty_token_in_simplecmd(t_list **alst, size_t index, \
										t_list **lst_token, t_cmd *cmd)
{
	t_list			*curr;
	t_settings_del	*del2;

	curr = *alst;
	if (*alst == NULL)
		return ;
	del2 = ft_struct_init_settings_del(&del2);
	del2->index = index;
	curr = *alst;
	while (curr != NULL && ft_strcmp(curr->content, "empty") == 0 && \
										curr->tag_empty_cmd_before_dq == 1)
		curr = ft_head_empty_token(curr, cmd, *lst_token, del2);
	while (curr != NULL && curr->next != NULL && \
									curr->tag_empty_cmd_before_dq == 1)
		curr = ft_head_empty_token(curr, cmd, *lst_token, del2);
	if (cmd->flag_empty_head_list == 1)
	{
		*lst_token = curr;
		cmd->flag_empty_head_list = 0;
	}
	*alst = curr;
	while (curr != NULL && (curr->position < \
					cmd->simplecmds[del2->index]->end_simplecmd_pos))
		curr = ft_middle_empty_token(curr, cmd, *lst_token, del2);
	ft_free_struct_t_settings_del(&del2);
}

void	ft_check_builtins(t_cmd *cmd, t_list *start_lst_retokenized, \
											t_data_env *data_env, size_t i)
{
	ft_check_export(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_unset(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_env(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_echo(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_cd(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_pwd(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
	ft_check_exit(cmd, start_lst_retokenized, cmd->simplecmds[i], data_env);
}

void	ft_aux_parse_in_scmd(t_cmd *cmd, t_list *lst_token)
{
	ft_get_last_heredoc_position(cmd);
	lst_token = ft_lst_first(lst_token);
	cmd->lst_token_retokenized = ft_lst_first(cmd->lst_token_retokenized);
	cmd->lst_token = lst_token;
}

void	ft_update_start_lst_token(t_list **start_lst_token, \
													t_cmd *cmd, size_t i)
{
	if (*start_lst_token != NULL)
		*start_lst_token = ft_readjust_start_lst_token(*start_lst_token, \
																	cmd, i);
}

int	ft_parse_tokens_in_s_cmd(t_cmd *cmd, t_list *lst_token, \
												t_data_env *data_env)
{
	int		exec_return;
	size_t	i;
	t_list	*start_lst;
	t_list	*start_lstretok;

	exec_return = 0;
	i = 0;
	start_lst = lst_token;
	ft_malloc_heredocs_of_cmd(cmd);
	while (i < cmd->nb_of_simplecmds && start_lst != NULL)
	{
		ft_get_end_simplecmd_pos(cmd, cmd->simplecmds[i], &start_lst);
		ft_nb_of_redir_token_in_scmd(cmd, cmd->simplecmds[i], start_lst, i);
		ft_malloc_redir_file_tabs_of_simplecmd(cmd->simplecmds[i]);
		if (cmd->simplecmds[i]->nb_of_redir_token > 0)
			ft_del_parse_redir_token_in_scmd(&start_lst, i, &lst_token, cmd);
		ft_del_empty_token_in_simplecmd(&start_lst, i, &lst_token, cmd);
		start_lstretok = retok_dequot_1(cmd, start_lst, cmd->simplecmds[i]);
		ft_check_builtins(cmd, start_lstretok, data_env, i);
		parse_cmdarg_scmd_frm_retoknzd_lst(start_lstretok, cmd->simplecmds[i]);
		ft_update_start_lst_token(&start_lst, cmd, i);
		i++;
	}
	ft_aux_parse_in_scmd(cmd, lst_token);
	return (exec_return);
}
