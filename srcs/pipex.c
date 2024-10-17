/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:20:57 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/14 14:01:19 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec(char *argv, char **env)
{
	char	**cmd;
	char	*path;

	if (!argv || !env)
		exit(4);
	cmd = ft_split(argv, ' ');
	if (!cmd)
		exit(5);
	path = find_path(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(cmd);
		exit(6);
	}
	if (execve(path, cmd, env) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(cmd);
		free(path);
		exit(7);
	}
}

void	first_child(char *argv[], int *fd, char **env)
{
	int		infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
		handle_errors(2, argv);
	if (dup2(infile, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1)
		handle_errors_plus(5);
	close(infile);
	close(fd[0]);
	close(fd[1]);
	exec(argv[2], env);
}

void	second_child(char *argv[], int *fd, char **env)
{
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		handle_errors_plus(6);
	if (dup2(fd[0], STDIN_FILENO) == -1 || dup2(outfile, STDOUT_FILENO) == -1)
		handle_errors_plus(5);
	close(outfile);
	close(fd[0]);
	close(fd[1]);
	exec(argv[3], env);
}

int	main(int argc, char *argv[], char **env)
{
	int		fd[2];
	pid_t	first_pid;
	pid_t	second_pid;

	if (argc != 5)
		handle_errors(1, argv);
	if (pipe(fd) == -1)
		handle_errors(3, NULL);
	first_pid = fork();
	if (first_pid == -1)
		handle_errors(4, NULL);
	if (first_pid == 0)
		first_child(argv, fd, env);
	second_pid = fork();
	if (second_pid == -1)
		handle_errors(4, NULL);
	if (second_pid == 0)
		second_child(argv, fd, env);
	close(fd[1]);
	close(fd[0]);
	if (waitpid(first_pid, NULL, 0) == -1)
		handle_errors_plus(7);	
	if (waitpid(second_pid, NULL, 0) == -1)
		handle_errors_plus(7);
	return (0);
}
