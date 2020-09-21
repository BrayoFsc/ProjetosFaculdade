#include "wav.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void read_audio(wave *audio, FILE *wav)
{
   // le os primeiros 44 bytes para o header
   fread(audio, 1, 44, wav);
   audio->samplesPC = (audio->dataChunkSize / audio->channels / (audio->bitsPS / 8));
   // se tiver apenas 1 canal armazena ele no array da direita
   if (audio->channels == 1)
   {
      audio->Right = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);
      fread(audio->Right, audio->bitsPS / 8, audio->samplesPC, wav);

   } else
   {
      audio->Right = (int16_t *)malloc(audio->samplesPC);
      audio->Left = (int16_t *)malloc(audio->samplesPC);
      for (int i = 0; i < audio->samplesPC; i++)
      {
         fread(&audio->Right[i], audio->bitsPS / 8, 1, wav);
         fread(&audio->Left[i], audio->bitsPS / 8, 1, wav);
      }
   }
}

void wav_info(wave audio)
{
   printf("riff tag        (4 bytes): \"%.4s\"\n", audio.chunkID);
   printf("riff size       (4 bytes): %u\n", audio.chunkSize);
   printf("wave tag        (4 bytes): \"%.4s\"\n", audio.format);
   printf("form tag        (4 bytes): \"%.4s\"\n", audio.fmtChunkID);
   printf("fmt_size        (4 bytes): %u\n", audio.fmtChunkSize);
   printf("audio_format    (2 bytes): %u\n", audio.audioFormat);
   printf("num_channels    (2 bytes): %u\n", audio.channels);
   printf("sample_rate     (4 bytes): %u\n", audio.sampleRate);
   printf("byte_rate       (4 bytes): %u\n", audio.byteRate);
   printf("block_align     (2 bytes): %u\n", audio.blockAlign);
   printf("bits_per_sample (2 bytes): %u\n", audio.bitsPS);
   printf("data tag        (4 bytes): \"%.4s\"\n", audio.dataChunkID);
   printf("data size       (4 bytes): %u\n", audio.dataChunkSize);
   printf("bytes per sample         : %u\n", audio.bitsPS / 8);
   printf("samples per channel      : %u\n", audio.samplesPC);
}

void wav_play(wave audio, CLI cl)
{
   FILE *output;
   if (cl.output == NULL) // caso não aja output, a saída vai para o stdout
      output = stdout;
   else
      output = fopen(cl.output, "w");

   fwrite(&audio, 1, 44, output);
   if (audio.channels == 1) // quando tem apenas um canal o fwrite pode ser usando apenas 1 vez
      fwrite(audio.Right, audio.bitsPS / 8, audio.samplesPC, output);
   else // já com 2 canais é necessário um for, já que eles foram armazenados em vetores diferentes
   {
      for (int i = 0; i <= audio.samplesPC; i++)
      {
         fwrite(&audio.Right[i], audio.bitsPS / 8, 1, output);
         fwrite(&audio.Left[i], audio.bitsPS / 8, 1, output);
      }
   }
   fclose(output);
}

void wav_vol(wave *audio, CLI cl)
{
   float vol;
   if (cl.vol == 0)
      vol = 1;
   else
      vol = cl.vol;
   // aqui simplismente multiplico o valor da sample pelo volume, com a diferença entre 1 e 2 canais
   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
         audio->Right[i] = audio->Right[i] * vol;
   else
   {
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         audio->Right[i] = audio->Right[i] * vol;
         audio->Left[i] = audio->Left[i] * vol;
      }
   }
}

void wav_norm(wave *audio)
{
   int16_t max; // primeiro preciso encontrar o maior valor absoluto das samples
   max = found_max(audio);

   double norm;
   norm = (float)INT16_MAX / max; // depois calculo qual a normalização, dividindo o maior valor possível pelo maior sample
   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
         audio->Right[i] = clipping(audio->Right[i] * norm); // depois multiplico cada valor pela norm e previno o clipping
   else
      for (int i = 0; i <= audio->samplesPC; i++)
      {
         audio->Right[i] = clipping(audio->Right[i] * norm);
         audio->Left[i] = clipping(audio->Left[i] * norm);
      }
}

int16_t found_max(wave *audio)
{
   // simplismente percorro as samples e guardo o maior valor
   int16_t max;
   if (audio->channels == 1)
      for (int i = 0; i <= audio->samplesPC; i++)
      { // abs() me garante o maior valor absoluto, seja ele positivo ou negativo
         if (abs(audio->Right[i]) > max)
            max = abs(audio->Right[i]);
      }
   else
   {
      for (int i = 0; i <= audio->samplesPC; i++)
         if (abs(audio->Right[i]) > max)
            max = abs(audio->Right[i]);

      for (int i = 0; i <= audio->samplesPC; i++)
         if (abs(audio->Left[i]) > max)
            max = abs(audio->Left[i]);
   }
   return max;
}

