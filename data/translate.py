import requests
import sys

api = "deepL"
auth_key = "key here"
# api = "deepL_pro"

raw = sys.argv[1] + '\n'

def translate_LibreTranslate():
    url ="https://libretranslate.de/translate"
    params = { "q": raw, "source": "ja", "target": "en", "format": "text" }
    headers =  { "Content-Type": "application/json" }
    response = requests.post(url, params, headers)
    return response.json()["translatedText"]

def translate_DeepL():
    url = "https://api-free.deepl.com/v2/translate"
    params = { "auth_key": auth_key, "source_lang": "JA", "target_lang": "EN", "text": raw }
    response = requests.post(url, params)
    return response.json()["translations"][0]["text"]

def translate_DeepLpro():
    url = "https://api.deepl.com/v2/translate"
    params = { "auth_key": auth_key, "source_lang": "JA", "target_lang": "EN", "text": raw }
    response = requests.post(url, params)
    return response.json()["translations"][0]["text"]

write = raw + "LibreTranslate : \n" + translate_LibreTranslate()

if api == "deepL":
    write = write + "DeepL : \n" + translate_DeepL()
        
elif api == "deepL_pro":
    write = write + "DeepL : \n" + translate_DeepLpro()

with open("text.txt", 'w') as f:
    f.write(write)
