/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:26:57 by deddaidi          #+#    #+#             */
/*   Updated: 2024/11/06 14:16:51 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	sl;
	size_t	dl;

	i = 0;
	j = 0;
	sl = ft_strlen(src);
	while (dst[i] != '\0')
		i++;
	dl = i;
	if (size == 0 || size <= dl)
		return (sl + size);
	while (src[j] != '\0' && j < size - dl - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dl + sl);
}
