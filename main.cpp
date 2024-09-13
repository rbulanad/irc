#include "main.hpp"

int ft_stoi(std::string & s)
{
    int i;
    std::istringstream(s) >> i;
    return i;
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (std::cout << "ERROR: Enter a port." << std::endl, 1);
	int	port = atoi(argv[1]);
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address)); //set la memoire a 0 first
	address.sin_family = AF_INET; //ipv4
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	int	addrlen = sizeof(address);

	int	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) exit(1);
	if (bind(fd, (struct sockaddr *)&address, addrlen) < 0)
		close(fd), std::cout << "ERROR: Failed bind." << std::endl, exit (1);
	if (listen(fd, 3) < 0)
		close(fd), std::cout << "ERROR: Failed listen." << std::endl, exit(1);
	std::cout << "Currently listening on port: " << port << std::endl;
	int	new_fd = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_fd < 0)
		close(fd), std::cout << "ERROR: Failed accept." << std::endl, exit(1);
	std::cout << "Connection accepted." << std::endl;
	close(new_fd);
	close(fd);
	return (0);
}
