#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define FIFO1 "/tmp/FIFO_NAME1"
#define FIFO2 "/tmp/FIFO_NAME2"
#define FIFO3 "/tmp/FIFO_NAME3"


int main(int argc, char const *argv[]) {

int fifo_fp1;
int fifo_fp2;
int fifo_fp3;

if( fifo_fp1 = open(FIFO1, O_RDONLY)) == NULL){ err("fopen"); }

if( fifo_fp2 = open(FIFO2, O_RDONLY)) == NULL){ err("fopen"); }

if( fifo_fp3 = open(FIFO3, O_RDONLY)) == NULL){ err("fopen"); }

for (;;)
{
  fd_set fds;
  int maxfd;
  int max;
  int res;
  char buf[256];

  FD_ZERO(&fds); // Clear FD set for select
  FD_SET(fifo_fp1, &fds);
  FD_SET(fifo_fp2, &fds);
  FD_SET(fifo_fp3, &fds);

  max = fifo_fp1 > fifo_fp2 ? fifo_fp1 : fifo_fp2;
  maxfd = max > fifo_fp3 ? max : fifo_fp3;

  select(maxfd + 1, &fds, NULL, NULL, NULL);


  if (FD_ISSET(fifo_fp1, &fds))
  {
     // We can read from fifo_fp1
     res = read(fifo_fp1, buf, sizeof(buf));
     if (res > 0)
     {
        printf("Read %s bytes from channel1\n", buf);
     }
  }
  if (FD_ISSET(fifo_fp2, &fds))
  {
     // We can read from fifo_fp2
    res = read(fifo_fp2, buf, sizeof(buf));
    if (res > 0)
    {
        printf("Read %s bytes from channel2\n", buf);
    }
  }
  if (FD_ISSET(fifo_fp3, &fds))
  {
     // We can read from fifo_fp3
    res = read(fifo_fp3, buf, sizeof(buf));
    if (res > 0)
    {
        printf("Read %s bytes from channel3\n", buf);
    }
  }
}

  return 0;
}
