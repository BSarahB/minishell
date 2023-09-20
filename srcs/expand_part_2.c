/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:15:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/20 18:15:24 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//ici on va s occuper de: si cmd principale  == echo -> DEQUOTE(PAS DE RETOKENIZE) 
// si cmd principale  == none ->RETOKENIZE + dequote