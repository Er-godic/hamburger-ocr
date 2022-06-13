#include <stdio.h>
#include <stdlib.h>
#include "X11.hpp"
#include "ocr.hpp"

int main(int argc, char** argv)
{
	
	system("tput smcup");	
	HamburgerOCR::OCR ocr;
	HamburgerOCR::Xinit();
	
	int length = strlen(HAMBURGER_DATA_PATH) + 20;
	char* snapshot_path = (char*) malloc(length * sizeof(char));
	strcpy(snapshot_path, HAMBURGER_DATA_PATH);
	strcat(snapshot_path, "/snapshot.png");
	char* text_path =  (char*) malloc(length * sizeof(char));
	strcpy(text_path, HAMBURGER_DATA_PATH);
	strcat(text_path, "/text.txt");
	
	bool closed = false;
	char buffer[20];
	while(!closed)
	{	
		printf("# ");
		scanf("%10s", buffer);

		if (!strcmp(buffer, "quit")) 
			closed = true;
		
		else if (!strcmp(buffer,"sc"))
		{
			HamburgerOCR::Box sel = HamburgerOCR::makeSelection();
			sel.capture(snapshot_path);
			ocr.processImage(snapshot_path);

			int c;
			FILE *file;
			file = fopen(text_path, "r");
			if (file) 
			{
				while ((c = getc(file)) != EOF) putchar(c);
				fclose(file);
			}
		}

		else printf("# Invalid command.\n");
	}
	
	free(snapshot_path);
	free(text_path);
	system("tput rmcup");
	HamburgerOCR::Xdinit();
	return 0;
}
