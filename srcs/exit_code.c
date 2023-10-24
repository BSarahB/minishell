/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:54:50 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/20 11:55:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//on va ici venir modifier  la variable ? dans lst envp
t_listenvp  *ft_get_exit_status(t_listenvp **alst, char *key, int exit_status)
{ 
    char    *new_keyval;
    char    *exit_status_str;
    t_listenvp *tmp;

    tmp = *alst;
    new_keyval = NULL;
    exit_status_str = NULL;
    while (*alst != NULL)
    {
        if (ft_strncmp((*alst)->key_value, key, 2) == 0)
        {
            exit_status_str = ft_itoa(exit_status);
            new_keyval = ft_strjoin("?=", exit_status_str);
            new_keyval = ft_update_string(&(*alst)->key_value, new_keyval);// new_keyval = ft_update_string(&(*alst)>key_value, new_key_val);
            (*alst)->key_value = new_keyval; //on envoie l adresse &next de 4 de l element qui nous interesse
            ft_free_struct_str(&exit_status_str);
            return(tmp);
        }        
        alst = &(*alst)->next;
    }
    return(tmp);
}