
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **argv, char **envp)
{
	(void)envp;
	char	*line;
	if (argc != 1 || argv[1])
		return (EXIT_FAILURE);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		
	}
}