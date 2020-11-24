#include <iostream>

#include "wordcount.h"

int main(int argc, char *argv[])
{
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <text filename>" << std::endl;
    return 1;
  }

  wordcount wc;

  wc.readFile(argv[1]);
  wc.prettyPrint("/tmp/a.json");

  wc.map();
  wc.prettyPrint("/tmp/b.json");

  return 0;
}
