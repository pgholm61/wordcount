#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>

#include "wordcount.h"

//-----------------------------------------------------------------------------
int WordCount::readFile(const std::string& filename)
//-----------------------------------------------------------------------------
{
  std::ifstream in(filename);
  if(!in.good())
    return 1;

  std::string line;
  int i=0;
  // Read file line by line and put content in array of json objs
  while(in) 
    {
      json j_obj;
      std::getline(in, line);
      j_obj["file"] = filename;
      j_obj["l_no"] = i++;
      j_obj["text"] = line;
      all_.push_back(j_obj);
    }
  in.close();

  return 0;
}


//-----------------------------------------------------------------------------
std::string WordCount::makeMD5(std::string inp)
//-----------------------------------------------------------------------------
{
  u_char result[MD5_DIGEST_LENGTH];
  MD5((u_char*)inp.c_str(), inp.size(), result);

  std::ostringstream out;
  out << std::hex << std::setfill('0');
  for(long long c: result)
    out << std::setw(2) << (long long)c;

  return out.str();
}


//-----------------------------------------------------------------------------
json WordCount::countWords(std::string inp)
//-----------------------------------------------------------------------------
{
  std::vector<std::string> line;
  boost::split(line, inp, boost::is_any_of(" \t.,"));


  json word_count;
  for (int i=0; i<line.size(); i++)
    {
      // Clean words (i.e. line[i]) of special chars "clinging" to them (is this part of the task?)
      line[i].erase(std::remove_if(line[i].begin(), line[i].end(), [](char c) { return !std::isalnum(c); }), line[i].end());
      if(line[i].empty()) // ignore/don't count empty strings
	continue;
      word_count[line[i]] = word_count.contains(line[i]) ? int(word_count[line[i]]) + 1 : 1;
    }
  return word_count;
}


//-----------------------------------------------------------------------------
void WordCount::map()
//-----------------------------------------------------------------------------
{
  for (auto it = all_.begin(); it != all_.end(); ++it) {
    // Add md5sum
    (*it)["md5"] = makeMD5((*it)["text"]); // Note that this is lower case - probably ok!
    // Count words
    (*it)["word_counts"] = countWords((*it)["text"]);   
  } 
}


//-----------------------------------------------------------------------------
json WordCount::reduce()
//-----------------------------------------------------------------------------
{
  json allwords;

  for (auto it = all_.begin(); it != all_.end(); ++it) 
    {
      json j_obj = (*it)["word_counts"];

      for (auto w = j_obj.begin(); w != j_obj.end(); ++w) 
	allwords[w.key()] = allwords.contains(w.key()) ? int(allwords[w.key()]) + 1 : int(1);
    }

  return allwords;
}


//-----------------------------------------------------------------------------
void WordCount::printAll(const std::string outfile)
//-----------------------------------------------------------------------------
{
  if(outfile.size() == 0)
    std::cout << all_.dump(4) << std::endl;
  else
    {
      std::ofstream out(outfile);
      out << all_.dump(4) << std::endl;
      out.close();
    }  
}
