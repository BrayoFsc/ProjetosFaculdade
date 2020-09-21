#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
      while ((option = getopt(argc, argv, "i:o:l:t:")) != -1)
      {
         switch (option)
         {
         case 'i':
            cl->input = optarg;
            break;
         case 'o':
            cl->output = optarg;
            break;
         case 'l':
            cl->vol = atof(optarg);
            break;
         case 't':
            cl->delay = atoi(optarg) * 1000;
            break;
         }
      }
}

int main(int argc, char **argv)
{
   CLI cl;
   cl.input = NULL;
   cl.output = NULL;
   cl.vol = 1;
   command_line(&cl, argc, argv);
   if (cl.vol >= 10 || cl.vol < 0)
      perror("-l must be 0.0<=l>=10.0");

   FILE *wav;
   wave audio;

   printf("Opening file..\n");
   if (cl.input == NULL)
      wav = stdin;
   else
      wav = fopen(cl.input, "rd");

   if (wav == NULL)
      perror("Error opening filen");

   // reading file data into the struct
   read_audio(&audio, wav);
   wav_wide(&audio, cl.vol);
   wav_play(audio, cl);
   fclose(wav);
}
