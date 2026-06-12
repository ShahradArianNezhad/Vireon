#include <chrono>
#include <thread>




class Clock{
public:
  Clock(){
    start = std::chrono::high_resolution_clock::now();
  }

  double getLast(){
    return std::chrono::duration_cast<std::chrono::seconds>(last.time_since_epoch()).count();
  }
  double getStart(){
    return std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
  }
  
  static inline double now(){
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
  }

  double getDeltaTime(){
    std::chrono::duration<double> delta = std::chrono::high_resolution_clock::now()-last;
    return delta.count(); 
  }

  void setTimestamp(){
    if(hasReset){
      last=std::chrono::high_resolution_clock::now();
      hasReset=0;
    }else{
      start=last;
      last=std::chrono::high_resolution_clock::now();
    }
  }

  void reset(){
    start=std::chrono::high_resolution_clock::now();
    last=std::chrono::high_resolution_clock::now();
    hasReset=1;
  }

  void sleep(double s){
    long ms = s*1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

private:
  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point last;
  bool hasReset=0;
};
