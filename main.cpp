#include "main.hpp"

std::vector<std::string>	TabExec(std::string cmd)
{
	cmd = cmd.erase(cmd.size() - 1);
	std::cout << "CMD = " << cmd << std::endl;
	if (cmd.find(' ') == std::string::npos)
	{
		const char *msg = "CMD is missing arguments \n";
		throw (msg);
	}
	if (cmd == "CAP LS")
		std::cout << "IGNORE CAP LS" << std::endl;

	std::stringstream ss(cmd);
	std::string word;
	std::vector<std::string> vec;
	while (!ss.eof())
		getline(ss, word, ' '), vec.push_back(word); //split la ligne par les espaces et fill le vector
	return (vec);
}

void	Parser(std::string cmd)
{
	std::vector<std::vector<std::string> > vecvec; //vec de vec, contient CMD en 1st place
	std::cout << cmd << std::endl;
	int y = 0;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\n' && cmd[i-1] == '\r')
		{
			vecvec.push_back(TabExec(cmd.substr(y, i - y)));
			y = i + 1;
		}
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
	std::cout << "Connection accepted, waiting for authentification..." << std::endl;
	char buffer[1024] = {0};
	memset(buffer, 0, sizeof(buffer));
	while (1)
	{
		int receive = recv(new_fd, buffer, 1024, 0);
		if (receive < 1)
			break ;
		std::string iencli(buffer, receive);
		try
		{
			Parser(iencli);
		}
		catch (std::string msg)
		{
			std::cerr << msg << std::endl;
			close(new_fd);
			close(fd);
		}
	}
	close(new_fd);
	close(fd);
	return (0);
}
