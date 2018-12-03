#ifndef __THREADSAFE_QUEUE__
#define __THREADSAFE_QUEUE__

#include <deque>  
#include <mutex>  
#include <memory>  
#include <condition_variable>  
#include <vector>

template<typename T>  
class threadsafe_queue  
{  
public:  
    threadsafe_queue() {}  
    ~threadsafe_queue() {}  
  
    void push_front(T new_data)  {  
        std::lock_guard<std::mutex> lk(mut);            
        data_queue.push_front(new_data);          
        cond.notify_one();  
    }  


    void push(T new_data)  {  
        std::lock_guard<std::mutex> lk(mut);            
        data_queue.push_back(new_data);          
        cond.notify_one();  
    }  

    void push_all(std::vector<T> new_data)  {  
        std::lock_guard<std::mutex> lk(mut);            
        for(T t : new_data ) {
            //data_queue.push(std::move(t));          
            data_queue.push_back(t);          
        }
        cond.notify_one();  
    }  
 
    // void wait_and_pop(T& val) {  
    //     std::unique_lock<std::mutex> ulk(mut);                  
    //     cond.wait(ulk,[this]() { return !data_queue.empty(); });  
    //     val=std::move(data_queue.front());  
    //     data_queue.pop_front();  
    // }  

    // std::shared_ptr<T> wait_and_pop() {  
    //     std::unique_lock<std::mutex> ulk(mut);  
    //     cond.wait(ulk,[this]() { return !data_queue.empty(); });  
    //     std::shared_ptr<T> val(std::make_shared<T>(std::move(data_queue.front())));  
    //     data_queue.pop_front();  
    //     return val;  
    // }  

    bool try_pop(T& val) {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
        //val=std::move(data_queue.front());  
        val=data_queue.front();  
        data_queue.pop_front();  
        return true;  
    }  

    bool front(T& val) {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
       // val=std::move(data_queue.front()); 
        val=data_queue.front(); 
        return true;  
    }  

    bool pop() {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
        data_queue.pop_front();  
        return true;  
    }  

    bool equal_pop(T& val) {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
        T b = data_queue.front();
        if(val == b) {
            data_queue.pop_front();
            return true;
        }
        return false;  
    }  

    // std::shared_ptr<T> try_pop() {  
    //     std::shared_ptr<T> val;  
    //     std::lock_guard<std::mutex> lk(mut);  
    //     if(data_queue.empty())  
    //         return val;  
    //     val=std::make_shared<T>(std::move(data_queue.front()));  
    //     data_queue.pop_front();  
    //     return val;  
    // }  

    bool empty() {  
        std::lock_guard<std::mutex> lk(mut);  
        return data_queue.empty();  
    }  

    bool dump_and_clear(std::vector<T>& new_data, size_t max_size) {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
        //size_t now = 0;
        while( !data_queue.empty() ) {
            new_data.push_back(data_queue.front());
            data_queue.pop_front();
            if( new_data.size() >= max_size ) {
                break;
            }
        }
      //  data_queue.clear();
        return true;
    }  

    bool dump(std::vector<T>& new_data) {  
        std::lock_guard<std::mutex> lk(mut);  
        if(data_queue.empty())  
            return false;  
        for( auto k : data_queue ) {
            new_data.push_back(k);
        }

      //  data_queue.clear();
        return true;
    }  

    bool clear() {  
        std::lock_guard<std::mutex> lk(mut);  
        
        data_queue.clear();
        return true;
    }  

    size_t size() {  
        std::lock_guard<std::mutex> lk(mut);  
        return data_queue.size();  
    }  

private:  
    std::deque<T> data_queue;  
    std::mutex mut;  
    std::condition_variable cond;  
};  

#endif
