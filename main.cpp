#include <iostream>

/*
	抄写的原页面地址：
	http://blog.csdn.net/xiajun07061225/article/details/9250579
*/

using std::cerr;
using std::cout;
using std::endl;

/*
	创建监听fd
*/
int create_and_bind(char* port)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *result, *rp;
	int s = getaddrinfo(NULL, port, &hints, *result);
	if (s != 0)
	{
		cerr << "getaddrinfo:" << gai_strerror(s) << endl;
		return -1;
	}

	int sfd;
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
			// success
			break;
		}

		close(sfd);
	}

	if (rp == NULL)
	{
		cerr << "count not bind" << endl;
		return -1;
	}

	freeaddrinfo(result);

	return sfd;
}

int make_socket_non_blocking(int sfd)
{
	//得到文件状态标志
	int flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
		cerr << "fcntl get" << endl;
		return -1;
	}

	//设置文件状态标志
	flags |= O_NONBLOCK;
	int s = fcntl(sfd, F_SETFL, flags);
	if (s == -1)
	{
		cerr << "fcntl set" << endl;
		return -1;
	}

	return 0;
}

int main()
{
	char* port = "65001";

	int sfd = create_and_bind(port);
	if (sfd == -1)
		abort();

	int s = make_socket_non_blocking(sfd);
	if (s == -1)
		abort();

	s = listen(sfd, SOMAXCONN);
	if (s == -1)
	{
		cerr << "listen" << endl;
		abort();
	}



	return 0;
}
