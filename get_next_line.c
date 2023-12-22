/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaquet <marvin@42quebec.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 10:12:52 by ppaquet           #+#    #+#             */
/*   Updated: 2023/04/01 10:12:53 by ppaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "get_next_line_utils.c" //
#include <stdio.h> //
#include <fcntl.h> //
#include <string.h> //

/* Takes a double pointer to string as parameter, free the memory block, 
modify it's value to NULL, and finally return NULL. */
char	*freestr(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

/* Takes the static stash (sta) character string, reads the file descriptor
(fd) in section(s) defined by BUFFER_SIZE. The function joins the temporary
buffer(s) (buf) string to end of 'sta', then verify if '\n' is in the new stash.
If so, the function returns the new stash including the "next line". */
char	*fill_stash(int fd, char *sta)
{
	ssize_t		byte_read;
	char		*buf;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(*buf));
	if (!buf)
		return (freestr(&sta));
	byte_read = 1;
	while (byte_read > 0)
	{
		byte_read = read(fd, buf, BUFFER_SIZE);
		if (byte_read < 0)
			break ;
		buf[byte_read] = '\0';
		sta = ft_gnl_strjoin(sta, buf);
		if (ft_gnl_strchr(sta, 10))
			break ;
	}
	buf = freestr(&buf);
	if (!sta || byte_read < 0 || (*sta == '\0' && byte_read <= 0))
		return (freestr(&sta));
	return (sta);
}

/* Updates a static char string (stash), so it can return the next_line.
When this function is recalled, it continues to update 'stash' so the following
next_line can be returned, retaining where it stopped reading the file
descriptor (fd), also the previously read characters stored in 'stash'.*/
char	*get_next_line(int fd)
{
	static char	*stash = NULL;
	char		*give_line;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
	{
		stash = malloc(sizeof(char));
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	stash = fill_stash(fd, stash);
	if (!stash)
		return (NULL);
	give_line = NULL;
	give_line = content(stash);
	if (!give_line)
		return (freestr(&stash));
	stash = clean_stash(stash);
	return (give_line);
}
int	main(int argc, char **argv)
{
	int		gnl_fd;
	char	*gnl_ptr;
	int		i;

	gnl_fd = 0;
	gnl_ptr = NULL;
	if (argc < 2)
	{
		printf("\nEntrer des chaines de charactères en argument.\n\n");
		return (-1);
	}
	if (argc == 2)
	{
		gnl_fd = open("gnl_test.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		if (gnl_fd == -1)
		{
			printf("open() error.");
			return (-1);
		}
		else
			write(gnl_fd, argv[1], strlen(argv[1]));
		gnl_fd = close(gnl_fd);
	}
	else
	{
		printf(">>> Entrer une seule chaine de charactère");
		return (-1);
	}
	i = 0;
	gnl_fd = open("gnl_test.txt", O_RDONLY);
	if (gnl_fd < 0)
		return (-1);
	printf("\n===============|| GNL RESULT ||===============\n");
	while (i != -1)
	{
		gnl_ptr = get_next_line(gnl_fd);
		printf("%s", gnl_ptr);
		if (!gnl_ptr)
			i = -1;
		free(gnl_ptr);
		gnl_ptr = NULL;
	}
	printf("\n===============||   THE END  ||===============\n\n");
	gnl_fd = close(gnl_fd);
	return (0);
}
