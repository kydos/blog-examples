#include <iostream>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono> 

#include "gen/oxymeter_DCPS.hpp"

using namespace dds::domain;
using namespace dds::topic;
using namespace dds::pub;
using namespace ac::blog;

std::ostream& operator << (std::ostream& os, const Oxymetry& o) {
  os << "("
     << o.deviceId() << ", "
     << o.spO2() << ", "
     << o.bpm() << ", "
     << o.pleth()
     <<")";
  return os;
}

Oxymetry get_oximetry(const std::string& deviceId) {
  int spO2 = random() % 100;
  int bpm = random() % 80;
  int pleth = random() % 100;

  return Oxymetry(deviceId, spO2, bpm, pleth); 
} 
const int UPDATE_PERIOD = 1; //sec

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "USAGE:\n\t oxymeter <device-id>" << std::endl;
    return 1;
  }

  std::string deviceId(argv[1]);

  DomainParticipant dp(0);
  Topic<Oxymetry> topic(dp, "TOxymetry");
  Publisher pub(dp);

  DataWriter<Oxymetry> dw(dp, topic);
  
  // Initialize random number generation with a seed
  srandom(clock());
  
  // To keep things simple keep writing samples forever...
  while (true) {
    auto o = get_oximetry(deviceId);
    dw.write(o);
    std::cout << o << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(UPDATE_PERIOD));
  }
  return 0;
}
