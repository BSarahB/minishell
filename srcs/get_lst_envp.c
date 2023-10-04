/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lst_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:17:20 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/04 09:17:40 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_listenvp *ft_get_lst_envp2(char **envp)
{
    t_listenvp *lst_envp;
    t_listenvp *new;
    int         i;

    lst_envp = NULL;
    i = 0;
    new = NULL;
    if(envp)
    {
        while(envp[i])
        {
            new = ft_lstnew_for_lst_envp(envp[i]);
            ft_lstadd_back_envp(&lst_envp, new);
            i++;
        }
    }
 //   ft_aff_listenv_ptr_sur_char_content(lst_envp);
    return(lst_envp);
}