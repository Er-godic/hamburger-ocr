#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

// https://muthu.co/all-tesseract-ocr-options/
// https://tesseract-ocr.github.io/tessdoc/ImproveQuality#image-processing

namespace HamburgerOCR
{

class OCR
{
	tesseract::TessBaseAPI* tessapi;

public:
	OCR()
	{
		tessapi = new tesseract::TessBaseAPI();

		// init tesseract-ocr with japanese
		if (tessapi->Init(NULL, "jpn")) 
		{
			printf("Could not initialize tesseract.\n");
			exit(-1);
		}
		
		tessapi->SetVariable("user_defined_dpi", "300");
    		tessapi->SetVariable("enable_new_segsearch", "0");
		tessapi->SetVariable("language_model_ngram_on", "0");
		tessapi->SetVariable("chop_enable", "1");
		tessapi->SetVariable("tessedit_enable_dict_correction", "1");
    		tessapi->SetVariable("textord_force_make_prop_words", "F");
    		tessapi->SetVariable("edges_max_children_per_outline", "40");
    		tessapi->SetVariable("preserve_interword_spaces", "1");
	}

	void processImage(const char* snapshot_path)
	{
		Pix *image = pixRead(snapshot_path);
		tessapi->SetImage(image);

		char* output = tessapi->GetUTF8Text();
		int length = strlen(HAMBURGER_DATA_PATH) + strlen(output) + 32;
		char* cmd = (char*) malloc(length * sizeof(char));
		snprintf(cmd, length, "(cd %s && python translate.py '%s')", HAMBURGER_DATA_PATH, output);
		system(cmd);
		delete[] output;
		free(cmd);
		pixDestroy(&image);
	}

	~OCR()
	{
		tessapi->End();
		delete tessapi;
	}
};

}
