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
void	child_process(char *argv, char **env, int infile, int write_fd)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		handle_errors(4, NULL);
	if (pid == 0)
	{
		if (dup2(infile, STDIN_FILENO) == -1 || dup2(write_fd, STDOUT_FILENO) == -1)
			handle_errors_plus(5);
		close(infile);
		close(write_fd);
		exec(argv, env);
	}
	else
	{
		close(write_fd);
		if (waitpid(pid, NULL, 0) == -1)
			handle_errors_plus(7);
	}
}


void	last_child(char *argv, char **env, int read_fd, int outfile)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		handle_errors(4, NULL);
	if (pid == 0)
	{
		if (dup2(read_fd, STDIN_FILENO) == -1 || dup2(outfile, STDOUT_FILENO) == -1)
			handle_errors_plus(5);
		close(read_fd);
		close(outfile);
		exec(argv, env);
	}
	else
	{
		close(read_fd);
		close(outfile);
		if (waitpid(pid, NULL, 0) == -1)
			handle_errors_plus(7);
	}
}

int	main(int argc, char *argv[], char **env)
{
	int		i;
	int		infile;  // Usado para o redirecionamento de entrada
	int		outfile;
	int		fd[2];

	if (argc < 5)
		handle_errors(1, argv);
	infile = open(argv[1], O_RDONLY, 0777);  // Abrir arquivo de entrada
	if (infile == -1)
		handle_errors(2, argv);
	i = 1;
	while (++i < argc - 2)
	{
		if (pipe(fd) == -1)
			handle_errors(3, argv);
		child_process(argv[i], env, infile, fd[1]);
		close(fd[1]);
		infile = fd[0];
	}
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		handle_errors_plus(6);
	last_child(argv[i], env, infile, outfile);
	close(infile);
	close(outfile);
}
