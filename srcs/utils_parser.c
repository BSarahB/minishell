/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:03:08 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:03:31 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(char *content, t_simpleCmd *simpleCmd, size_t i, int title,int tag_expand, t_cmd *cmd)
{
	if(title == redir_out || title == redir_append)
	{
		simpleCmd->outfile[i] = ft_strdup(content);
	
		if(i == simpleCmd->nb_of_outfile -1)
			simpleCmd->outfile[i + 1] = NULL;
		if(title == redir_append)
			simpleCmd->append_track_index[i] = 1;
		

	} 
	if(title == redir_in || title == redir_heredoc)
	{
		simpleCmd->infile[i] = ft_strdup(content);
		if(i == simpleCmd->nb_of_infile -1)
			simpleCmd->infile[i + 1] = NULL;
		if(title == redir_heredoc)
			simpleCmd->heredoc_track_index[i] = 1;
		if(tag_expand == 1)
			simpleCmd->heredoc_track_index[i] = 2;

		
	}
	if(title == redir_heredoc)
	{
		simpleCmd->heredoc[simpleCmd->k] = ft_strdup(content);
		
		if(simpleCmd->k == simpleCmd->nb_of_heredoc -1)
			simpleCmd->heredoc[simpleCmd->k + 1] = NULL;
		simpleCmd->k = simpleCmd->k + 1;

		cmd->heredocs[cmd->k] =ft_strdup(content); //ft_strdup(content);
		if(cmd->k == cmd->nb_of_heredocs -1)
			cmd->heredocs[cmd->k + 1] = NULL;
		cmd->k = cmd->k +1; //ICI l index sera +1 a la sortie .... si on voulait s en servir pour recuperer last .... il faut mettre le else

	}
	if(title == redir_err)
	{
		simpleCmd->errfile[i] = ft_strdup(content);
		if(i == simpleCmd->nb_of_errfile -1)
			simpleCmd->errfile[i + 1] = NULL;
	}
}

void	ft_lstdelone(t_list **lst, void(*parse)(char *content, t_simpleCmd *simpleCmd, size_t i, int title, int tag_expand, t_cmd *cmd), t_simpleCmd *simpleCmd, size_t i, int redir, t_cmd *cmd)
{
	 if(*lst && parse)
	 {
		if(redir == 1)
			{
				parse((*lst)->content, simpleCmd, i, (*lst)->title, (*lst)->tag_expand, cmd);

			}

		//free ts les mallocs ici de str etc...
		if(redir == 0)
			free((*lst)->content);
		if(redir == 1)
			free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
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

//fcts utiles au deboggage
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
			//printf("cmd_and_args de la simpleCmd[%zu] : <%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;
		}
		//printf("\n\n\n");		
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
			//printf("abs_cmd_and_args[%zu]<%s>\n",i, cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
//	printf("\n\n\n");
		i++;
		}
		else
			break;
	}
}

