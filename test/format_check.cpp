#include <string>

namespace format_check{
class Sample{public:
Sample(const std::string& value):value_(value){}
const std::string& value()const{return value_;}
private:
std::string value_;
};
}

int main(){format_check::Sample sample{"ci"};return sample.value().empty()?1:0;}
