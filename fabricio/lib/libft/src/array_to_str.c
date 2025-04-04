/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flopez-r <flopez-r@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:10:21 by albartol          #+#    #+#             */
/*   Updated: 2024/09/04 13:36:41 by flopez-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*array_to_str(char **array, const char spliter)
{
	char	*str;
	char	split[2];
	size_t	len;
	int		i;

	if (spliter)
	{
		split[0] = spliter;
		split[1] = 0;
		len = array_char_len(array) + array_len(array);
	}
	else
		len = array_char_len(array);
	str = (char *)ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (array[i])
	{
		ft_strlcat(str, array[i], len + 1);
		i++;
		if (spliter && array[i])
			ft_strlcat(str, split, len + 1);
	}
	return (str);
}
