
#include <string>

namespace dur {

class File
{
public:
  File(const std::string& fullPath);

private:
  std::string        theName;
  time_t             theMtime;
  unsigned long long theSize;
};

}
