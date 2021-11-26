/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akihito <akihito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 20:00:32 by akihito           #+#    #+#             */
/*   Updated: 2021/11/25 19:54:40 by akihito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H
# define BUFFSIZE 1024

# ifndef TURE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

#include <stdlib.h>
typedef struct	s_stack
{
	char	message[BUFFSIZE];//
	int		top_bit;
	int		top_byte;
	_Bool	all_receive;
	_Bool	buff_overflow;
}		t_stack;

t_stack		g_to_print = {{0}, 1 << 6, 0, FALSE, FALSE};
//構造体t_stackを初期化する。
void	error(char *str_error);

#endif