#include <stdio.h>

int main(int argc, char ** argv)
{
  char str[] = "sihtgubed";
  char *stri = &str[8];
  char *buf[9];
  char **bufi, **bufend;
  bufi = buf;
  bufend = &buf[9];

  while (bufi != bufend){
    *bufi = stri;
    bufi++;
    stri--;
  }

  while (bufi != buf){
    // moved bufi-- to top of while loop,
    // Else the first read will occurr outside of
    // the array str since size of str is 9 and size of buf is 10.
    bufi--;
    *(*bufi) -= 32;
  }

  while (bufi != bufend){
    printf("%c", **bufi);
    bufi++;
  }
  printf("\n");
}
