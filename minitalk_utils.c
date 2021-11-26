#include "includes/minitalk.h"
#include <stdio.h>


void	ft_bzero(void *s, size_t n)//memsetでもできる。
{
	size_t	i;
	char	*s_chr_p;

	i = 0;
	s_chr_p = (char *)s;
	while (i < n)
	{
		*(s_chr_p + i) = 0;
		i++;
	}
}

int	main()
{
	char test[] = "aaaaaaaaaaaa";
	int i = 0;
	ft_bzero((void *)test,6);
	while (i < 10)
	{
		printf("%c\n",test[i]);
		i++;
	}
	return (0);
}
