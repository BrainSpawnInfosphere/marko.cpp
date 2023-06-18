#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

class SockAddr {
  public:
  void filter(const std::string& addr) {
    regex proto("(udp|tcp|unix)\\:\\/\\/([a-z,A-Z,\\d,\\/,.,*,_,-,:]+)");
    smatch m;
    regex_search(addr, m, proto);

    if (m.size() == 0) {
      cout << "No matches!!" << endl;
    }
    else if (m[1] == "unix"){
      cout << "unix path: " << m[2] << endl;
    }
    else if (m[1] == "tcp" || m[1] == "udp") {
      cout << "inet" << endl;
      regex ipport("([a-z,A-Z,\\d,\\/,.,*]+):([*,\\d]+)");
      smatch mm;
      string ss = m[2];
      regex_search(ss, mm, ipport);

      if (mm.size() != 3) cout << "error " << mm.size() << endl;
      string ip;
      if (mm[1] == "*") ip = "ANYADDR_IN";
      else ip = mm[1];
      cout << ip << ':' << mm[2] << endl;
    }
  }
};

int main()
{
    string s{"udp://*:3000"};

    SockAddr sa;
    sa.filter(s);
    sa.filter("tcp://1.2.3.4:5000");
    sa.filter("unix:///bob/here");

  //  regex r("(udp|tcp|unix)\\:\\/\\/([a-z,A-Z,\\d,\\/,.,*]+):([\\d]+)");
  //  smatch m;
  //  regex_search(s, m, r);

    // for each loop
    // for (auto x : m)
    //     cout << x << endl;

    // cout << m[1] << endl;
    // cout << m[2] << endl;
    // cout << m[3] << endl;

    return 0;
}