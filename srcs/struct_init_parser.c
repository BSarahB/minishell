/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:46:30 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:46:38 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *ft_memset(void *b, char c, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
		((char *)b)[i++] = c;
	return (b);
}

char *ft_init_cstring(char **str, size_t len, char init_value)
{
	*str = malloc(sizeof(char) * (len + 1));
	if (!(*str))
		return (NULL);
	if (len > 0)
		ft_memset(*str, init_value, len);
	(*str)[len] = '\0';
	return (*str);
}

t_simpleCmd *ft_struct_init_simpleCmd(t_simpleCmd **simpleCmd)
{
	*simpleCmd = (t_simpleCmd *)malloc(sizeof(t_simpleCmd));
	if (!(*simpleCmd))
		return (NULL);
	(*simpleCmd)->number_of_arguments = 0;
	(*simpleCmd)->errnum = 0;
	(*simpleCmd)->cmd_and_args = NULL;
	(*simpleCmd)->abs_cmd_and_args = NULL;
	(*simpleCmd)->heredoc = NULL;
	(*simpleCmd)->infile = NULL;
	(*simpleCmd)->outfile = NULL;
	(*simpleCmd)->errfile = NULL;
	(*simpleCmd)->append_track_index = NULL;
	(*simpleCmd)->heredoc_track_index = NULL;
	(*simpleCmd)->end_simpleCmd_pos = 0;
	(*simpleCmd)->nb_of_tokens_in_simpleCmd = 0;
	(*simpleCmd)->flag_empty_simpleCmd = 0;
	(*simpleCmd)->nb_of_redir_token = 0;
	(*simpleCmd)->nb_of_heredoc = 0;
	(*simpleCmd)->k = 0;
	(*simpleCmd)->nb_of_infile = 0;
	(*simpleCmd)->nb_of_outfile = 0;
	(*simpleCmd)->nb_of_errfile = 0;
	(*simpleCmd)->first_redir = 0;
	(*simpleCmd)->nb_of_outfile_before_nofile = 0;
	(*simpleCmd)->nb_of_errfile_before_nofile = 0;
	(*simpleCmd)->nofile = 0;
	return (*simpleCmd);
}

size_t	ft_count_simpleCmds_nbr(t_list *lst_token)
{
		size_t	k;
		size_t	simpleCmd_nbr;
		t_list	*tmp;
		t_list	*new;
		(void)new;

		simpleCmd_nbr = 0;
		ft_simplify_list(lst_token);
		tmp = lst_token;
	//	ft_aff_list_ptr_sur_char_content(lst_token);
		while (tmp)
		{
			while (tmp && tmp->title == operator)
				tmp = tmp->next;
			k = 0;
			while (tmp && tmp->title != operator)
			{
				if(tmp->content != NULL)
				{
					k++;
					tmp = tmp->next;
				}
				else{
					break;
				}
			}
			if (k != 0)
				simpleCmd_nbr++;
			if(tmp)
				tmp = tmp->next;
		}
		return (simpleCmd_nbr);
}

t_cmd	*ft_struct_init_cmd(t_cmd **cmd,t_list *lst_token)
{
	size_t 		k;
	size_t 		nbr_of_simpleCmds;
	t_simpleCmd	*simpleCmd;
	int			*heredocs_track_index;

	k = 0;
	heredocs_track_index = NULL;
	//TODO METTRE A NULL ICI LES VARIABLES AVANT DE LES MALLOC
	nbr_of_simpleCmds = ft_count_simpleCmds_nbr(lst_token);
	//on peut proteger ici si nbr == 0 et ne pas rentrer la dedans (ex : |)
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!(*cmd))
		return (NULL);
	(*cmd)->simpleCmds = malloc(sizeof(t_simpleCmd *) * (nbr_of_simpleCmds + 1));
	if (!((*cmd)->simpleCmds))
		return (NULL);
	while(k < nbr_of_simpleCmds)
	{
		(*cmd)->simpleCmds[k] = ft_struct_init_simpleCmd(&simpleCmd);
		k++;
	}
	(*cmd)->simpleCmds[k] = 0;
	(*cmd)->heredocs_track_index = ft_init_ctab(&heredocs_track_index, nbr_of_simpleCmds, 0);
	if (!((*cmd)->heredocs_track_index))
		return (NULL);
	(*cmd)->nb_of_heredocs = 0;
	(*cmd)->k = 0;
	(*cmd)->line_count = 0;
	(*cmd)->nb_of_simpleCmds = nbr_of_simpleCmds;
	(*cmd)->background = 0;
	(*cmd)->path_tab = 0;
	(*cmd)->lst_token = lst_token;
	(*cmd)->lst_token_retokenized = NULL;
	//(*cmd)->lst_envp = NULL;
	(*cmd)->flag_head_list = -1;
	(*cmd)->flag_empty_head_list = -1;
	(*cmd)->heredocs = NULL;
	(*cmd)->outputfile = NULL;
	(*cmd)->inputfile = NULL;
	(*cmd)->errfile = NULL;
	(*cmd)->background = 0;
	return (*cmd);
}
//	(*cmd)->outfile = ft_init_cstring(&((*cmd)->outfile), 0, init_value);
//	(*cmd)->inputfile = ft_init_cstring(&((*cmd)->inputfile), 0, init_value);
//	(*cmd)->errfile = ft_init_cstring(&((*cmd)->errfile), 0, init_value);