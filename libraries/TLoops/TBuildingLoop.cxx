#include "TBuildingLoop.h"

#include <chrono>
#include <thread>

//#include "RawDataQueue.h"
//#include "RawDataQueue.h"
//#include "TDetectorEnv.h"
#include "TGEBEvent.h"
#include "TGRUTOptions.h"
#include "TNSCLEvent.h"

#include "TDataLoop.h"

//#include "TMode3.h"

ClassImp(TBuildingLoop)

TBuildingLoop *TBuildingLoop::Get(std::string name,TDataLoop *input) {
  if(name.length()==0 && input) {
    name = "build_loop";
  }
  TBuildingLoop *loop = (TBuildingLoop*)StoppableThread::Get(name);
  if(!loop && input)
    loop = new TBuildingLoop(name,input);
  return loop;
}

TBuildingLoop::TBuildingLoop(std::string name,TDataLoop *input) :
  StoppableThread(name),input_source(input) {

  SetBuildWindow(1000);
  event_start = 0;
  //next_event = new TUnpackedEvent;
}


TBuildingLoop::~TBuildingLoop(){
  //delete next_event;
}


int TBuildingLoop::Pop(std::vector<TRawEvent> &event) {
  return output_queue.Pop(event);
}


bool TBuildingLoop::Iteration(){
  TRawEvent event;

  int error = input_source->Pop(event);///input_queue.Pop(event);
  if(error<0) {
    // Stop if the parent has stopped and the queue is empty.
    return input_source->IsRunning();
  }


  CheckBuildWindow(&event);
  next_event.push_back(event);

  return true;
}

bool TBuildingLoop::CheckBuildWindow(TRawEvent *event) {
  long timestamp = event->GetTimestamp();
  if(timestamp > event_start + build_window) {
    //next_event->Build();
    output_queue.Push(next_event);
    next_event.clear(); // = new TUnpackedEvent;
    event_start = timestamp;
    return true;
  }
  return false;
}