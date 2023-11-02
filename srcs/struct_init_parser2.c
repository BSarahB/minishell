/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_parser2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:46:30 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:46:38 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memset(void *b, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		((char *)b)[i++] = c;
	return (b);
}

char	*ft_init_cstring(char **str, size_t len, char init_value)
{
	*str = malloc(sizeof (char) * (len + 1));
	if (!(*str))
		return (NULL);
	if (len > 0)
		ft_memset(*str, init_value, len);
	(*str)[len] = '\0';
	return (*str);
}

void	ft_struct_init_simplecmd_1(t_simpleCmd **simpleCmd)
{
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
	(*simpleCmd)->end_simplecmd_pos = 0;
	(*simpleCmd)->nb_of_tokens_in_simplecmd = 0;
	(*simpleCmd)->flag_empty_simplecmd = 0;
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
	(*simpleCmd)->export_no_option = 0;
	(*simpleCmd)->unset_no_option = 0;
}

void	ft_struct_init_simplecmd_2(t_simpleCmd **simpleCmd)
{
	(*simpleCmd)->echo_no_option = 0;
	(*simpleCmd)->cd_no_option = 0;
	(*simpleCmd)->unset_solo = 0;
	(*simpleCmd)->export_solo = 0;
	(*simpleCmd)->cd_solo = 0;
	(*simpleCmd)->oldpwd = NULL;
	(*simpleCmd)->pwd = NULL;
	(*simpleCmd)->exit_str = NULL;
	(*simpleCmd)->exit_code = 0;
	(*simpleCmd)->exit_no_option = 0;
	(*simpleCmd)->builtin_solo = 0;
	(*simpleCmd)->exit_solo = 0;
	(*simpleCmd)->is_builtin = 0;
	(*simpleCmd)->builtin = -1;
	(*simpleCmd)->out_denied = 0;
}

t_simpleCmd	*ft_struct_init_simplecmd(t_simpleCmd **simpleCmd)
{
	*simpleCmd = (t_simpleCmd *)malloc(sizeof (t_simpleCmd));
	if (!(*simpleCmd))
		return (NULL);
	ft_struct_init_simplecmd_1(simpleCmd);
	ft_struct_init_simplecmd_2(simpleCmd);
	return (*simpleCmd);
}
