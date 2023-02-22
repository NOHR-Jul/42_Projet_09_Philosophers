/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouichou <aouichou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 18:57:35 by aouichou          #+#    #+#             */
/*   Updated: 2023/02/15 20:28:47 by aouichou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <errno.h>

# define READ_FD 			0
# define WRITE_FD 			1
# define PATH				"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin"

# define RED	       		"\033[1;31m"
# define GREEN	       		"\033[1;32m"
# define CYAN	       		"\033[1;36m"
# define BLUE	       		"\033[1;34m"
# define YELLOW	       		"\033[1;33m"
# define BLANK	       		"\033[0m"

enum	e__errors
{
	QUOTE = 1,
	NO_DIR = 2,
	NO_PERM = 3,
	NO_CMD = 6,
	DUP_ERR = 7,
	FORK_ERR = 8,
	PIPE_ERR = 9,
	PIPE_ENDERR = 10,
	MEM = 11,
	IS_DIR = 12,
	NOT_DIR = 13
};

// enum	e_operators ////////////////////////////////////////////////
// {
// 	BEFORE,
// 	AFTER,
// 	AND,
// 	OR,
// }

typedef	struct	s_ext
{
	int			exit_status;
}				t_ext;

typedef struct s_cmd
{
	char		**cmd_array;
	char		*path;
	int			infile;
	int			outfile;
}				t_cmd;

typedef struct s_operators ////////////////////////////////////////////////////////
{
	int			operator;
	int			position;
	char		*line;
}				t_operators;

typedef struct s_main
{
	char		**environ;
	pid_t		pid;
	t_list		*cmds;
}				t_main;


t_ext			*get_ext(void);
t_main			*get_ptr(void);


void			signal_handler(int sig);


void			*ft_error(char *arg, int error, int rno);
void			cd_errors(char **str[2]);


t_main			*init_main(char **argv, char **envp);
t_cmd			*init_cmd(void);

char			*get_prompt(t_main *ptr);


char			*ft_getenv(char *var, char **envp, int z);
char			**ft_setenv(char *var, char *value, char **envp, int n);
int				ft_export(t_main *ptr);
int				ft_unset(t_main *ptr);
int				var_in_envp(char **envp, char *var, int i[2]);


void			*check_line(char *line, t_main *ptr);


char			**split_and_check_quotes(char *line, char *set);


char			*expand_path(char *str, int i, int quotes[2], char *var);
char			*expand_variables(char *str, int i, int quotes[2], t_main *ptr);


int				check_builtin(t_cmd *cmd);
int				builtin(t_main *ptr, t_list *cmd, int *do_exit, int n);
int				ft_echo(t_list	*cmd);
int				ft_pwd(void);
int				ft_cd(t_main *ptr);


int				ft_exit_atoi(char *nb, long *nbr);
int				ft_exit(t_list *cmd, int *do_exit);


void			free_ptr(void);
void			free_lst(void *content);


char			**unquote_array(char **argv);


int				get_heredoc(char *str[2], char *temp[2]);
char			*read_heredoc(char *str[2], int len, char *limit, char *warn);


int				get_fd(int oldfd, char *path, int flags[2]);
void			infile1(t_cmd **node, char **argv, int *i);
void			infile2(t_cmd **node, char **argv, int *i);
void			outfile1(t_cmd **node, char **argv, int *i);
void			outfile2(t_cmd **node, char **argv, int *i);


t_list			*fill_list(char **argv, int i);


char			**cmds_split_trim(char *str, char *set);


void			*execute(t_main *ptr, t_list *cmds);
void			get_cmd(t_main *ptr, t_list *cmds, char **array, char *path);


void			*ft_fork(t_main *ptr, t_list *cmds, int fd[2]);


int				ft_pipex(t_main *ptr, t_list *cmds);

#endif