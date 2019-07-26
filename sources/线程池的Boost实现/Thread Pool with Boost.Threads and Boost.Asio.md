## [A Thread Pool with Boost.Threads and Boost.Asio](http://progsch.net/wordpress/?p=71)
After spending some time being frustrated with the C++11 async/future stuff (not sure if i’m too dumb or the implementations aren’t that great yet), I dug up some old code and found this nice way of doing a thread pool with Boost.Threads (obviously) and Boost.Asio. Since the code is actually pretty short for what it does I’ll just dump it here:
```cpp
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
 
class ThreadPool;
 
// our worker thread objects
class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool; 
};
 
// the actual thread pool
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F>
    void enqueue(F f);
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::unique_ptr<boost::thread> > workers;
     
    // the io_service we are wrapping
    boost::asio::io_service service;
    boost::asio::io_service::work working;
    friend class Worker;
};
 
// all the workers do is execute the io_service
void Worker::operator()() { pool.service.run(); }
 
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads) : working(service)
{
    for(size_t i = 0;i<threads;++i)
        workers.push_back(
            std::unique_ptr<boost::thread>(
                new boost::thread(Worker(*this))
            )
        );
}
 
// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
    service.post(f);
}
 
// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    service.stop();
    for(size_t i = 0;i<workers.size();++i)
        workers[i]->join();
}
```
It’s essentially a wrapper around a io_service. The usage then looks something like this:
```cpp
// create a thread pool of 4 worker threads
ThreadPool pool(4);
 
// queue a bunch of "work items"
for(int i = 0;i<8;++i)
{
    pool.enqueue([i]
    {
        std::cout << "hello " << i << std::endl;
        boost::this_thread::sleep(
            boost::posix_time::milliseconds(1000)
        );
        std::cout << "world " << i << std::endl;
    });
}
```
which produces a funny mixture of garbled output clearly showing that the lambdas are executed in parallel.

This entry was posted in C++, Parallel Computing by JakobProgsch. Bookmark the permalink.
