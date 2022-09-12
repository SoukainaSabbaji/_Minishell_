/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssabbaji <ssabbaji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:53:10 by ssabbaji          #+#    #+#             */
/*   Updated: 2022/09/12 14:55:02 by ssabbaji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define TERM_OWNER 130

int	nofork_list(t_data *data, t_cmd *cmd)
{
	data->fork_flag = 0;
	if (!ft_strcmp(cmd->cmd[0], "cd"))
		data->exit_stat = my_cd(data, cmd);
	else if (!ft_strcmp(cmd->cmd[0], "export") && !cmd->next)
		data->exit_stat = export(data, cmd);
	else if (!ft_strcmp(cmd->cmd[0], "env") || 
	(!ft_strcmp(cmd->cmd[0], "/usr/bin/env") && !cmd->next))
		my_env(data, cmd);
	else if (!ft_strcmp(cmd->cmd[0], "unset") && !cmd->next)
		data->exit_stat = unset(data, cmd);
	else if (!ft_strcmp(cmd->cmd[0], "exit") && !cmd->next)
	{
		data->exit_stat = my_exit(data, cmd);
		if (data->exit_stat != 1)
			exit(data->exit_stat);
	}
	else
		data->fork_flag = 1;
	return (data->exit_stat);
}

int	check_nonfork(t_data *data, t_cmd *cmd)
{	
	data->fork_flag = 0;
	if (heredoc_exec(data, cmd) && cmd->her_doc_num)
	{
		if (cmd->her_in)
		{
			close(cmd->her_in);
			data->fork_flag = 1;	
		}
	}
	if(cmd->cmd[0])
		return (nofork_list(data, cmd));
	return (data->exit_stat);
}

int	check_builtins(t_data *data, t_cmd *cmd_lst)
{
	//echo pwd , unset, export , env , exit
	char **cmd;

	data->non_built_f = 0;
	cmd = cmd_lst->cmd;
	if (!ft_strcmp(cmd[0], "export"))
		data->exit_stat = export(data, cmd_lst);
	else if (!ft_strcmp(cmd[0], "unset"))
		data->exit_stat = unset(data, cmd_lst);
	else if (!ft_strcmp(cmd[0], "echo"))
		my_echo(data, cmd_lst);
	else if (!ft_strcmp(cmd[0], "env"))
		my_env(data, data->lst_cmd);
	else if (!ft_strcmp(cmd[0], "exit"))
	{
		data->exit_stat = my_exit(data, cmd_lst);
		if (data->exit_stat != 1)
			exit(data->exit_stat);
	}
	else
		data->non_built_f = 1;;
	return (data->exit_stat);
}
//if the child process exits normally WIFEXITED evaluates to true
//and the macro is used to query the exit code of the child
//p evaluates to 1 , the pid of the exited child process

int	terminate_pid(int pid, int count)
{
    int status;
    int corpse;
		
	while (count)
	{
		corpse = wait(&status);
        if (corpse < 0)
            printf("Failed to wait for process %d (errno = %d)\n", (int)pid, errno);
        else if (corpse != pid)
            printf("Got corpse of process %d (status 0x%.4X) when expecting PID %d\n",
                   corpse, status, (int)pid);
        else if (WIFEXITED(status))
			kill(pid, SIGKILL);
		count--;
	}
    return WEXITSTATUS(status);
}

void	dup_and_close(t_data *data, t_cmd *cmd)
{
	dup2(cmd->fd_in, 0);
	dup2(cmd->fd_out, 1);
	close_all(cmd , data->pipes, c_lstcmd(data));
	data->exit_stat = check_builtins(data, cmd);
	if(data->non_built_f)
		data->exit_stat = execution_2(data, cmd);
}

int	check_fork(int	*pid, t_data *data)
{
	int i;

	i = 0;
	if (*pid != 0 && data->fork_flag)
	{
		*pid = fork();
		if (*pid < 0)
			perror("fork() error");
		i++;
	}
	return (i);
}

int	execution(t_data *data)
{
	t_cmd	*cmd;
	int		pid;
	int		fork_c;
	
	cmd = data->lst_cmd;
	fork_c = 0;
	int h = 0;
	while (cmd)
	{
		data->exit_stat = check_nonfork(data, cmd);
		fork_c = check_fork(&pid, data);
		if (pid == 0 && !data->rerror_f)
		{
			g_where_ami = 0;
			dup_and_close(data , cmd);
			exit(data->exit_stat);
		}
		cmd = cmd->next;	
	}
	close_all(data->lst_cmd, data->pipes, c_lstcmd(data));
	if (fork_c)
		data->exit_stat = terminate_pid(pid , fork_c);
	return (data->exit_stat);
}

int	pre_execution(t_data *data)
{
	int	pid;
	t_cmd *cmd;

	pid = 0;
	data->paths = ft_split(custom_getenv("PATH", data->lst_env),':');
	if (data->lst_cmd)
	{
		data->pipes = initialize_pipes(data);
		data->exit_stat = execution(data);
	}
	return (0);
}
