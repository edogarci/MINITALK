/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edogarci <edogarci@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:34:53 by edogarci          #+#    #+#             */
/*   Updated: 2023/12/15 12:32:20 by edogarci         ###   ########.fr       */
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
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
			i++;
		else
		{
			write(STDOUT_FILENO, "PID incorrecto.\n", 16);
			exit(1);
		}
	}
	dec = 1;
	pid = 0;
	while (str[i - 1] >= '0' && str[i - 1] <= '9')
	{
		pid = pid + ((str[i - 1] - 48) * dec);
		dec = dec * 10;
		i--;
	}
	return (pid);
}

/*
pass char to binary
*/
void	f_pass_to_binary(char *c_bits, char c)
{
	int	cont;

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

	c_bits = NULL;
	c_bits = malloc(8 * sizeof(char));
	if (c_bits == NULL)
	{
		write(STDOUT_FILENO, "Error reservando memoria.\n", 26);
		exit(1);
	}
	f_pass_to_binary(c_bits, c);
	cont = 7;
	while (cont >= 0)
	{
		if (c_bits[cont] == '0')
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		cont--;
		usleep(100);
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

	if (argc != 3)
	{
		write(STDOUT_FILENO, "Error. Revisa argumentos.\n", 26);
		exit(1);
	}
	pid = f_atoi(argv[1]);
	if (pid == 0)
	{
		write(STDOUT_FILENO, "PID no puede ser 0\n", 19);
		exit(1);
	}
	f_send_string(pid, argv[2]);
	f_send_string(pid, "\n");
	return (0);
}
