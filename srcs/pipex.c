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
	{
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(9);
	}
	if (dup2(infile, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("Error duplicating file descriptor\n", 2);
		exit(10);
	}
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
	{
		ft_putstr_fd("Error opening/creating the file\n", 2);
		exit(12);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1 || dup2(outfile, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("Error duplicating file descriptor\n", 2);
		exit(13);
	}
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
		handle_errors(2);
	if (pipe(fd) == -1)
	{
		ft_putstr_fd("Error creating the pipe\n", 2);
		exit(2);
	}
	first_pid = fork();
	if (first_pid == -1)
	{
		ft_putstr_fd("Error with fork\n", 2);
		exit(3);
	}
	if (first_pid == 0)
		first_child(argv, fd, env);
	second_pid = fork();
	if (second_pid == -1)
	{
		ft_putstr_fd("Error with fork\n", 2);
		exit(3);
	}
	if (second_pid == 0)
		second_child(argv, fd, env);
	close(fd[1]);
	waitpid(first_pid, NULL, 0);
	waitpid(second_pid, NULL, 0);
	return (0);
}
