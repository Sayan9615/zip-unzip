#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct
{
	char name[256];
	long size;
}file;
void archive(int cnt,const char *filename,char *filetype[])
{
	FILE* zip = fopen(filename, "wb");
	if (zip == NULL)
	{
		printf("fisierul nu s a deschis ");
		exit(-1);
	}
	for (int i = 0; i < cnt; i++)
	{
		FILE* f = fopen(filetype[i], "rb");
		if (zip == NULL)
		{
			printf("fisierul nu s a deschis ");
			exit(-1);
		}
		file type;
		strncpy(type.name, filetype[i], sizeof(type.name));
		fseek(f, 0, SEEK_END);
		type.size = ftell(f);
		rewind(f);
		fwrite(&type, sizeof(file),1, zip);
		char* buff = (char*)malloc(type.size);
		if (buff == NULL)
		{
			printf("copy vector allocation unsuccesfull");
			free(buff);
			fclose(f);
			exit(-1);
		}
		fread(buff, 1, type.size, f);
		fwrite(buff, 1,type.size, zip);
		free(buff);
		fclose(f);
	}

	fclose(zip);
}
void extractFiles(const char* archive_name)
{
	FILE* archive = fopen(archive_name,"rb");
	if (archive == NULL)
	{
		printf("the file doesn't open");
		fclose(archive);
		exit(-1);
	}
	fseek(archive, 0, SEEK_END);
	long fullsize = ftell(archive);
	fseek(archive, 0, SEEK_SET);
	while (ftell(archive) < fullsize)
	{
		file files;
		fread(&files, 1, sizeof(file), archive);
		FILE* exit_file = fopen(files.name, "wb");
		if (!exit_file)
		{
			printf("file problem");
			fclose(exit_file);
			exit(-1);
		}
		char* buff = (char*)malloc(files.size);
		if (!buff)
		{
			printf("buffer allocation failed");
			free(buff);
			fclose(exit_file);
			exit(-1);
		}
		fread(buff, 1, files.size, archive);
		fwrite(buff, 1, files.size, exit_file);
		free(buff);
		fclose(exit_file);
	}
	fclose(archive);
}
int main(int argc,char *argv[])
{
	if (argc < 3) {
		printf("Use for:\n");
		printf("Archive: %s archive <name_archive> <file1> <file2> ...\n", argv[0]);
		printf("Extract files: %s extract <name_archive>\n", argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "archive") == 0) 
	{
		if (argc < 4) 
		{
			printf("You have to say the name of archive.\n");
			return -1;
		}

		const char* archive_name = argv[2];
		char** files_to_archive = &argv[3]; 
		int file_count = argc - 3;

		archive(file_count, archive_name, files_to_archive);
		printf("Archive created succesfull %s.\n", archive_name);
	}
	else 
		if (strcmp(argv[1], "extract") == 0) 
	{
			if (argc != 3) 
		{
			printf("Please specify the name of archive.\n");
			return -1;
		}

		const char* archive_name = argv[2];
		extractFiles(archive_name);
		printf("Extraction succesfull.\n");
	}
	else 
	{
		printf("Unknown command. Used for:\n");
		printf("Archive command: %s archive <name_archive> <file1> <file2> ...\n", argv[0]);
		printf("Extract files: %s extract <name_archive>\n", argv[0]);
		return -1;
	}

	return 0;
}
