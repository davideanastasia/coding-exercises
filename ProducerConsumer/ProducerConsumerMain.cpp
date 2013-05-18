#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <iostream>
#include <thread>

#include <boost/lexical_cast.hpp>

// request container
template <typename Type>
class request
{
public:
    request() :
        item_available_(false)
    {}

    void push(const Type &x);
    void pop(Type* x);

private:
    std::queue< Type> queue_;
    std::mutex mutex_;
    volatile bool item_available_;
    std::condition_variable not_empty_;
};

template <typename Type>
void request<Type>::push(const Type &x)
{
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(x);
    item_available_ = true;
    lock.unlock();
    not_empty_.notify_all();
}

template <typename Type>
void request<Type>::pop(Type* x)
{
    std::unique_lock<std::mutex> lock(mutex_);
    not_empty_.wait(lock, [this]() { return item_available_;});

    *x = queue_.front();
    queue_.pop();

    item_available_ = queue_.size();
}


typedef request<std::string> RequestString;

// producer
class producer
{
public:
    producer(RequestString & _request)
        : request_(_request)
    {}

    void operator()()
    {
        size_t counter = 0;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            request_.push("something " + boost::lexical_cast<std::string>(++counter));

            std::stringstream ss;
            ss << "producer " << counter << std::endl;
            std::cout << ss.str() << std::flush;
        }
    }

private:
    RequestString & request_;
};

class consumer
{
public:
    consumer(RequestString & _request)
        : request_(_request)
    {}

    void operator()()
    {
        std::string temp_string;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            request_.pop(&temp_string);

            std::stringstream ss;
            ss << "consumer: " << temp_string << std::endl;
            std::cout << ss.str() << std::flush;
        }
    }

private:
    RequestString & request_;
};

int main()
{
    // request
    RequestString request_;

    // consumer
    std::thread first_consumer_{ consumer(request_) };
    std::thread second_consumer_{ consumer(request_) };

    // producer
    std::thread producer_{ producer(request_) };

    // join
    producer_.join();
    first_consumer_.join();
    second_consumer_.join();
}
