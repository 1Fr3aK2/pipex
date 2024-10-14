#include "../includes/pipex.h"

void	exec(char *argv, char **env)
{
	char	**cmd;
	char	*path;

	if (!argv || !env)
		exit(4);	
	cmd = ft_split(argv, ' ');
	if(!cmd)
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



void child_process(char *argv, char **env)
{
    int fd[2];
    pid_t pid;

    if(!env || (!(*env)))
        exit(3);
    if (pipe(fd) == -1)
	{
		ft_putstr_fd("Error creating the pipe\n", 2);
		exit(4);
	}
    pid = fork();
    if(pid < 0)
    {
        ft_putstr_fd("Error with fork\n", 2);
        exit(5);
    }
    if(pid == 0)
    {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            ft_putstr_fd("Error duplicating file descriptor\n", 2);
            exit(6);
        }
        close(fd[1]);
        exec(argv, env);
    }
    else
    {
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            ft_putstr_fd("Error duplicating file descriptor\n", 2);
            exit(7);
        }
        close(fd[0]);
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[], char **env)
{
    int infile;
    int outfile;
    int i;

    if(argc < 5)
        handle_errors(2);
    else
    {
        i = 2;
        infile = open(argv[1], O_RDONLY, 0777);
        outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if(dup2(infile, STDIN_FILENO) == -1)
        {
            ft_putstr_fd("Error duplicating file descriptor\n", 2);
            exit(2);
        }
        close(infile);
        while(i < argc - 2)
            child_process(argv[i++], env);        
        if (dup2(outfile, STDOUT_FILENO) == -1)
        {
            ft_putstr_fd("Error duplicating file descriptor\n", 2);
            exit(3);
        }
        close(outfile);
        exec(argv[argc - 2], env);
    }
}
