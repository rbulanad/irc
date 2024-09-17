#include "main.hpp"

void	Parser(std::string cmd)
{
	std::string msg;
	std::cout << cmd << std::endl;
	if (cmd[0] == '\n')
	{
		 msg = "Skip";
		throw (msg);
	}
	if (cmd.find('\n') != std::string::npos && cmd.find('\r') != std::string::npos && cmd.find(' ') != std::string::npos)
	{
		msg = "Not a command";
		throw (msg);
	}
}

int	main(int argc, char **argv)
{
	if (argc < 3)
		return (std::cout << "ERROR: Enter a port and password." << std::endl, 1);
	int	port = atoi(argv[1]);
	if (port <= 0)
		return (std::cout << "ERROR: Invalid port." << std::endl, 1);

	std::string servPass = argv[2];
	struct sockaddr_in address;
	int	addrlen = sizeof(address);
	memset(&address, 0, addrlen); //set la memoire a 0 first
	address.sin_family = AF_INET; //ipv4
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	int	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		close(fd), std::cout << "ERROR: Failed bind." << std::endl, exit (1);

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		close(fd), std::cout << "ERROR: Failed setsockopt." << std::endl, exit (1);

	if (bind(fd, (struct sockaddr *)&address, addrlen) < 0)
		close(fd), std::cout << "ERROR: Failed bind." << std::endl, exit (1);

	if (listen(fd, 3) < 0)
		close(fd), std::cout << "ERROR: Failed listen." << std::endl, exit(1);

	std::cout << "Currently listening on port: " << port << std::endl;
	int	new_fd = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_fd < 0)
		close(fd), std::cout << "ERROR: Failed accept." << std::endl, exit(1);
	//std::cout << "Connection accepted, waiting for authentification..." << std::endl;
	//const char *msg = "Password? ";
	//send (new_fd, msg, strlen(msg), 0);
	char buffer[1024] = {0};
	memset(buffer, 0, sizeof(buffer));
	while (1)
	{
		int receive = recv(new_fd, buffer, 1024, 0);
		if (receive < 1)
			break ;
		std::string iencli(buffer, receive);
		try { Parser(iencli); }
		catch (std::string msg)
		{
			std::cerr << msg << std::endl;
		}
	}
	/*clientPass.erase(std::remove(clientPass.begin(), clientPass.end(), '\n'), clientPass.end());
	clientPass.erase(std::remove(clientPass.begin(), clientPass.end(), '\r'), clientPass.end());
	f (clientPass != servPass)
	{
		msg = "Wrong password.";
		send (new_fd, msg, strlen(msg), 0);
	}
	else
	{
		msg = "Good password.";
		send (new_fd, msg, strlen(msg), 0);
	}*/
	close(new_fd);
	close(fd);
	return (0);
}
