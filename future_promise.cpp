#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <iostream>
#include <optional>


template <typename T>
struct ControlBlock {
    ControlBlock(T* p): ptr(p), promise_counter(0), future_counter(0) {}
 
    ~ControlBlock() {
        delete ptr;
    }
 
    bool promise_counter;
    bool future_counter;
    T* ptr;
    //std::optional<T> future_ptr;
    std::condition_variable wait_set_;
    std::mutex m_;
};
 
template<typename T>
class Promise;
 
template <typename T>
class Future {
public:
    Future() : cb(nullptr) {}
    Future(const Promise<T>& ptr) : cb(ptr->cb), value_(ptr->value_){}

    T get() {
		std::unique_lock<std::mutex> lock(m_);
      	while (!cb->future_counter) {
        	cb->wait_set_.wait(lock);
      	}

      	return value_;
    }
 
    ~Future() {
        cb->future_counter = 0;
    }
 
    friend WeakPtr<T>;

private:
    T value_;
    ControlBlock<T>* cb;
};
 
template<typename T>
class Promise {
public:
	Promise() : cb(nullptr) {}
    Promise(T* ptr) : cb(new ControlBlock<T>(ptr)) {
    	future_ = Future(*this);
    	cb->future_counter = 1;
    	cb->promise_counter = 1;
    }
 
    void set_value(T value){
    	value_ = value;
    	cb->wait_set_.notify_all();
    }

    Future<T> get_future() {
    	return future_;
    }
 
    ~WeakPtr() {
 
    }
private:
 
    ControlBlock<T>* cb;
    T value_;
    Future<T> future_;
};
