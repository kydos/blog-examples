#include <iostream>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono> 

#include "gen/oxymeter_DCPS.hpp"

using namespace dds::domain;
using namespace dds::topic;
using namespace dds::sub;
using namespace dds::sub::cond;
using namespace ac::blog;
using namespace dds::core::cond;
using namespace dds::sub::status;

std::ostream& operator << (std::ostream& os, const Oxymetry& o) {
  os << "("
     << o.deviceId() << ", "
     << o.spO2() << ", "
     << o.bpm() << ", "
     << o.pleth() 
     <<")";
  return os;
}

int main(int argc, char* argv[]) {
  DomainParticipant dp(0);
  Topic<Oxymetry> topic(dp, "TOxymetry");
  Subscriber sub(dp);
  
  DataReader<Oxymetry> dr(dp, topic);
  
  // Create the waitset
  WaitSet ws;
  // Create a ReadCondition for our data reader 
  // and configure it for new data
  ReadCondition rc(dr, DataState::new_data());
  // attache the condition
  ws += rc;

  while (true) {
    ws.wait();
    auto samples = dr.read();
    
    std::for_each(samples.begin(),
		  samples.end(),
		  [](const dds::sub::Sample<Oxymetry>& s) {
		    std::cout << s.data() << std::endl;
		  });     
  }
  return 0;
}
