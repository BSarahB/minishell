/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_and_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:37:53 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 12:38:12 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_parse_redir_err(t_list *lst, t_simpleCmd *simpleCmd, size_t i)
{
    simpleCmd->errfile[i] = ft_strdup(lst->content);
	if(i == simpleCmd->nb_of_errfile -1)
	    simpleCmd->errfile[i + 1] = NULL;
}

void    ft_parse_redir_heredoc(t_list *lst, t_simpleCmd *simpleCmd, t_cmd *cmd)
{
	simpleCmd->heredoc[simpleCmd->k] = ft_strdup(lst->content);
	if(simpleCmd->k == simpleCmd->nb_of_heredoc -1)
		simpleCmd->heredoc[simpleCmd->k + 1] = NULL;
	simpleCmd->k = simpleCmd->k + 1;
	cmd->heredocs[cmd->k] =ft_strdup(lst->content); //ft_strdup(content);
    if(cmd->k == cmd->nb_of_heredocs -1)
		cmd->heredocs[cmd->k + 1] = NULL;
	cmd->k = cmd->k +1; //ICI l index sera +1 a la sortie .... si on voulait s en servir pour recuperer last .... il faut mettre le else
}

void    ft_parse_redir_in(t_list *lst, t_simpleCmd *simpleCmd, size_t i)
{
    simpleCmd->infile[i] = ft_strdup(lst->content);
    if(i == simpleCmd->nb_of_infile -1)
		simpleCmd->infile[i + 1] = NULL;
	if(lst->title == redir_heredoc)
		simpleCmd->heredoc_track_index[i] = 1;
	if(lst->tag_expand == 1)
		simpleCmd->heredoc_track_index[i] = 2;		
}

void    ft_parse_redir_out(t_list *lst, t_simpleCmd *simpleCmd, size_t i)
{
    simpleCmd->outfile[i] = ft_strdup(lst->content);
	if(i == simpleCmd->nb_of_outfile -1)
		simpleCmd->outfile[i + 1] = NULL;
	if(lst->title == redir_append)
		simpleCmd->append_track_index[i] = 1;
}

void	ft_parse(t_list *lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd)
{
	if(lst->title == redir_out || lst->title == redir_append)
        ft_parse_redir_out(lst, simpleCmd, i);
	if(lst->title == redir_in || lst->title == redir_heredoc)
        ft_parse_redir_in(lst, simpleCmd, i);
	if(lst->title == redir_heredoc)
        ft_parse_redir_heredoc(lst, simpleCmd, cmd);
	if(lst->title == redir_err)
        ft_parse_redir_err(lst, simpleCmd, i);
}

void	ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd)
{
	 if(*lst)
	 {
		
		ft_parse(*lst, simpleCmd, i, cmd);
		free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
	 }
}

void	ft_lstdelone(t_list **lst)
{
	 if(*lst)
	 {
		free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
	 }
}