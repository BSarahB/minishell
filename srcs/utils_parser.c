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

void	ft_lstdelone(t_list **lst, void(*parse)(char *content, t_simpleCmd *simpleCmd, size_t i, int title), t_simpleCmd *simpleCmd, size_t i, int redir)
{
	 if(*lst && parse)
	 {
		if(redir == 1)
			parse((*lst)->content, simpleCmd, i, (*lst)->title);
		//free ts les mallocs ici de str etc...
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

