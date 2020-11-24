#!/usr/bin/python3

import json
import operator

def read_json(filename):
    with open(f'{filename}') as file:
        my_dict = json.load(file)
    return my_dict

def sort_and_report(wc):
    i = 0
    print("The 10 most common words were (descending order):")
    for w in sorted(wc, key=wc.get, reverse=True):
        print(i, ": ", w, "\t==>", wc[w])
        i += 1
        if i >= 10:
            break

if __name__ == '__main__':
    my_dict: object = read_json('/tmp/c.json')
    print("Analysed file: ", my_dict["file"])

    sort_and_report(my_dict["word_counts"])
