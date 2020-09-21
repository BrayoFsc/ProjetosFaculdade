#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void command_line(CLI *cl, int argc, char **argv)
{
   int option;
   if (argc > 1)
   {
      option = getopt(argc, argv, "i:o:l:t:");
      switch (option)
      {
      case 'o':
         cl->output = optarg;
         cl->files = argc - 3; // tira o -o, output e o 0 que é o executavel
         break;
      }
   }
}

int main(int argc, char **argv)
{
   CLI cl;
   cl.output = NULL;
   command_line(&cl, argc, argv);

   if (argc < 1)
      perror("no file selected");

   FILE **wav;
   wav = NULL;
   wav = malloc(cl.files * sizeof(FILE *));
   if (wav == NULL)
      perror("Error on memory allocation");

   printf("Opening files..\n");

   for (int i = 1; i <= cl.files; i++)
      wav[i - 1] = fopen(argv[i], "rd");

   wave audio;
   audio.Right = NULL;
   audio.Left = NULL;

   // reading file data into the struct
   audio.dataChunkSize = 0;
   audio.fmtChunkSize = 0;
   audio.samplesPC = 0;
   for (int i = 0; i < cl.files; i++)
      read_cat(&audio, wav[i], cl.files);

   wav_play(audio, cl);
   for (int i = 0; i < cl.files; i++)
      fclose(wav[i]);
}
