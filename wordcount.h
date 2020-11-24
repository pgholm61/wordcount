#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

class WordCount
{

public:
  WordCount() { all_ = json::array(); }
  ~WordCount() {}

  int readFile(const std::string& filename);
  void map();
  json reduce();

  void printAll(const std::string outfile = "");
private:
  std::string makeMD5(std::string inp);
  json countWords(std::string inp);

  json all_;
};
