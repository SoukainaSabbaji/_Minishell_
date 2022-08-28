# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <errno.h>
#include <string.h>
#include "minishell.h"

char	*ft_substr(char *s, int start, size_t len)
{
	char			*ptr;
	unsigned int	i;
	int				end;

	i = 0;
	end = len + start;
	if (!s)
		return (NULL);
	ptr = (char *)malloc((len + 1) * sizeof(char));
	if (!ptr)
		exit (1);
	if (start <= strlen(s))
	{
		while (start < end && s[start] != '\0')
		{
			ptr[i] = s[start];
			i++;
			start++;
		}
	}
	ptr[i] = '\0';
	return (ptr);
}
int main()
{
    char pwd[]= "alo ";
    printf("%s", ft_substr(pwd, 0, strlen(pwd) - 1));
}
// int main ()
// {
//     int pipes[2];
//     int pid;
//     int fd;
//     char    **cmd1;
//     char    **cmd2;

//     cmd1 = malloc (sizeof (char *) * 3);
//     cmd2 = malloc (sizeof (char *) * 3);
//     cmd1[0] = "/bin/cat";
//     cmd1[1] = "txt";
//     cmd1[2] = NULL;
    
//     cmd2[0] = "/usr/bin/grep";
//     cmd2[1] = "main";
//     cmd2[2] = NULL;
//     pipe(pipes);
//     fd = open ("txt", O_CREAT | O_RDWR);
//     if (fd == -1)
//         printf("mok ana\n");
//     pid = fork ();
//     if (pid == -1)
//         return (0);
//     if (pid == 0)
//     {
//         close(pipes[0]);
//         dup2(pipes[1], 1);
//         close(pipes[1]);
//         if(execve(cmd1[0], cmd1, NULL) == -1)
//             return (printf("error\n"));
//     }
//     wait (NULL);
//      pid = fork ();
//     if (pid == -1)
//         return (0);
//     if (pid == 0)
//     {
//         close(pipes[1]);
//         dup2(pipes[0], 0);
//         close(pipes[0]);
//         if(execve(cmd2[0], cmd2, NULL) == -1)
//             return (printf("error\n"));
//     }
//     close(pipes[1]);
//     close(pipes[0]);
//     wait (NULL);

// }