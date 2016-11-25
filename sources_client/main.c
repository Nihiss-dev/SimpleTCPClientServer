#include "client.h"

int	init_connection(char *address)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;
  struct hostent *hostinfo;

  if (sock == -1)
    {
      write(1, "Cannot create socket\n", 21);
      return (-1);
    }
  if ((hostinfo = gethostbyname(address)) == NULL)
    {
      write(1, "Can't get infos\n", 16);
      return (-1);
    }
  /*
   * Just setting the fields of the struct sockaddr_in
   */
  sin.sin_addr = *(struct in_addr*) hostinfo->h_addr;

  /*
   * For the address, we also can do as following
   * sin.sin_addr.s_addr(inet_addr(address))
   */

  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET;

  /*
   * Trying to connect with info stored in struct sockaddr_in
   */
  if (connect(sock, (struct sockaddr*)&sin, sizeof(struct sockaddr)) == -1)
    {
      write(1, "Can't connect\n", 14);
      return (-1);
    }
  return (sock);
}

void	client(char **av)
{
  /*
   * create socket
   */
  int sock = init_connection(av[1]);
  char buf[1024];
  while (1)
    {
      /*
       * set all the bytes of duf to 0
       * avoid garbage in our data
       */
      bzero(buf, 1024);
      if (read(0, buf, 1024) <= 0)
	{
	  write(1, "Error while reading\n", 21);
	  return;
	}
      /*
       * Send our data to the server
       */
      write(sock, buf, strlen(buf));
      bzero(buf, 1024);
      if (read(sock, buf, 1024) <= 0)
	{
	  write(1, "Error while reading\n", 21);
	  return;
	}
      write(1, buf, strlen(buf));
    }
}

int	main(int ac, char **av)
{
  if (ac < 2)
    {
      write(1, "You must give the address of the server\n", 33);
      return (1);
    }
  client(av);
  return (0);
}
