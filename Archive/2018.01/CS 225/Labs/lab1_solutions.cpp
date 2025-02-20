/******************************************************************************
* Name       : Cody Morgan
* Assignment : Lab 1
* Class      : CS 225
* Semester   : Fall 2018
******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>


/********** LAB 1 *********/

unsigned short readUnsignedShort(char *block, int offset)
{
  return (unsigned short)block[offset] + ((unsigned short)block[offset + 1] << 8);
}

void writeUnsignedInt(char *block, int offset, unsigned int value)
{
  unsigned const intBytes = sizeof(unsigned int);
  unsigned const bits = 8;
  char mask = 0xFF;  //mask for writing bytes

  for (unsigned i = 0; i < intBytes; i++)
  {
    char nextByte = value & mask;
    block[offset + i] = nextByte;
    value >>= bits;
  }
}

int lab1()
{
  unsigned char stuff[] = { 0x01,0x23,0x34,0x56,0x78,0x9a,0xbc,0xde };
  unsigned short x;

  x = readUnsignedShort((char*)stuff, 3);
  printf("expected: %u\n", 30806u);
  printf("     got: %d\n", x);

  writeUnsignedInt((char*)stuff, 2, 17);
  printf("expected: 0x01 0x23 0x11 0x00 0x00 0x00 0xbc 0xde\n");
  printf("     got: ");
  for (int i = 0; i < 8; ++i)
    printf("0x%02x ", stuff[i]);
  printf("\n\n\n");

  return 0;
}

/********** LAB 2 *********/

int getFloat(const char *fname, int offset, float *pvalue) 
{
  std::ifstream file(fname, std::ios::binary | std::ios::ate); //ate: at the end
  if (file)
  {
    std::ifstream::pos_type pos = file.tellg(); //current position is at the end
    std::vector<char> buffer(pos);              //vector is the size of the file
    file.seekg(0, std::ios::beg);               //move the file index to beginning
    file.read(&buffer[0], pos);                 //read entire file into buffer

    if (offset < buffer.size())
    {
      *pvalue = *reinterpret_cast<float*>(&buffer[offset]);
      return 1;
    }
  }
  return 0;
}

int lab2()
{
  FILE *file;
  float *data;
  float value;
  int okay;

  /* test file */
  data = (float*)malloc(10 * sizeof(float));
  for (int i = 0; i < 10; ++i)
    data[i] = 0.321f*i;
  file = fopen("test.dat", "wb");
  if (file)
  {
    fwrite(data, 10, sizeof(float), file);
    fclose(file);
  }

  /* tests */
  okay = getFloat("test.dat", 4 * sizeof(float), &value);
  printf("expected: okay=%d, value=%f\n", 1, 4 * 0.321f);
  printf("     got: okay=%d, value=%f\n", okay, value);

  printf("\n");

  okay = getFloat("test.dat", 12 * sizeof(float), &value);
  printf("expected: okay=%d\n", 0);
  printf("     got: okay=%d\n", okay);

  printf("\n");

  okay = getFloat("yabbadabba.doo", 123456, &value);
  printf("expected: okay=%d\n", 0);
  printf("     got: okay=%d\n", okay);

  return 0;
}

/********** LAB 3 *********/

unsigned int isWave16(const char *fname) 
{
  FILE *file;
  file = fopen(fname, "rb");

  // #1 check: file esists
  if (file)
  {
    // #2 : WAVE marker at index 8
    char buffer[4] = { 0 };
    fseek(file, 8, SEEK_SET);
    fread(buffer, sizeof(char), 4, file);
    if (strcmp(buffer, "WAVE"))
    {
      // #3 : 16 found at index 34
      fseek(file, 34, SEEK_SET);
      unsigned short sixteen = 0;
      fread(&sixteen, sizeof(unsigned short), 1, file);
      if (sixteen == 16)
      {
        fseek(file, 40, SEEK_SET);
        unsigned size = 0;
        fread(&size, 1, sizeof(char), file);

        fseek(file, 0, SEEK_END);
        unsigned actualSize = ftell(file) - 44;
        if (actualSize == size)
        {
          // #4 : valid data size
          return size;
        }
      }
    }
  }
  return 0;
}

