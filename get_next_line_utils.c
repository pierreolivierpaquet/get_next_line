/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaquet <marvin@42quebec.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 10:13:24 by ppaquet           #+#    #+#             */
/*   Updated: 2023/04/01 10:13:25 by ppaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Calculates the number of characters (lenght) of the string provided,
then returns the value (l). */
size_t	ft_gnl_strlen(char *str)
{
	size_t	l;

	if (!str || !str[0])
		return (0);
	l = 0;
	while (str[l])
		l++;
	return (l);
}

/* Calculates the lenght of both strings, allocates a memory block to join these
into the newly created string (res). Copies "s1", then "s2" to the "res" then
NULL terminate. Before returning the new string, the s1 memory block is freed.
In any case of memory allocating error, 's1' is managed to be properly freed.*/
char	*ft_gnl_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!s1)
		return (NULL);
	res = malloc((ft_gnl_strlen(s1) + ft_gnl_strlen(s2) + 1) * sizeof(*res));
	if (!res)
		return (freestr(&s1));
	i = 0;
	j = 0;
	while (s1[j] != '\0')
		res[i++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		res[i++] = s2[j++];
	res[i] = '\0';
	s1 = freestr(&s1);
	return (res);
}

/* Search for the first occurence of 'c' in "s1" character string. If 'c' is
targetted, it's adress is returned; Otherwise, NULL is returned if not
found */
char	*ft_gnl_strchr(char *s1, int c)
{
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	if (((char)c) == '\0')
		return ((char *)&s1[ft_gnl_strlen(s1)]);
	while (s1[i] != '\0')
	{
		if (s1[i] == (char)c)
			return ((char *)&s1[i]);
		i++;
	}
	return (NULL);
}

/* Return the string made from all characters located before the first occuring
'\n' (or '\0', in the case of last line). */
char	*content(char *str)
{
	char	*gnl_content;
	size_t	i;
	size_t	j;

	if (!str)
		return (freestr(&str));
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	gnl_content = malloc((i + 1) * sizeof(*gnl_content));
	if (!gnl_content)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		gnl_content[j++] = str[i++];
	if (str[i] == '\n')
		gnl_content[j++] = str[i++];
	gnl_content[j] = '\0';
	return (gnl_content);
}

/* Cleans "stash" from the characters located before the '\n', and keeps the one
for next line. Stores these inside a new allocated memory block, the returns
the new "cleand" character strings. */
char	*clean_stash(char *stash)
{
	char	*cleaned;
	size_t	l;
	size_t	i;

	if (!stash)
		return (freestr(&stash));
	l = ft_gnl_strlen(stash);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\0')
		return (freestr(&stash));
	cleaned = malloc((l - i + 1) * sizeof(*cleaned));
	if (!cleaned)
		return (freestr(&stash));
	l = 0;
	i++;
	while (stash[i] != '\0')
		cleaned[l++] = stash[i++];
	cleaned[l] = '\0';
	free(stash);
	stash = NULL;
	return (cleaned);
}
