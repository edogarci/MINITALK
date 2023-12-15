/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edogarci <edogarci@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:34:50 by edogarci          #+#    #+#             */
/*   Updated: 2023/12/15 12:22:05 by edogarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
get PID lenght
*/
int	f_get_pid_len(int pid)
{
	int	len;

	len = 1;
	while (pid / 10 != 0)
	{
		pid = pid / 10;
		len = len * 10;
	}
	return (len);
}

/*
show PID on screen
*/
void	f_print_pid(int pid)
{
	int		len;
	char	c;

	if (pid == 0)
	{
		write(STDOUT_FILENO, "Error. PID es 0.\n", 17);
		exit(1);
	}
	write(STDOUT_FILENO, "SERVER PID: ", 12);
	len = f_get_pid_len(pid);
	while (pid > 0)
	{
		c = (pid / len) + 48;
		write(STDOUT_FILENO, &c, 1);
		pid = pid % len;
		len = len / 10;
	}
	write(STDOUT_FILENO, "\n", 1);
}

/*
Handler function for SIGUSR1 and SIGUSR2
convents 8bits binary to decimal (char)
and print it
*/
void	f_sig_handler(int sig)
{
	static char	c;
	static int	cont;

	if (cont < 8)
	{
		if (sig == SIGUSR2)
			c = (c * 2) + 1;
		else
			c = c * 2;
		cont++;
	}
	if (cont == 8)
	{
		write(STDOUT_FILENO, &c, 1);
		cont = 0;
		c = 0;
	}
}

/*
- show PID of server side,
- register signal handlers and
- wait for signal forever
*/
int	main(void)
{
	int	pid;

	pid = getpid();
	f_print_pid(pid);
	signal(SIGUSR1, f_sig_handler);
	signal(SIGUSR2, f_sig_handler);
	while (1)
		pause();
	return (0);
}
