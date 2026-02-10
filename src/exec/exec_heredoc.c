#include "minishell.h"
#include <readline/history.h>

void	execute_heredoc(t_shell *sh)
{
	int		fd[2];
	char	*end;
	char	*line;

	pipe(fd);
	if (sh->ast->args[2])
	{
		end = sh->ast->args[2];
	}

	while (1)
	{
		line = readline("> ");
		if (!line)
        	break;
		if (ft_strncmp(line, end, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);

}
