#include <atomic>
#include <fstream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#include <nlohmann/json.hpp>

using nlohmann::json;


struct LineStruct {
    std::string filename;
    int line_no;
    std::string text;
};

struct CountStruct {
    std::string filename;
    int line_no;
    std::string text;
    std::string md5;
    json count_elem;
};


class WordCount
{

public:
    WordCount():
        n_threads_(8) {}
    ~WordCount();

    int readFile(const std::string& filename);
    void map();
    void reduce();
    //  json reduce();
    json getWordList();

private:

    void readWorker(const std::string& filename);
    void mapWorker();
    void reduceWorker();


    std::queue<LineStruct> lines_;
    std::queue<CountStruct> counts_;
    std::unordered_map<std::string, int> word_count_;

    std::mutex line_mutex_;
    std::mutex count_mutex_;
    std::mutex all_words_mutex_;

    std::ifstream in_;
    std::thread read_thread_;

    int n_threads_;
    std::vector<std::thread> threads_;
    std::atomic<int> readDone_ {0};

};
