#include "main.hpp"

int	main()
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address)); //set la memoire a 0 first
	address.sin_family = AF_INET; //ipv4
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = INADDR_ANY;

	int	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) exit(1);
	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		close(fd), exit (1);
	return (0);
}
