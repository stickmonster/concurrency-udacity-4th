#include <iostream>
#include <random>
#include <future>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> youLock(_mutex);
    _condition.wait(youLock, [this] {return !_queue.empty();});
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> youLock(_mutex);
    std::cout << "The new message " << msg << " has been dispatched to the queue\n";
    _queue.push_back(std::move(msg));
    _condition.notify_one();


}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
    _msg_queue = std::make_shared<MessageQueue<TrafficLightPhase>>();
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
while (true){

std::this_thread::sleep_for(std::chrono::milliseconds(1));
auto msg = _msg_queue->receive();
 if (msg == TrafficLightPhase::green)
 return;
}
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 

    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread


void TrafficLight::cycleThroughPhases()

{
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(4000.0, 6000.0);
    int change_duration = distr(eng);

    
    while(1)
    {
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
         std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();   
      
           //check if the counter reaches the generated random value in miliseconds
  if(duration >= change_duration){

      if(_currentPhase == TrafficLightPhase::red)
        {
          _currentPhase = TrafficLightPhase::green;
        }
        else {
          _currentPhase = TrafficLightPhase::red;
        }
             auto change = _currentPhase;
		auto sent_change_future = std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send,  _msg_queue, std::move(change));
	    sent_change_future.wait();
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
        t1 = std::chrono::system_clock::now();
        int change_duration = distr(eng);

  

}
}
}

       // discarded code fragments I may return to if things go wrong
        //   auto last_update = std::chrono::system_clock::now();
       // long time_since_last_update = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - last_update).count();
        //t1 = std::chrono::high_resolution_clock::now();
        //int counter = 0;
   //std::default_random_engine eng;
     //   std::uniform_int_distribution<int> range(4000, 6000);
   
 //MessageQueue<TrafficLightPhase> _queue;
        //_queue.send(TrafficLightPhase &&msg) = (std::move(_currentPhase));
  //_queue.send(std::move(_currentPhase));
     








    





