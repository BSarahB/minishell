/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 22:57:50 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 22:57:57 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_free_buffer(char **buffer)
{
	ft_free_struct_str(buffer);
	return (0);
}

void	ft_is_expand_to_subst_ambiguous(t_list *lst_token,	char **buffer, \
char **trimmed_buffer)
{
	if (lst_token->tag_ambigeous == 1 || **buffer == '\0')
	{
		if (lst_token->prev != NULL)
		{
			if (lst_token->prev->title == redir_in || \
lst_token->prev->title == redir_out || \
lst_token->prev->title == redir_append)
			{
				lst_token->tag_ambigeous = 1;
				if (lst_token->prev->title == redir_out || \
lst_token->prev->title == redir_append)
					lst_token->prev->title = redir_in;
			}
		}
	}
	else
	{
		*buffer = ft_epur_buffer_ws(*buffer);
		*trimmed_buffer = ft_strtrim(*buffer, " ");
	}
}

void	ft_update_token_content(t_list *lst_token,	char **trimmed_buffer)
{
	ft_update_string(&lst_token->content, *trimmed_buffer);
	if (lst_token->content == NULL)
	{
		lst_token->tag_empty_cmd_before_dq = 1;
		ft_update_string(&lst_token->content, ft_strdup("empty"));
	}
	else if (ft_strcmp(lst_token->content, "\"\"") == 0)
	{
		if (lst_token->prev != NULL)
		{
			if (lst_token->prev->title == redir_out || \
lst_token->prev->title == redir_append)
				lst_token->prev->title = redir_in;
		}
		else
			lst_token->tag_empty_cmd_after_dq = 1;
	}
}

int	ft_is_expand_to_substitute(t_list *lst_token, char *envp[])
{
	char	*str;
	char	*buffer;
	char	*trimmed_buffer;

	buffer = ft_init_string(1096);
	str = lst_token->content;
	trimmed_buffer = NULL;
	if (str == NULL)
		return (0);
	if (ft_strcmp(lst_token->content, "$") == 0)
		return (ft_free_buffer(&buffer));
	if (lst_token->prev)
		if (lst_token->prev->title == redir_heredoc)
			return (ft_free_buffer(&buffer));
	if (ft_is_expand_here(lst_token, str, buffer, envp) == 1)
	{
		lst_token->expand_exists = 1;
		ft_is_expand_to_subst_ambiguous(lst_token, &buffer, &trimmed_buffer);
		if (lst_token->tag_ambigeous != 1)
			ft_update_token_content(lst_token, &trimmed_buffer);
	}
	free(buffer);
	buffer = NULL;
	return (0);
}

void	ft_expand_and_retokenize(t_list *lst_token, char *envp[])
{
	t_list	*tmp;

	tmp = lst_token;
	if (tmp == NULL)
		return ;
	while (lst_token)
	{
		ft_is_expand_to_substitute(lst_token, envp);
		lst_token = lst_token->next;
	}
	lst_token = tmp;
	return ;
}
