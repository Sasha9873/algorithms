#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <mutex>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <condition_variable>
 
using namespace std::literals::chrono_literals;
 
class PetersonMutex
{
public:
    PetersonMutex()
    {
        want[0].store(false);
        want[1].store(false);
        waiting.store(0);
    }
 
    void lock(int threadId)
    {
		int other = 1 - threadId;
        want[threadId].store(true); // индикатор интереса текущего потока
        waiting.store(threadId); // говорим, что этот поток будет ждать, если понадобится
        /* Цикл ожидания. Мы находимся в этом цикле, если второй процесс выполняет свою 
        критическую секцию. Когда второй процесс выйдет из критической секции, выполнится
        процедура unlock(int threadId), флаг заинтересованности (want[other]) 
        станет равен false, и цикл закончится. */
        while (want[other].load() && waiting.load() == threadId) {
            // wait
        }
    }
 
    void unlock(int threadId) {
        want[threadId].store(false);
    }
 
private:
    std::array<std::atomic<bool>, 2> want; // флаги заинтересованности потоков
    std::atomic<int> waiting; // номер ждущего потока
};
 
 
class SpinWait {
  public:
    SpinWait() : state_(0) {
 
    }
 
    void lock() {
      while (state_.exchange(1) == 1) {
      }
    }
 
    void unlock() {
      state_.store(0);
    }
 
  private:
    std::atomic<int> state_;
};
 
/*class Mutex {
  public:
    Mutex() : state_(0) {
    }
 
    void lock() {
      while (state_.exchange(1) == 1) {
        state_.wait(1);
      }
    }
 
    void unlock() {
      state_.store(0);
      state_.notify_one();
    }
 
  private:
    std::atomic<int> state_;
};*/
 
class Barrier {
public:
  Barrier(int n) : need_wait_(n) {}
 
  void arrive() {
    std::unique_lock<std::mutex> lk(m_);
    --need_wait_;
    if (need_wait_ == 0) {
      wait_all_.notify_all();
      lk.unlock();
    } else {
      while (need_wait_ != 0) {
        wait_all_.wait(lk);
      }
    }
  }
private:
  std::mutex m_;
  std::condition_variable wait_all_;
  int need_wait_;
};


class shared_mutex{
public:
    shared_mutex(){}

    void lock() {
        std::unique_lock<std::mutex> lk(m_);
        while (k_shared_ != 0 && k_private_ != 0) {
            wait_all_.wait(lk);
        }
        ++k_private_;
    }
 
    void unlock() {
        wait_all_.notify_one();
        --k_private_;
    }

    void shared_lock() {
        std::unique_lock<std::mutex> lk(m_);
        while (k_private_ != 0) {
            wait_all_.wait(lk);
        }
        ++k_shared_;
    }
 
    void shared_unlock() {
        wait_all_.notify_one();
        --k_shared_;
    }

    ~shared_mutex(){}

private:
    std::mutex m_;
    std::condition_variable wait_all_;
    int k_shared_ = 0;
    int k_private_ = 0;
};
 

int main() {
  //std::atomic<int> x = 0;
  shared_mutex m1;
  shared_mutex m2;
  /* Dead lock example 
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; ++i) {
      m1.lock();
      m2.lock();
      ++x;  // mov. inc. mov
      m2.unlock();
      m1.unlock();
    }
  });
  std::thread t2([&]() {
    for (int i = 0; i < 1000000; ++i) {
      m2.lock();
      m1.lock();
      ++x;  // mov. inc.mov
      m1.unlock();
      m2.unlock();
    }
  });
  */
 /* Deadlock
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; ++i) {
      m1.lock();
      m1.lock();
      ++x;  // mov. inc. mov
      m1.unlock();
    }
  });
*/
  /*std::vector<int> v;
  std::thread t1([&]() {
    try {
      for (int i = 0; i < 1000000; ++i) {
        std::lock_guard<std::mutex> lk(m1);
        v.at(1000);
 
      }
    } catch (std::out_of_range& e) {
      std::cout << e.what() << std::endl;
    }
  });
  std::thread t2([&]() {
    for (int i = 0; i < 1000000; ++i) {
      std::lock_guard<std::mutex> lk(m1);
    }
  });
  t1.join();
  t2.join();*/
  //std::cout << x << std::endl;

  int x = 0;

  std::thread t1([&]() {
    for (int i = 0; i < 1000000; ++i) {
      m1.lock();
      ++x;  
      m1.unlock();
    }
  });
  std::thread t2([&]() {
    for (int i = 0; i < 1000000; ++i) {
      m1.lock();
      ++x;  // mov. inc. mov
      m1.unlock();
    }
  });

  t1.join();
  t2.join();
  std::cout << x << std::endl;

  return 0;
}