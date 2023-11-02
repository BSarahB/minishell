/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check_var_in2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:45:11 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:45:22 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_keys(char **key_input, char **key)
{
	ft_free_struct_str(key);
	ft_free_struct_str(key_input);
}
