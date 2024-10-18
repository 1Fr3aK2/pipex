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
		exit(11);
	}
	if (execve(path, cmd, env) == -1)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(cmd);
		free(path);
		exit(12);
	}
}

pid_t	childo_process(char *argv, char **env, int *infile)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		handle_errors(3, NULL);
	pid = fork();
	if (pid == -1)
		handle_errors(4, NULL);
	if (pid == 0)
	{
		close(fd[0]);
		if (dup2(*infile, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1)
			handle_errors_plus(5);
		close(*infile);
		close(fd[1]);
		exec(argv, env);
	}
	close(fd[1]);
	*infile = fd[0];
	return (pid);
}

pid_t	last_child(char *argv, char **env, int infile, char *outfile_path)
{
	int		outfile;
	pid_t	pid2;

	outfile = open(outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		handle_errors_plus(6);
	pid2 = fork();
	if (pid2 == -1)
		handle_errors(4, NULL);
	if (pid2 == 0)
	{
		if (dup2(infile, STDIN_FILENO) == -1 || dup2(outfile, STDOUT_FILENO) == -1)
			handle_errors_plus(5);
		close(infile);
		close(outfile);
		exec(argv, env);
	}
	close(infile);
	close(outfile);
	return (pid2);
}

int	main(int argc, char *argv[], char **env)
{
	int		infile;
	int		i;
    int     j;
	pid_t	pid;
	pid_t	pid2;

	if (argc < 5)
		handle_errors_plus(8);
	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
		handle_errors(2, argv);
	i = 1;
	while (++i < argc - 2)
		pid = childo_process(argv[i], env, &infile);
	pid2 = last_child(argv[i], env, infile, argv[argc - 1]);
    while (j < argc - 2)
        waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
