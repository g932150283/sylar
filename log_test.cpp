#include <stdio.h> 
#include <time.h>   
#include <iostream>
#include <boost/circular_buffer.hpp>
#include <thread>
#include <mutex>
#include <vector>
 
#include <stdio.h>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/SimpleLayout.hh>
#include "log4cpp/PropertyConfigurator.hh"
 
 
using namespace std;
std::mutex g_mutex;  // protects g_i
// 1. circular_buffer 在多线程下是否需要加锁
//    ---- thread1 write
//    ---- thread2 read
//    ---- thread3 write
 
 
boost::circular_buffer<int> cb(3);
 
void thread1_foo()
{
    int i = 0;
    while(i++ < 1000)
    {
        cb.push_back(i);
        {
            lock_guard<std::mutex> lock(g_mutex);
            cout << "thread1 write: " << i << endl;
        }
    }
}
 
void log_test()
{
 
    try
    {
        log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
    }
    catch (log4cpp::ConfigureFailure& f)
    {
        std::cout << "Configure Problem " << f.what() << std::endl;
 
    }
 
 
    log4cpp::Category& root = log4cpp::Category::getRoot();
    log4cpp::Category& sub3 = log4cpp::Category::getInstance(std::string("sub3"));
 
    for(int i = 0; i < 100; i++)
    {
        sub3.fatal("some test");
    }
 
}
 
void thread2_foo()
{
    int i = 0;
    while(i++ < 2000)
    {
        if(cb.empty())
        {
            continue;
        }
 
        int a = cb[0];
        cb.pop_front();
 
        {
 
            lock_guard<std::mutex> lock(g_mutex);
            cout<< "thread2 read : "<< a << endl;
        }
    }
}
 
 
 
 
#define LOGFILE "./test.log"
 
int main() {
    /*Setting up Appender, layout and Category*/
//    log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender",LOGFILE);//第一个参数是appender的名字，第二个是log文件的名字
//    log4cpp::Layout *layout = new log4cpp::SimpleLayout();
//    //log4cpp::Layout *layout = new log4cpp::BasicLayout();
//    log4cpp::Category& category = log4cpp::Category::getInstance("abc");
//   
//    appender->setLayout(layout);
//    category.setAppender(appender);
//    category.setPriority(log4cpp::Priority::INFO);
//   
//    /*The actual logging*/
//    category.info("This is for tracing the flow");
//    category.notice("This is to notify certain events");
//    category.warn("This is to generate certain warnings");
    log_test();
    return 0;
}