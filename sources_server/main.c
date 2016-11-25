#include "server.h"

void	server()
{
  int	sock;
  int	client_sock;
  int	read_size;
  int	struct_size;
  char	buf[1024];
  struct sockaddr_in sin;
  struct sockaddr_in client;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      write(1, "Cannot create socket\n", 21);
      return;
    }
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET;

  /*
   * Bind the socket with the infos stored in struct sockaddr_in
   */
  if ((bind(sock, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
      write(1, "Cannot bind\n", 12);
      return;
    }

  /*
   * start listening on the socket
   */
  listen(sock, 3);

  /*
   * accept incoming connexions
   */
  struct_size = sizeof(struct sockaddr_in);
  if ((client_sock = accept(sock, (struct sockaddr*)&client,
			    (socklen_t*)&struct_size)) < 0)
    {
      write(1, "Accept failed\n", 14);
      return;
    }

  /*
   * Just read data the client sent
   * and send him back
   */
  bzero(buf, 1024);
  while (read_size = (read(client_sock, buf, 1024)) > 0)
    {
      write(1, buf, strlen(buf));
      //write(client_sock, "You sent me: ", 13);
      write(client_sock, buf, strlen(buf));
      bzero(buf, 1024);
    }
  if (read_size == 0)
    {
      write(1, "client disconnected\n", 20);
      close(client_sock);
    }
  if (read_size < 0)
    {
      write(1, "read failed\n", 12);
      close(client_sock);
    }
  close(sock);
  return;
}

int	main()
{
  server();
  return (0);
}
