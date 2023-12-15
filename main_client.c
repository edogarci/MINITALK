/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edogarci <edogarci@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:34:53 by edogarci          #+#    #+#             */
/*   Updated: 2023/12/14 20:55:15 by edogarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/*
convert string to int
*/
int	f_atoi(char *str)
{
	int	i;
	int	pid;
	int	dec;

	i = 0;
	while (str[i] != '\0')
		i++;
	dec = 1;
	pid = 0;
	while (str[i - 1] >= '0' && str[i - 1] <= '9')
	{
		pid = pid + ((str[i - 1] - 48) * dec);
		dec = dec * 10;
		i--;
	}
	if (pid > 0)
		return (pid);
	write(STDOUT_FILENO, "Error\n", 6);
	exit(1);
}

/*
send 8 bits (a char) to given PID
SIGUSR1 = 0
SIGUSR2 = 1
*/
void	f_send_char(int pid, char c)
{
	int		cont;
	char	*c_bits;

	c_bits = malloc(8 * sizeof(char));
	cont = 0;
	while (cont < 8)
	{
		if (c % 2 == 0)
			c_bits[cont] = '0';
		else
			c_bits[cont] = '1';
		c = c / 2;
		cont++;
	}
	cont = 7;
	while (cont >= 0)
	{
		if (c_bits[cont] == '0')
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		cont--;
		usleep(1000);
	}
	free(c_bits);
}

/*
send string, char by char, to given PID
*/
void	f_send_string(int pid, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		f_send_char(pid, str[i]);
		i++;
	}
}

/*
expected two parameters when called:
- PID (int)
- String to send to server
*/
int	main(int argc, char *argv[])
{
	int	pid;

	pid = f_atoi(argv[1]);
	f_send_string(pid, argv[2]);
	f_send_string(pid, "\n");
	return (0);
}
