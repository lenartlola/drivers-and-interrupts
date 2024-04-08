#https://wiki.osdev.org/PS/2_Keyboard

# Get the set keycodes from osdef wiki

import sys
import ssl

import requests

from bs4 import BeautifulSoup

URL = "https://wiki.osdev.org/PS/2_Keyboard"

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
def get_set1():
    i = 0
    f = open("set1.html", "r")
    set1_html = f.read()
    soup = BeautifulSoup(set1_html, "html.parser")
    rows = soup.find_all('tr')
    data = []
    print("struct set set1[] = {")
    for row in rows:
        cols = row.find_all('td')
        cols = [ele.text.strip() for ele in cols]
        data.append([ele for ele in cols if ele])
    for items in data:
        if not items:
            continue
        for item in items:
            if i % 2 == 0:
                new_line = "\t{"
                new_line += item
                new_line += ", \""
            else:
                if "pressed" in item:
                    item = item.replace(" pressed", '')
                    new_line += item
                    new_line += "\", "
                    new_line += "PRESSED},"
                    print(new_line)
            i += 1
        #print(items)
    #print(data)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python get_set.py <set>");
        sys.exit(1)
    if sys.argv[1] == "set1":
        get_set1()

