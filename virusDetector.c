#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus virus;
typedef struct link link;

struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

struct link {
    virus *v;
    link *next;
};

void PrintHex(unsigned char *buffer, long length);

unsigned char* getFileData(char * fileName);

int getFileSize(char * fileName);

int readVirus(int index, unsigned char * buffer, struct virus ** viruses, int virusesIndex);

void printVirus(virus *v);

/* Print the data of every link in list. Each item followed by a newline character. */
void list_print(link *virus_list); 

/* Add a new link with the given data to the list 
  (either at the end or the beginning, depending on what your TA tells you),
  and return a pointer to the list (i.e., the first link in the list).
  If the list is null - create a new entry and return a pointer to the entry. */
link* list_append(link* virus_list, virus* data); 
 
/* Free the memory allocated by the list. */
void list_free(link *virus_list);

/* The detect_virus function compares the content of the buffer byte-by-byte with the 
 * virus signatures stored in the virus_list linked list. */
void detect_virus(unsigned char *buffer, link *virus_list, unsigned int size);
void detect_virus_f(unsigned char *buffer, link *virus_list, unsigned int size);

/* compare if 'contained' is contained in text */
int isVirusThere(unsigned char* text, virus* v, unsigned int size);

int main(int argc, char **argv) {
  int index = 0;
  int virusesIndex = 0;
  unsigned char * buffer = getFileData("signatures");
  struct virus * viruses[10];
  link * virus_list = NULL;
  
  /* the core of the algorithm */
  /* iteration over viruses - saves them in a linkedlist */
  while(virusesIndex < 10)
  {
    index = readVirus(index, buffer, viruses, virusesIndex);
    virusesIndex++;
  }
  virusesIndex =  0;
  
  while(virusesIndex < 10)
  {
    virus_list = list_append(virus_list, viruses[virusesIndex]);
    virusesIndex++;
  }
  
  /* loads the checked file data */
  unsigned char * checkedFileBuffer = getFileData("infected");
  
  if(argc > 1 && strcmp(argv[1], "-f") == 0)
    detect_virus_f(checkedFileBuffer, virus_list, getFileSize("infected"));
  else
    detect_virus(checkedFileBuffer, virus_list, getFileSize("infected"));
  
  list_free(virus_list);
    
  /* terminate */
  free(buffer);
  free(checkedFileBuffer);
  return 0;
}

void PrintHex(unsigned char *buffer, long length)
{
  int i = 0;
  while(i != length)
  {
    printf("%02X ", buffer[i]);
    i++;
  }
}

int getFileSize(char * fileName)
{
  FILE *fp;
  long size = 0;
  if(!(fp = fopen(fileName, "r")))
  {
    fputs ("File error",stderr); 
    exit (1);
  }
  
  /* obtain file size: */
  fseek (fp , 0 , SEEK_END);
  size = ftell (fp);
  rewind (fp);
  
  fclose (fp);
  return (size < 10240) ? size : 10240;
}

unsigned char* getFileData(char * fileName)
{
  FILE *fp;
  long size = 0;
  unsigned char * buffer;
  size_t result = 0;
  if(!(fp = fopen(fileName, "r")))
  {
    fputs ("File error",stderr); 
    exit (1);
  }
  
  /* obtain file size: */
  fseek (fp , 0 , SEEK_END);
  size = ftell (fp);
  rewind (fp);
  
  /* allocate memory to contain the whole file: */
  buffer = (unsigned char*) malloc (sizeof(unsigned char)*size);
  if (buffer == NULL) {
    fputs ("Memory error",stderr);
    exit (2);
  }
  
  /* copy the file into the buffer: */
  result = fread (buffer,1,size,fp);
  if (result != size) {
    fputs ("Reading error",stderr); 
    exit (3);
  }
  
  fclose (fp);
  return buffer;
}

int readVirus(int index, unsigned char * buffer, struct virus ** viruses, int virusesIndex)
{
  int size = 0;
  size = 256 * buffer[index] + buffer[index + 1] - 18;
  index += 2;
  
  viruses[virusesIndex] = malloc(sizeof(struct virus) + size + 16);
  
  viruses[virusesIndex]->length = size;
  
  memcpy(viruses[virusesIndex]->name, buffer + index, 16);
  index += 16;
  
  memcpy(viruses[virusesIndex]->signature, buffer + index, viruses[virusesIndex]->length);
  index += viruses[virusesIndex]->length;
  
  return index;
}

void printVirus(virus *v)
{
  printf("Virus name: %s\n", v->name);
  printf("Virus size: %d\n", v->length);
  printf("signature:\n");
  
  PrintHex((unsigned char *)v->signature, v->length);
  
  printf("\n\n");
}

void list_print(link *virus_list)
{
  if(virus_list)
  {
    printVirus(virus_list->v);
    list_print(virus_list->next);
  }
}

link* list_append(link* virus_list, virus* data)
{
  link* temp = malloc(sizeof *temp);
  temp->v = data;
  temp->next = NULL;
  if(!virus_list)
    return temp;
  else
  {
    link * currentLink = virus_list;
    while(currentLink->next)
      currentLink = currentLink->next;
    
    currentLink->next = temp;
  }
  return virus_list;
}

void list_free(link *virus_list)
{
  free(virus_list->v);
  if(virus_list->next)
    list_free(virus_list->next);
  free(virus_list);
}

void detect_virus_f(unsigned char *buffer, link *virus_list, unsigned int size)
{
  link *currentLink = virus_list;
  int virusIndex = 0;
  int chosenOne = -1;
  virus *chosenVirus;
  int virusStatingPoint[10];
  int min = size + 1;
    
  while(currentLink)
  {
    virusStatingPoint[virusIndex] = isVirusThere(buffer, currentLink->v, size);
    if(virusStatingPoint[virusIndex] < min && virusStatingPoint[virusIndex] != -1)
    {
      min = virusStatingPoint[virusIndex];
      chosenOne = virusIndex;
      chosenVirus = currentLink->v;
    }
    
    virusIndex++;
    currentLink = currentLink->next;
  }
  if(chosenOne != -1)
  {
    printf("Virus Found:\n");
    printf("starting byte location: %d\n", virusStatingPoint[chosenOne]);
    printf("virus name: %s\n", chosenVirus->name);
    printf("size of the virus signature: %d\n", chosenVirus->length);
    printf("\n");
  }
}

void detect_virus(unsigned char *buffer, link *virus_list, unsigned int size)
{
  link *currentLink = virus_list;
  int virusStatingPoint;
  
  while(currentLink)
  {
    if((virusStatingPoint = isVirusThere(buffer, currentLink->v, size)) != -1)
    {
      printf("Virus Found:\n");
      printf("starting byte location: %d\n", virusStatingPoint);
      printf("virus name: %s\n", currentLink->v->name);
      printf("size of the virus signature: %d\n", currentLink->v->length);
      printf("\n");
    }
	
    currentLink = currentLink->next;
  }
}

int isVirusThere(unsigned char* text, virus* v, unsigned int size)
{
  unsigned int startingLetter = 0;
  while(v->length <= size - startingLetter )
  {
    if(memcmp(text + startingLetter, v->signature, v->length) == 0)
      return startingLetter;
    startingLetter++;
  }
  return -1;
}