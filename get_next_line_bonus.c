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

#include "get_next_line_bonus.h"

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

/* Updates a static tab of char str (stash), so it can return the next_lines.
When this function is recalled, it continues to update 'stash' so the following
next_lines can be returned, retaining where it stopped reading the files
descriptor (xfd), also the previously read characters stored in 'stash'.*/
char	*get_next_line(int fd)
{
	static char	*stash[OPEN_MAX];
	char		*give_line;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash[fd])
	{
		stash[fd] = malloc(sizeof(char));
		if (!stash[fd])
			return (NULL);
		stash[fd][0] = '\0';
	}
	stash[fd] = fill_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	give_line = NULL;
	give_line = content(stash[fd]);
	if (!give_line)
		return (freestr(&stash[fd]));
	stash[fd] = clean_stash(stash[fd]);
	return (give_line);
}
