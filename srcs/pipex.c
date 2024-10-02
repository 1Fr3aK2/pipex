/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:38:51 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/02 15:13:32 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exec(char *argv, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	path = find_path(cmd[0], env);
	if (execve(path, cmd, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		ft_free(cmd);
		exit(6);
	}
}

void	child_process(char *argv[], int *fd, char **env)
{
	int		infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
	{
		ft_printf("Error opening the file\n");
		exit(4);
	}
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	exec(argv[2], env);
}

void	parent_process(char *argv[], int *fd, char **env)
{
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		ft_printf("Error opening the file\n");
		exit(5);
	}
	dup2(outfile, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	exec(argv[3], env);
}

int	main(int argc, char *argv[], char **env)
{
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		handle_errors(1);
	if (pipe(fd) == -1)
	{
		ft_printf("Error creating the pipe\n");
		exit(2);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_printf("Error with fork\n");
		exit(3);
	}
	if (pid == 0)
		child_process(argv, fd, env);
	waitpid(pid, NULL, 0);
	parent_process(argv, fd, env);
}
