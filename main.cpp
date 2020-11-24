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
  wc.prettyPrint("/tmp/a.json"); // Result from task A.

  wc.map();
  wc.prettyPrint("/tmp/b.json"); // Result from task B.

  nlohmann::json wordlist;
  wordlist["filename"] = argv[1];
  wordlist["word_counts"] = wc.reduce(); 

  std::cout << wordlist.dump(4) << std::endl;

  return 0;
}
