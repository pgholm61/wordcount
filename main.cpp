#include <iostream>
#include <fstream>
#include <thread>

#include "wordcount.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <text filename>" << std::endl;
        return 1;
    }

    WordCount wc;
    if(wc.readFile(argv[1]))  {
        std::cerr << "Could not read file " << argv[1] << ". Exiting" <<std::endl;
        return 1;
    }

    wc.map();

    json wordlist;
    wordlist["file"] = argv[1];
    //  wordlist["word_counts"] = wc.reduce();

    wc.reduce();
    wordlist["word_counts"] = wc.getWordList();

    std::ofstream out("/tmp/c.json");
    out << wordlist.dump(4) << std::endl;
    out.close();  // Result from task C

    return 0;
}
