#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
   int flag_a = 0;
   int flag_b = 0;
   char *value_c = NULL;
   int option;

   opterr = 0;

   // options:  -a, -b, -c value (defined by "abc:")
   while ((option = getopt(argc, argv, "abc:")) != -1) switch (option)
      {
      case 'a': // option -a was set
         flag_a = 1;
         break;
      case 'b': // option -b was set
         flag_b = 1;
         break;
      case 'c': // option -c was set with value
         value_c = optarg;
         break;
      default:
         fprintf(stderr, "Usage: %s -a -b -c value\n", argv[0]);
         exit(1);
      }

   printf("flag_a = %d, flag_b = %d, value_c = %s\n", flag_a, flag_b, value_c);

   return 0;

   printf("%.4s", audio.chunkID);
   printf("%u", audio.chunkSize);
   printf("%.4s", audio.format);
   printf("%.4s", audio.fmtChunkID);
   printf("%u", audio.fmtChunkSize);
   printf("%u", audio.audioFormat);
   printf("%u", audio.channels);
   printf("%u", audio.sampleRate);
   printf("%u", audio.byteRate);
   printf("%u", audio.blockAlign);
   printf("%u", audio.bitsPS);
   printf("%.4s", audio.dataChunkID);
   printf("%u", audio.dataChunkSize);
   printf("%u", audio.bitsPS / 8);
   printf("%u", audio.samplesPC);
}