# hamburger-ocr

This project is going to be my attempt at using ocr to read Moon Runes. Not sure if its ready for others yet, I built it in Arch Linux. 
Still some kinks to work out to make it user friendly. 
This was a project for myself so that I could play some japanese games but I might spruce things up over time. So it works well for ME.
But you might have to go through a few hoops depending on how shoddy my code is.

Dependencies are Leptonica, Tensorflow for the OCR 
https://github.com/DanBloomberg/leptonica 
https://github.com/tensorflow/tensorflow 
Make sure to add horizontal Japanese model file to the correct directory.

Need Python 3 and Python requests library to run script that uses some API for translation
In the script make sure to put in the API key if you choose to use DeepL. 

Once you run the ocr you have some commands you can type
"quit" or you can type
"sc" to make a box (rubberbanding) to select a screenshot for the ocr

Also make sure you have a working compositor up and running since the rubberbanding is done on a transparent window.
