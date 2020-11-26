#include "wordcount.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>


/********************************/
/*     Service functions        */
/********************************/

namespace {
//-----------------------------------------------------------------------------
std::string makeMD5(const std::string& inp)
//-----------------------------------------------------------------------------
{
    u_char result[MD5_DIGEST_LENGTH];
    MD5((u_char*)inp.c_str(), inp.size(), result);

    std::ostringstream out;
    out << std::uppercase << std::hex << std::setfill('0');

    for(long c: result)
        out << std::setw(2) << c;

    return out.str();
}


//-----------------------------------------------------------------------------
json countWords(const std::string& inp)
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

} // end anonymous namespace

/********************************/
/*     Member functions         */
/********************************/

//-----------------------------------------------------------------------------
WordCount::~WordCount()
//-----------------------------------------------------------------------------
{
    in_.close();
}

//-----------------------------------------------------------------------------
void WordCount::readWorker( const std::string& filename)
//-----------------------------------------------------------------------------
{
    std::string line;
    int i=0;
    // Read file line by line and put content in queue of structs
    while(in_)
    {
        std::getline(in_, line);

        // Adding to lines too, to test threading.
        std::lock_guard<std::mutex> lock(line_mutex_);
        lines_.push({filename, i, line});
        i++;
    }
    readDone_.fetch_add(1);
}

//-----------------------------------------------------------------------------
int WordCount::readFile(const std::string& filename)
//-----------------------------------------------------------------------------
{
    in_.open(filename);
    if(!in_.good())
        return 1;

    read_thread_ = std::thread(&WordCount::readWorker, this, filename);

    return 0;
}

//-----------------------------------------------------------------------------
void WordCount::mapWorker()
//-----------------------------------------------------------------------------
{
    while (true) {
        LineStruct line;
        {
            // Complicated because I don't know when readFile has finished...
            std::lock_guard<std::mutex> lock(line_mutex_);
            if (readDone_.load() == 0) {
                if (lines_.empty())
                    continue;
            }
            else {
                if (lines_.empty())
                    return;
            }

            // Pop one line from queue
            line = lines_.front();
            lines_.pop();
        }

        std::string md5 = makeMD5(line.text);
        json count = countWords(line.text);

        std::lock_guard<std::mutex> lock(count_mutex_);
        counts_.push({line.filename, line.line_no, line.text, md5, count});
    }
}

//-----------------------------------------------------------------------------
void WordCount::map()
//-----------------------------------------------------------------------------
{
    for(int i = 0; i < n_threads_; i++)
        threads_.push_back(std::thread(&WordCount::mapWorker, this));
}


//-----------------------------------------------------------------------------
void WordCount::reduceWorker()
//-----------------------------------------------------------------------------
{
    while(true)
    {
        std::lock_guard<std::mutex> lock(count_mutex_);
        if(counts_.empty())
            return;

        CountStruct c = counts_.front();
        counts_.pop();

        {
            std::lock_guard<std::mutex> lock2(all_words_mutex_);
            for (auto w = c.count_elem.begin(); w != c.count_elem.end(); ++w)
            {
                if(word_count_.find(w.key()) == word_count_.end()) // unordered_map doesn't have contains()
                    word_count_[w.key()] = int(w.value());
                else
                    word_count_[w.key()] += int(w.value());
            }
        }
    }
}

//-----------------------------------------------------------------------------
void WordCount::reduce()
//-----------------------------------------------------------------------------
{
    read_thread_.join();
    for(int i = 0; i < n_threads_; i++)
        threads_[i].join();

    threads_.clear();
    for(int i = 0; i < n_threads_; i++)
        threads_.push_back(std::thread(&WordCount::reduceWorker, this));
}

//-----------------------------------------------------------------------------
json WordCount::getWordList()
//-----------------------------------------------------------------------------
{
    for(int i = 0; i < n_threads_; i++)
        threads_[i].join();

    return json(word_count_);
}

