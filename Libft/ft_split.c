/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 01:23:18 by deddaidi          #+#    #+#             */
/*   Updated: 2024/11/10 17:50:01 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (count);
}

static int	word_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*word_trim(char const *s, char c)
{
	int		i;
	char	*t;
	int		j;

	j = 0;
	i = word_len(s, c);
	t = malloc(sizeof(char) * (i + 1));
	if (t == NULL)
		return (0);
	while (s[j] && s[j] != c)
	{
		t[j] = s[j];
		j++;
	}
	t[j] = '\0';
	return (t);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**t;

	while (!s)
		return (NULL);
	i = 0;
	t = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (t == NULL)
		return (0);
	while (*s)
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s != '\0')
		{
			t[i] = word_trim(s, c);
			if (!t[i])
				return (free_array(t));
			i++;
		}
		while (*s != '\0' && *s != c)
			s++;
	}
	t[i] = 0;
	return (t);
}
