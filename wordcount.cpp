#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>

#include "wordcount.h"

void wordcount::readFile(const std::string& filename)
{
  std::ifstream t;
  std::string line;
  int i=0;

  t.open(filename);
  while(t) 
    {
      nlohmann::json j_obj;
      std::getline(t, line);
      j_obj["file"] = filename;
      j_obj["l_no"] = i++;
      j_obj["text"] = line;
      all_.push_back(j_obj);
    }
  t.close();
}

std::string wordcount::makeMD5(std::string inp)
{
  u_char result[MD5_DIGEST_LENGTH];
  MD5((u_char*)inp.c_str(), inp.size(), result);

  std::ostringstream sout;
  sout<<std::hex<<std::setfill('0');
  for(long long c: result)
    sout<<std::setw(2)<<(long long)c;

  return sout.str();
}

nlohmann::json wordcount::countWords(std::string inp)
{
  std::vector<std::string> line;
  boost::split(line, inp, boost::is_any_of(" \t.,"));

  nlohmann::json word_count;
  for (int i=0; i<line.size(); i++)
    word_count[line[i]] = word_count.contains(line[i]) ? int(word_count[line[i]]) + 1 : 1;

  return word_count;
}

void wordcount::map()
{
  for (auto it = all_.begin(); it != all_.end(); ++it) {
    // Add md5sum
    (*it)["md5"] = makeMD5((*it)["text"]); // Note that this is lower case - probably ok!
    // Count words
    (*it)["word_counts"] = countWords((*it)["text"]);   
  } 
}

void wordcount::prettyPrint(const std::string outfile)
{
  if(outfile.size() == 0)
    std::cout << all_.dump(4) << std::endl;
  else
    {
      std::ofstream s;
      s.open(outfile);
      s << all_.dump(4) << std::endl;
      s.close();
    }  
}
