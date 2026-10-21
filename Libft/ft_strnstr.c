/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:17:01 by deddaidi          #+#    #+#             */
/*   Updated: 2024/11/06 14:10:54 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[i] == '\0')
		return ((char *)s);
	while (s[i] != '\0' && i < len)
	{
		j = 0;
		while (s[i + j] == needle[j] && s[i + j] != '\0' && i + j < len)
			j++;
		if (needle[j] == '\0')
			return ((char *)s + i);
		i++;
	}
	return (0);
}
