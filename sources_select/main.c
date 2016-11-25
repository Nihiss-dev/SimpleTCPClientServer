#include "server_select.h"

void	server()
{
  int	sock;
  int	new_socket;
  int	client_socket[MAX_CLIENTS];
  int	max_sd;
  int	activity;
  struct sockaddr_in sin;
  char	buf[1024];
  fd_set readfds;
  int i;

  /*
   * init all the sockets to 0
   */
  for (i = 0; i < MAX_CLIENTS; ++i)
    {
      client_socket[i] = 0;
    }

  /*
   * create socket
   */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
      write(1, "Cannot create socket\n", 21);
      return;
    }
  int opt = 1;
  if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
		  sizeof(opt))) < 0)
    {
      write(1, "Error with setsockopt\n", 22);
      return;
    }
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET;

  /*
   * Bind the socket
   */
  if ((bind(sock, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
      write(1, "Cannot bind\n", 12);
      return;
    }

  /*
   * Start listening
   */
  if (listen(sock, 3) < 0)
    {
      write(1, "Cannot listen\n", 14);
      return;
    }
  int addrlen = sizeof(sin);
  int sd;
  while (1)
    {
      /*
       * Clear the set of fds
       */
      FD_ZERO(&readfds);

      /*
       * Set the main socket to set
       */
      FD_SET(sock, &readfds);
      max_sd = sock;
      for (i = 0; i < MAX_CLIENTS; ++i)
	{
	  sd = client_socket[i];
	  if (sd > 0)
	    FD_SET(sd, &readfds);
	  if (sd > max_sd)
	    max_sd = sd;
	}

      if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
	{
	  write(1, "Error with select\n", 18);
	  return;
	}

      /*
       * if something happened on main socket, there is an incoming connexion
       */
      if (FD_ISSET(sock, &readfds))
	{
	  if ((new_socket = accept(sock, (struct sockaddr*)&sin,
				   (socklen_t*)&addrlen)) < 0)
	    {
	      write(1, "Cannot accept\n", 14);
	      return;
	    }
	  for (i = 0; i < MAX_CLIENTS; ++i)
	    {
	      /*
	       * This means position is empty
	       * So we add it to the array of socket
	       */
	      if (client_socket[i] == 0)
		{
		  client_socket[i] = new_socket;
		  break;
		}
	    }
	}
      for (i = 0; i < MAX_CLIENTS; ++i)
	{
	  sd = client_socket[i];

	  /*
	   * Something happened in this socket
	   */
	  if (FD_ISSET(sd, &readfds))
	    {
	      bzero(buf, 1024);
	      if (read(sd, buf, 1024) == 0)
		{

		  /*
		   * Someone has disconnected
		   */
		  close(sd);
		  client_socket[i] = 0;
		}
	      else
		{
		  write(1, buf, strlen(buf));
		  write(sd, buf, strlen(buf));
		}
	    }
	}
    }
}

int	main()
{
  server();
  return (0);
}
