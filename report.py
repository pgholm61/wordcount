#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import json


def main():
    my_dict = read_json('/tmp/c.json')
    print(f"Analysed file: {my_dict['file']}")
    sort_and_report(my_dict["word_counts"])


def read_json(filename):
    try:
        with open(filename, 'r') as file:
            my_dict = json.load(file)
        return my_dict
    except:
        print(f"Could not read file: {filename}")
        sys.exit(1)


def sort_and_report(wc):
    i = 0
    print("The 10 most common words were (descending order):")
    for w in sorted(wc, key=wc.get, reverse=True):
        print(i, ": ", w, "\t==>", wc[w])
        i += 1
        if i >= 10:
            break


if __name__ == "__main__":
    main()
    sys.exit(0)
