#include <string>
#include "/usr/local/include/nlohmann/json.hpp"

class wordcount
{

public:
  wordcount() { all_ = nlohmann::json::array(); }
  ~wordcount() {}

  void readFile(const std::string& filename);
  void map();
  nlohmann::json reduce();

  void prettyPrint(const std::string outfile = "");
private:
  std::string makeMD5(std::string inp);
  nlohmann::json countWords(std::string inp);

  nlohmann::json all_;
};
