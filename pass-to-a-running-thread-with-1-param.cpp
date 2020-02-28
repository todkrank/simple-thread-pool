//https://stackoverflow.com/questions/22030027/c11-dynamic-threadpool
//https://en.cppreference.com/w/cpp/thread/condition_variable/wait
#include <iostream>
#include <thread>
#include <chrono>
#include <vector> //std::vector
#include <mutex> //std::mutex
#include <string> //std::string
#include <cstring> //strcmp()
#include <condition_variable> //std::condition_variable
#include <queue>
#include <functional>
#include <atomic>

const size_t MAXTHREADS(std::thread::hardware_concurrency());
std::vector<std::thread> threads;
std::queue<std::function<void()>> tasks;
std::atomic<bool> continue_condition(true);

std::mutex tasksmutex;
std::condition_variable condition;

void f(int );
void waitloop();
void pushtask(std::function<void()> );

int main(){
  int n(1);
  for(int i(0); i<MAXTHREADS; ++i){
    threads.push_back(std::thread(waitloop));
  }

  std::cout<<"Input \"0\" to stop execution of infinite loop and join all threads\n";
  while(continue_condition){
    std::cin>>n;
    if(!n){
      continue_condition=false;
      condition.notify_all();
      break;
    }
    std::function<void()> f_lambda=[n](){f(n);};
    pushtask(f_lambda);//store lambda instead
  }
  
  for(int i(0);i<MAXTHREADS;++i){
    threads.at(i).join();
  }
  
  return 0;
}

void f(int N){
  std::cout<<std::this_thread::get_id()<<" started"<<std::endl;
  for(int i(0);i<N;++i){
    std::cout<<i<<" ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout<<std::this_thread::get_id()<<" finished"<<std::endl;
  return;
}

void waitloop(){
  std::function<void()> task;
  while(continue_condition){
    {
      std::unique_lock<std::mutex> lock(tasksmutex);
      
      condition.wait(lock,[]{return !(tasks.empty()&&continue_condition);});
      if(!continue_condition)return;
      task=tasks.front();
      tasks.pop();
    }
    task();
  }
  return;
}

void pushtask(std::function<void()> newtask){
  {
     std::unique_lock<std::mutex> lock(tasksmutex);
     tasks.push(newtask);
  }
  condition.notify_one();
}
