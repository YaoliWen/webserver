// encode UTF-8

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<thread>
#include<condition_variable>
#include<mutex>
#include<vector>
#include<queue>
#include<future>

class ThreadPool{
private:
    bool m_stop;//是否停止线程池
    std::vector<std::thread>m_thread;//储存所有线程
    std::queue<std::function<void()>>tasks;//存储任务序列的队列
    std::mutex m_mutex;//互斥锁
    std::condition_variable m_cv;//条件变量

public:
    explicit ThreadPool(size_t threadNumber):m_stop(false){
        for(size_t i=0;i<threadNumber;++i)
        {
            //将threadNumber个线程依次加入线程池的vector中，每个线程都等待着执行任务队列中的任务
            m_thread.emplace_back(
                [this](){
                    for(;;)
                    {
                        std::function<void()>task;
                        { 
                            //锁
                            std::unique_lock<std::mutex>lk(m_mutex);
                            m_cv.wait(lk,[this](){ return m_stop||!tasks.empty();});
                            //当wait被唤醒，先拿锁，若线程停止或任务不为空时进行下一步，若不满足解锁继续等待
                            if(m_stop&&tasks.empty()) return;//退出线程
                            //复制任务
                            task=std::move(tasks.front());
                            
                            tasks.pop();
                        }
                        //执行任务
                        task();
                    }
                }
            );
        }
    }

    ThreadPool(const ThreadPool &) = delete; //拷贝构造函数，并且取消默认父类构造函数
    ThreadPool(ThreadPool &&) = delete; // 拷贝构造函数，不允许右值引用

    ThreadPool & operator=(const ThreadPool &) = delete;
    ThreadPool & operator=(ThreadPool &&) = delete;

    ~ThreadPool(){
        {
            //锁
            std::unique_lock<std::mutex>lk(m_mutex);
            //设置停止标志
            m_stop=true;
        }
        //唤起所有线程
        m_cv.notify_all();
        //依次回收线程
        for(auto& threads:m_thread)
        {
            threads.join();
        }
    }
    //添加一个任务到任务队列
    template<typename F,typename... Args>
    auto submit(F&& f,Args&&... args)->std::future<decltype(f(args...))>{
        //包装函数
        auto taskPtr=std::make_shared<std::packaged_task<decltype(f(args...))()>>( 
            std::bind(std::forward<F>(f),std::forward<Args>(args)...)
        );
        {
            // 锁
            std::unique_lock<std::mutex>lk(m_mutex);
            if(m_stop) throw std::runtime_error("submit on stopped ThreadPool");//如果线程池已经停止还继续添加，则抛出异常
            // 在任务队列中放入任务
            tasks.emplace([taskPtr](){ (*taskPtr)(); });
        }
        m_cv.notify_one();
        return taskPtr->get_future();

    }
};

#endif //THREADPOOL_H