int lab3()
{
  struct WaveHeader {
    char riff_label[4];
    unsigned riff_size;
    char file_tag[4];
    char fmt_label[4];
    unsigned fmt_size;
    unsigned short audio_format;
    unsigned short channel_count;
    unsigned sampling_rate;
    unsigned bytes_per_second;
    unsigned short bytes_per_sample;
    unsigned short bits_per_sample;
    char data_label[4];
    unsigned data_size;
  };

  FILE *file;
  struct WaveHeader header;
  short data[5] = { 2, -4, 6, -8, 10 };
  unsigned int size = 10;

  /* create WAVE file */
  strncpy(header.riff_label, "RIFF", 4);
  header.riff_size = 36 + sizeof(data);
  strncpy(header.file_tag, "WAVE", 4);
  strncpy(header.fmt_label, "fmt ", 4);
  header.fmt_size = 16;
  header.audio_format = 1;
  header.channel_count = 1;
  header.sampling_rate = 8000;
  header.bytes_per_second = 2 * 8000;
  header.bytes_per_sample = 2;
  header.bits_per_sample = 16;
  strncpy(header.data_label, "data", 4);
  header.data_size = sizeof(data);
  file = fopen("test.wav", "wb");
  fwrite(&header, 1, 44, file);
  fwrite(data, 1, sizeof(data), file);
  fclose(file);

  /* tests */
  size = isWave16("test.wav");
  printf("expected: size=%u\n", (unsigned int)sizeof(data));
  printf("     got: size=%u\n", size);

  printf("\n");

  header.bits_per_sample = 8;
  file = fopen("test.wav", "wb");
  fwrite(&header, 1, 44, file);
  fwrite(data, 1, sizeof(data), file);
  fclose(file);
  size = isWave16("test.wav");
  printf("expected: size=%u\n", 0);
  printf("     got: size=%u\n", size);

  printf("\n");

  header.bits_per_sample = 16;
  header.data_size = 55;
  file = fopen("test.wav", "wb");
  fwrite(&header, 1, 44, file);
  fwrite(data, 1, sizeof(data), file);
  fclose(file);
  size = isWave16("test.wav");
  printf("expected: size=%u\n", 0);
  printf("     got: size=%u\n", size);

  return 0;
}

/********** LAB 4 *********/

void readWave(const char *fname, char *header, short *data) 
{
  FILE *file;
  file = fopen(fname, "rb");
  int headerSize = 44;

  if (file)
  {
    unsigned size = isWave16(fname);
    if (size)
    {
      fseek(file, 0, SEEK_SET);
      fread(header, headerSize, 1, file);

      fseek(file, 44, SEEK_SET);
      fread(data, sizeof(short), 1, file);
    }
  }
}

int lab4()
{
  struct WaveHeader {
    char riff_label[4];
    unsigned riff_size;
    char file_tag[4];
    char fmt_label[4];
    unsigned fmt_size;
    unsigned short audio_format;
    unsigned short channel_count;
    unsigned sampling_rate;
    unsigned bytes_per_second;
    unsigned short bytes_per_sample;
    unsigned short bits_per_sample;
    char data_label[4];
    unsigned data_size;
  };

  FILE *file;
  struct WaveHeader header;
  short data[5] = { 2, -4, 6, -8, 10 };
  char read_header[44];
  short *read_data;
  int i;

  /* create WAVE file */
  strncpy(header.riff_label, "RIFF", 4);
  header.riff_size = 36 + sizeof(data);
  strncpy(header.file_tag, "WAVE", 4);
  strncpy(header.fmt_label, "fmt ", 4);
  header.fmt_size = 16;
  header.audio_format = 1;
  header.channel_count = 1;
  header.sampling_rate = 8000;
  header.bytes_per_second = 2 * 8000;
  header.bytes_per_sample = 2;
  header.bits_per_sample = 16;
  strncpy(header.data_label, "data", 4);
  header.data_size = sizeof(data);
  file = fopen("test.wav", "wb");
  fwrite(&header, 1, 44, file);
  fwrite(data, 1, sizeof(data), file);
  fclose(file);

  /* test */
  read_data = (short*)malloc(sizeof(data));
  readWave("test.wav", read_header, read_data);
  for (i = 0; i < 44 && ((char*)(&header))[i] == read_header[i]; ++i);
  if (i == 44)
    printf("header read okay\n");
  else
    printf("header read failed\n");
  for (i = 0; i < 5 && data[i] == read_data[i]; ++i);
  if (i == 5)
    printf("data read okay\n");
  else
    printf("dataread failed\n");

  return 0;
}

/********** Main *********/

int main(void)
{
  lab1();
  lab2();
  lab3();
  lab4();

  return 0;
}