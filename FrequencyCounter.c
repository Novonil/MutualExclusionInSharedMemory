/*
===============================================================================
<< -----------------------------  NOVONIL DAS  ----------------------------- >>
===============================================================================

Source Code :	FrequencyCounter.c
Input Files :	LongFile1.txt LongFile2.txt LongFile3.txt LongFile4.txt New.c Test1.txt
OutputFile :	Result.txt
Compile :		cc FrequencyCounter.c
Execute :		./a.out 6 LongFile1.txt LongFile2.txt LongFile3.txt LongFile4.txt New.c Test1.txt Result.txt

================================================================================
*/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/signal.h>

void ReadAndWriteFile(char fileToRead[], char fileToWrite[], int processId, int processNumber);

int main(int argc, char *argv[])
{
	int i, pid, status, wid, p;
	//Number of processes
	char c = *argv[1];

	//Check if the number of files is less than 1 and stop executing if needed.
	if ((c - '0') <= 0)
	{
		printf("Number of files not enough.\n");
		return;
	}
	//Check if n is equal to the number of files provided and stop executing if needed.
	else if ((c - '0') != (argc - 3))
	{
		printf("Number of input files and the count of files doesnot match.\n");
		return;
	}

	//For each file create a child process which reads from the input files and writes to the output file.
	for (i = 0; i < c - '0'; i++)
	{
		if ((pid = fork()) == 0)
		{
			ReadAndWriteFile(argv[i + 2], argv[argc - 1], getpid(),i+1);
		}
	}
	while ((wid = wait(&status)) > 0);
	printf("The children processes has completed writing\n");
	printf("The parent process is terminating\n");
	exit(1);
}



void ReadAndWriteFile(char fileToRead[], char fileToWrite[], int processId, int processNumber)
{
	char c, ch;
	int asc, i, p;
	int frequency[26] = { 0 };

	FILE* readerPointer;
	FILE* writerPointer;

	//Check if the file to read exists.
	if (access(fileToRead, F_OK) == -1)
	{
		printf("File %s doesnot exist all files before this has been written to the output\n", fileToRead);
		exit(1);
	}

	//Read the file
	readerPointer = fopen(fileToRead, "r");
	if (readerPointer == NULL)
	{
		printf("File %s failed to open\n", fileToRead);
		exit(1);
	}
	//Calculate the frequency of each alphabet.
	else
	{
		while ((c = fgetc(readerPointer)) != EOF)
		{
			asc = (int)c;
			if (asc >= 65 && asc <= 90)
			{
				frequency[asc - 65] = frequency[asc - 65] + 1;
			}
			if (asc >= 97 && asc <= 122)
			{
				frequency[asc - 97] = frequency[asc - 97] + 1;
			}
		}
		fclose(readerPointer);
	}

	//Write to the file
	writerPointer = fopen(fileToWrite, "a");
	if (writerPointer == NULL)
	{
		printf("File %s failed to open\n", fileToWrite);
		exit(1);
	}
	else
	{
		fprintf(writerPointer, "===========================================================================\n");
		fprintf(writerPointer, "      Process Number : %d File Name : %s Process Id : %d\n", processNumber, fileToRead,processId);
		fprintf(writerPointer, "===========================================================================\n");

		for (i = 0; i < 26; i++)
		{
			ch = i + 65;
			fprintf(writerPointer, "%c : %d \n", ch, frequency[i]);
		}

		fclose(writerPointer);
	}
	exit(1);
}