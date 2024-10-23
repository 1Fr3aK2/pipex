/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:01:59 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/23 14:02:28 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec(char *argv, char **env)
{
	char	**cmd;
	char	*path;

	if (!argv || !env)
		exit(9);
	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
		exit(10);
	path = find_path(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(cmd);
		exit(127);
	}
	if (execve(path, cmd, env) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(cmd);
		free(path);
		exit(126);
	}
}

void	first_child(char *argv[], int *fd, char **env)
{
	int		infile;

	if (!argv || !fd || !env)
		exit(9);
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

	if (!argv || !fd || !env)
		exit(4);
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
	int		status_first;
	int		status_second;

	if (argc != 5)
		handle_errors(1, NULL);
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
	close_pipe(fd);
	waitpid(first_pid, &status_first, 0);
	waitpid(second_pid, &status_second, 0);
	return (WEXITSTATUS(status_second));
}
