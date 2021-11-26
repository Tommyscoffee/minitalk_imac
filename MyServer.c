/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyServer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akihito <akihito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 14:42:28 by akihito           #+#    #+#             */
/*   Updated: 2021/11/26 00:02:15 by akihito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <zconf.h>
#include "./includes/minitalk.h"

void	put_pid()
{
	char	*tmp;

	tmp = ft_itoa(getpid());
	write(1, "pid:", 5);
	write(1,tmp, ft+strlen(tmp));
	write(1,"\n",1);
	free(tmp);
	return ;
}

// https://www.ibm.com/docs/ja/zos/2.2.0?topic=functions-sigaction-examine-change-signal-action#f-2c82c40-830d-4778-8368-c41d0b19cd8c
void	activebit(int sig, siginfo_t *info, void *context)
{
	//使わない変数をvoidにキャストして使っている
	(void)sig;
	(void)context;
	(void)info;
	if (!g_to_print.top_bit)//top_bitが0だったら
	{
		g_to_print.top_bit = 1 << 6;//6bit左に移動
		++(g_to_print.top_byte);//message[]のインデックスとして使われる
	}
	g_to_print.message[g_to_print.top_byte] += g_to_print.top_bit;//messageのインデックス
	g_to_print.top_bit >>= 1;//bitを右に一つずらす。
	if (g_to_print.top_byte == BUFFSIZE - 2 && !g_to_print.top_bit)//0が偽なので、「なかったら」
		g_to_print.buff_overflow = TRUE;
	else if (!g_to_print.message[g_to_print.top_byte]
		&& g_to_print.top_bit)//０が偽 messageがなくて、top_bitがあった場合
	{
		g_to_print.all_receive = TRUE;//
		kill(info->si_pid, SIGUSR1);
	}
}

void	nullbit(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;
	if (!g_to_print.top_bit)
	{
		g_to_print.top_bit = 1 << 6;
		++(g_to_print.top_byte);
	}
	g_to_print.top_bit >>= 1;
	if (g_to_print.top_byte == BUFFSIZE - 2 && !g_to_print.top_bit)
		g_to_print.buff_overflow = TRUE;
	else if (!g_to_print.message[g_to_print.top_byte]
		&& !g_to_print.top_bit)//messageがなくて、top_bitもなかったら
	{
		g_to_print.all_receive = TRUE;
		kill(info->si_pid, SIGUSR1);
	}
}

_Bool	main_handler()
{
	while (1)
	{
		pause();
		if (g_to_print.all_receive || g_to_print.buff_overflow)
		{
			write(1, g_to_print.message, ft_strlen(g_to_print.message));
			ft_bzero(g_to_print.message, BUFFSIZE);
			g_to_print.top_byte = 0;
			g_to_print.top_bit = 1 << 6;
			if (g_to_print.all_receive)
				write(1, "\n", 1);
			g_to_print.all_receive = FALSE;
			g_to_print.buff_overflow = FALSE;
		}
	}
	return (TRUE);
}

int	main()
{
// 	struct  sigaction {
// 	union __sigaction_u __sigaction_u;  /* signal handler */
// 	sigset_t sa_mask;               /* signal mask to apply */
// 	int     sa_flags;               /* see signal options below */
// };
	struct sigaction	active_act;
	struct sigaction	null_act;

	active_act.sa_sigaction = activebit;//sa_sigactionはsigaction構造体のメンバ"__sigaction_u __sication_u"のマクロ
	//sa_siginfoのフラグが立っってたらsa_sigactionが起動する。
	//起動時には、指定された”シグナルハンドラー関数が次のように起動される
	//   void  function(int signo, siginfo_t *info, void *context);
	//https://www.ibm.com/docs/ja/zos/2.2.0?topic=functions-sigaction-examine-change-signal-action
	//
	null_act.sa_sigaction = nullbit;
	active_act.sa_flags = SA_SIGINFO;
	null_act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &active_act, NULL) != 0)//sigactionの第2引数がnullだったら、sigaction()はsigを処理するために現在定義されいるアクションの判定だけする。
	//第さん引数　struct sigaction *oldはsigaction()がsigaction構造体を補完できるメモリ位置を示す。このメモリを使って偏在sigと関連している各ションが記述されているsigaction構造体を保管する。
		error("signal error\n");
	if (sigaction(SIGUSR2, &null_act, NULL) != 0)
		error("signal error\n");
	put_pid();
	ft_bzero(g_to_print.message, BUFFSIZE);
	main_handler();

	
}

//構造体sigactionの中身
// struct sigaction {
//     /* シグナルハンドラへの関数ポインタ */
//     void     (*sa_handler)(int);
//     void     (*sa_sigaction)(int, siginfo_t *, void *);
//     /* シグナルハンドラ実行中にブロックするシグナルのマスク */
//     sigset_t   sa_mask;
//     /* シグナルハンドラの動作を修正するためのフラグ */
//     int        sa_flags;
//     /* 現在では廃止扱いであり、使用禁止です。 */
//     void     (*sa_restorer)(void);
// };