int16_t clipping(int16_t sample)
{
   // para previnir o clipping basta ver se a sample passou de INT16_MAX, se passou
   // eu retorno esse valor, se não passou eu retorno a sample
   if (sample > INT16_MAX)
      return INT16_MAX;
   else if (sample < INT16_MIN)
      return INT16_MIN;
   return sample;
}

void wav_rev(wave *audio)
{
   //[UM CANAL]
   if (audio->channels == 1)
   { // primeiro crio um vetor auxiliar do mesmo tamanho do original
      int16_t *rightcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);
      // depois eu copio o original nele
      for (int i = 0; i <= audio->samplesPC; i++)
         rightcpy[i] = audio->Right[i];
      // depois simplismente copio a copia invertida
      for (int i = 0; i < audio->samplesPC; i++)
         audio->Right[i] = rightcpy[audio->samplesPC - i];
      // depois dou free na cópia
      free(rightcpy);

   } else
   { // para dois canais o sistema é o mesmo porém duplicado
      int16_t *rightcpy = NULL, *leftcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);
      leftcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);

      for (int i = 0; i <= audio->samplesPC; i++)
      {
         rightcpy[i] = audio->Right[i];
         leftcpy[i] = audio->Left[i];
      }

      for (int i = 0; i <= audio->samplesPC; i++)
      {
         audio->Right[i] = rightcpy[audio->samplesPC - i];
         audio->Left[i] = leftcpy[audio->samplesPC - i];
      }
      free(rightcpy);
      free(leftcpy);
   }
}

void wav_echo(wave *audio, CLI cl)
{
   // calculo o delay/echo que está em segundos
   float delay = (float)cl.delay / 1000;
   int echo = (audio->sampleRate * delay);

   if (audio->channels == 1)
   { // faço uma copia para evitar eco do eco e clipping
      int16_t *rightcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);
      // copiando a copia
      for (int i = 0; i <= audio->samplesPC; i++)
         rightcpy[i] = audio->Right[i];
      // para cada amostra eu somo a respectiva com delay da cópia vezes o volume na amostra do canal
      for (int i = echo; i <= audio->samplesPC; i++)
         audio->Right[i] += cl.vol * rightcpy[i - echo];
      free(rightcpy);

   } else
   {
      int16_t *rightcpy = NULL, *leftcpy = NULL;
      rightcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);
      leftcpy = (int16_t *)malloc(sizeof(int16_t) * audio->samplesPC);

      for (int i = 0; i <= audio->samplesPC; i++)
      {
         rightcpy[i] = audio->Right[i];
         leftcpy[i] = audio->Left[i];
      }

      for (int i = echo; i <= audio->samplesPC; i++)
      {
         audio->Right[i] += cl.vol * rightcpy[i - echo];
         audio->Left[i] += cl.vol * leftcpy[i - echo];
      }
      free(rightcpy);
      free(leftcpy);
   }
}

void wav_wide(wave *audio, float vol)
{ // verifeicando se o audio tem 2 canais
   if (audio->channels == 1)
      perror("audio must have 2 channels");

   // diminuo uma amostra de aúdio da outra depois somo cada uma com essa diminuição vezes o volume;
   for (int i = 0; i < audio->samplesPC; i++)
   {
      int diff = audio->Right[i] - audio->Left[i];
      audio->Right[i] += vol * diff;
      audio->Left[i] += vol * diff;
   }
}

void read_cat(wave *audio, FILE *wav, int files)
{ // armazeno os dados dos arquivos anteriores, se não tiver nenhum anterior, deve ser 0
   uint32_t chunkmax = audio->chunkSize;
   uint32_t datamax = audio->dataChunkSize;
   uint32_t old_samplePC = audio->samplesPC;
   fread(audio, 1, 44, wav);

   // leio o header e cálculo o samplesPC que é igual o do arquivo anterior mais o atual
   audio->samplesPC += (audio->dataChunkSize / audio->channels / (audio->bitsPS / 8));

   if (audio->channels == 1)
   { // leio o aquivo atual a partir do final do anterior
      audio->Right = (int16_t *)realloc(audio->Right, audio->samplesPC * sizeof(int16_t));

      for (int i = old_samplePC; i <= audio->samplesPC; i++)
         fread(&audio->Right[i], audio->bitsPS / 8, 1, wav);

   } else
   {
      audio->Right = (int16_t *)realloc(audio->Right, audio->samplesPC * sizeof(int16_t));
      audio->Left = (int16_t *)realloc(audio->Left, audio->samplesPC * sizeof(int16_t));

      for (int i = old_samplePC; i <= audio->samplesPC; i++)
      {
         fread(&audio->Right[i], audio->bitsPS / 8, 1, wav);
         fread(&audio->Left[i], audio->bitsPS / 8, 1, wav);
      }
   }
   // somo os arquivos guardados no arquivo atual
   audio->chunkSize += chunkmax;
   audio->dataChunkSize += datamax;
}
