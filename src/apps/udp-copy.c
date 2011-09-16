/* Copyright 2007, John W. Romein, Stichting ASTRON
 * Copyright 2009, Andreas Horneffer, RU Nijmegen
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


enum proto { UDP, TCP, File } input_proto, output_proto;

#define MAXBLOCKSIZE 1024*1024

int  sk_in, sk_out;
char *source, *destination;
int  blocksize, delay;

void setSendBufferSize(int sk, size_t size)
{
}


int create_IP_socket(char *arg, int is_output, enum proto proto)
{
  char		     *colon;
  struct sockaddr_in sa;
  struct hostent     *host;
  int		     sk, old_sk, buffer_size = 8 * 1024 * 1024;
  unsigned short     port;
  
  if ((colon = strchr(arg, ':')) == 0) {
    fprintf(stderr, "badly formatted IP:PORT address");
    exit(1);
  }

  port = colon[1] != '\0' ? atoi(colon + 1) : 0;
  *colon = '\0';

  if ((host = gethostbyname(arg)) == 0) {
    perror("gethostbyname");
    exit(1);
  }

  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_port   = htons(port);
  memcpy(&sa.sin_addr, host->h_addr, host->h_length);

  if ((sk = socket(AF_INET, proto == UDP ? SOCK_DGRAM : SOCK_STREAM, proto == UDP ? IPPROTO_UDP : IPPROTO_TCP)) < 0) {
    perror("socket");
    exit(1);
  }

  if (is_output) {
    while (connect(sk, (struct sockaddr *) &sa, sizeof sa) < 0) {
      if (errno == ECONNREFUSED) {
	sleep(1);
      } else {
	perror("connect");
	exit(1);
      }
    }

    if (setsockopt(sk, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof buffer_size) < 0)
      perror("setsockopt failed");
  } else {
    if (bind(sk, (struct sockaddr *) &sa, sizeof sa) < 0) {
      perror("bind");
      exit(1);
    }

    if (proto == TCP) {
      listen(sk, 5);
      old_sk = sk;

      if ((sk = accept(sk, 0, 0)) < 0) {
	perror("accept");
	exit(1);
      }
      
      close(old_sk);
    }

    if (setsockopt(sk, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof buffer_size) < 0)
      perror("setsockopt failed");
  }

  return sk;
}


int create_file(char *arg, int is_output)
{
  int fd;

  if ((fd = open(arg, is_output ? O_CREAT | O_WRONLY : O_RDONLY, 0666)) < 0) {
    perror("opening input file");
    exit(1);
  }

  return fd;
}


int create_fd(char *arg, int is_output, enum proto *proto)
{
  if (strncmp(arg, "udp:", 4) == 0 || strncmp(arg, "UDP:", 4) == 0) {
    *proto = UDP;
    arg += 4;
  } else if (strncmp(arg, "tcp:", 4) == 0 || strncmp(arg, "TCP:", 4) == 0) {
    *proto = TCP;
    arg += 4;
  } else if (strncmp(arg, "file:", 5) == 0 || strncmp(arg, "FILE:", 5) == 0) {
    *proto = File;
    arg += 5;
  } else if (strchr(arg, ':') != 0) {
    *proto = UDP;
  } else {
    *proto = File;
  }

  if (is_output)
    destination = arg;
  else
    source	= arg;

  switch (*proto) {
    case UDP  :
    case TCP  : return create_IP_socket(arg, is_output, *proto);

    case File : return create_file(arg, is_output);
  }
}


void init(int argc, char **argv)
{
  if ((argc < 3)||(argc > 5)) {
    fprintf(stderr, "Usage: \"%s src-addr dest-addr\", where addr is [tcp:|udp:]ip-addr:port or [file:]filename [blocksize [delay]]\n", argv[0]);
    exit(1);
  }

  sk_in  = create_fd(argv[1], 0, &input_proto);
  sk_out = create_fd(argv[2], 1, &output_proto);

  setlinebuf(stdout);

  blocksize = delay = 0;
  if (argc >= 4) {
    blocksize = atoi(argv[3]);
    if (argc == 5) {
      delay = atoi(argv[4]);
    };
  };

}


int main(int argc, char **argv)
{
  time_t   previous_time = 0, current_time;
  unsigned i, nr_packets = 0, nr_bytes = 0;
  char	   buffer[MAXBLOCKSIZE];
  int      size;


  init(argc, argv);
  if ( (blocksize <10) || (blocksize >MAXBLOCKSIZE) ){
    printf("Unsupported blocksize: %d setting blocksize to: %d \n",blocksize,MAXBLOCKSIZE);
    blocksize = MAXBLOCKSIZE;
  };

  while ((size = read(sk_in, buffer, blocksize)) != 0) {
    if (size < 0) {
      perror("read");
      sleep(1);
    } else if (write(sk_out, buffer, size) < size) {
      perror("write");
      sleep(1);
    } else {
      nr_bytes += size;
    }

    ++ nr_packets;

    if ((current_time = time(0)) != previous_time) {
      previous_time = current_time;

      if (input_proto == UDP)
	printf("copied %u bytes (= %u packets) from %s to %s\n", nr_bytes, nr_packets, source, destination);
      else
	printf("copied %u bytes from %s to %s\n", nr_bytes, source, destination);
      nr_packets = nr_bytes = 0;
    }
    if (delay) {
      usleep(delay);
    };
  }

  return 0;
}
