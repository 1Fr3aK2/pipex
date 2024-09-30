int main(int argc, char *argv[])
{
	int fd[2];
	if(pipe(fd) == -1)
	{
		printf("An error ocurred with opening the pipe\n");
		return (1);
	}
	if(argc < 5)
	{
		printf("Error in the number of arguments");
		return (2);
	}
	else
		
}