/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/10/02 16:27:09 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	handle_errors(int error)
{
	if (error == 1)
		ft_printf("./pipex infile cmd cmd outfile\n", 2);
	exit(1);
}

void	ft_free(char **cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

/* variable name = PATH=
value = return = /home/raamorim/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin */
char	*get_env(char *variable_name, char **env)
{
	int		i;
	int		len;
	char	*value; // tudo o que vem depois do nome da variavel ex : PATH=;
	
	if (!variable_name)
		return (NULL);
	len = ft_strlen(variable_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], variable_name, len) == 0 && env[i][len] == '=')
		{
			value = ft_strchr(env[i], '=');
			if (!value)
				return (NULL);
			else
				return (value + 1);
		}
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**cmd_flags;    // Separação do comando e flags
	char	**paths;        // Diretórios extraídos da variável PATH
	char	*correct_path;  // Caminho do executável (cmd)
	char	*directory;     // Caminho temporário para combinar diretórios
	int		i;

	// Obter os diretórios da variável PATH
	paths = ft_split(get_env("PATH", envp), ':');
	if (!paths) // Se não encontrar a variável PATH, retorne NULL
		return (NULL);
	// Separar o comando e as flags, se houver
	cmd_flags = ft_split(cmd, ' ');
	if (!cmd_flags || !cmd_flags[0]) // Se o comando for inválido, libere a memória e retorne NULL
	{
		ft_free(paths);
		ft_free(cmd_flags);
		return (NULL);
	}
	// Percorrer os diretórios da variável PATH
	i = 0;
	while (paths[i])
	{
		// Combina o diretório com o comando
		directory = ft_strjoin(paths[i], "/");
		correct_path = ft_strjoin(directory, cmd_flags[0]);
		free(directory);
		// Verificar se o caminho é válido e executável
		if (access(correct_path, F_OK | X_OK) == 0)
		{
			ft_free(paths);
			ft_free(cmd_flags);
			return (correct_path);
		}
		free(correct_path);
		i++;
	}
	// Se não encontrar o executável, libere a memória alocada
	ft_free(paths);
	ft_free(cmd_flags);
	return (NULL); // Retorna NULL se nenhum caminho for encontrado
}