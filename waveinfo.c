
#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// command line instructions

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
      while ((option = getopt(argc, argv, "i:o:l:")) != -1)
      {
         switch (option)
         {
         case 'i':
            cl->input = optarg;
            break;

         case 'o':
            cl->output = optarg;
            break;
         }
      }
}

int main(int argc, char **argv)
{
   CLI cl;
   cl.input = NULL;
   command_line(&cl, argc, argv);

   FILE *wav;
   wave audio;

   printf("Opening file..\n");
   if (cl.input == NULL)
      wav = stdin;
   else
      wav = fopen(cl.input, "rd");

   if (wav == NULL)
   {
      printf("Error opening filen");
      exit(1);
   }

   // reading file data into the struct
   read_audio(&audio, wav);
   wav_info(audio);
   fclose(wav);
}
