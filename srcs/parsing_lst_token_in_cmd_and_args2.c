/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_token_in_cmd_and_args2.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:51:33 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:51:49 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_get_abs_argumentsb(char **abs_c_and_a)
{
	char	slash[2];

	slash[0] = '/';
	slash[1] = 0;
	ft_update_string(&abs_c_and_a[0], ft_strjoin(slash, abs_c_and_a[0]));
	return (abs_c_and_a);
}
