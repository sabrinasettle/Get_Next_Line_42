/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssettle <ssettle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 11:49:15 by ssettle           #+#    #+#             */
/*   Updated: 2019/03/26 13:38:18 by ssettle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char			*ft_strccpy(char *dst, const char *src, char c)
{
	int i;

	i = 0;
	while (src[i] && src[i] != c)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static t_list	*getme(int fd, t_list **file)
{
	t_list	*tmp;

	tmp = *file;
	while (tmp)
	{
		if ((int)tmp->content_size == fd)
			return (tmp);
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_list));
	tmp->content = malloc(1);
	ft_memcpy(tmp->content, "\0", 1);
	tmp->content_size = fd;
	ft_lstadd(file, tmp);
	tmp = *file;
	return (tmp);
}

void			cleanme(t_list *text)
{
	char	*tmp;

	if ((tmp = ft_strchr(text->content, '\n')))
	{
		tmp = ft_strdup(tmp + 1);
		free(text->content);
		text->content = tmp;
	}
	else
		ft_strclr(text->content);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*file_list;
	t_list			*file;
	char			*tmp;
	char			buf[BUFF_SIZE + 1];
	int				ret;

	ERR_OCR(fd < 0 || line == NULL || read(fd, buf, 0) < 0);
	file = getme(fd, &file_list);
	while ((ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = '\0';
		ERR_OCR(!(tmp = ft_strjoin(file->content, buf)));
		free(file->content);
		file->content = tmp;
		BREAK_OCR((ft_strchr(file->content, '\n')));
	}
	if (!ret && !ft_strlen(file->content))
	{
		ft_strclr(*line);
		return (0);
	}
	ERR_OCR(!(*line = ft_strnew(ft_strlen(file->content))));
	*line = ft_strccpy(*line, file->content, '\n');
	cleanme(file);
	return (1);
}
