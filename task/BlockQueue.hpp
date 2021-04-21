#pragma once 

#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>


class Task
{
  public:
    int _x;
    int _y;

  public:
    Task()
    {}


    Task(int x,int y)
      :_x(x)
       ,_y(y)
    {}


    int Run()
    {
      return _x + _y;
    }

    ~Task()
    {}

};

class BlockQueue 
{
  public:
    bool isFull()
    {
      return q.size() == _cap;
    }

    bool isEmpty()
    {
      return q.empty();
    }

    void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }

    void UnlockQueue()
    {
      pthread_mutex_unlock(&lock);
    }

    void WakeUpComsumer()
    {
      std::cout<<"wake up comsumer"<<std::endl;
      pthread_cond_signal(&c_cond);
    }


    void WakeUpProductor()
    {
      std::cout<<"wake up productor"<<std::endl;
      pthread_cond_signal(&p_cond);
    }


    void ComsumerWait()
    {
      std::cout<<"comsumer wait ..."<<std::endl;
      pthread_cond_wait(&c_cond,&lock);
    }


    void ProductorWait()
    {
      std::cout<<"productor wait ..."<<std::endl;
      pthread_cond_wait(&p_cond,&lock);
    }


  public:
    BlockQueue(size_t cap)
      :_cap(cap)
    {
      pthread_mutex_init(&lock,nullptr);
      pthread_cond_init(&c_cond,nullptr);
      pthread_cond_init(&p_cond,nullptr);
    }

    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&p_cond);
      pthread_cond_destroy(&c_cond);
    }

    void Put(Task t)
    {
      //此时的阻塞队列是一个临界资源，所以访问是需要加锁的
      LockQueue();
      while(isFull())
      {
        WakeUpComsumer();
        ProductorWait();
      }
      q.push(t);
      UnlockQueue();
    }

    void Get(Task& t)
    {
      LockQueue();
      while(isEmpty())//这里不能使用if语句来进行判断，因为有可能会被异常唤醒
      {
        WakeUpProductor();
        ComsumerWait();
      }
      t = q.front();
      q.pop();
      UnlockQueue();
    }
  private:
    std::queue<Task> q;
    size_t _cap;
    pthread_mutex_t lock;
    pthread_cond_t p_cond; //在该条件变量下，生产者等
    pthread_cond_t c_cond;//在该条件变量下，消费者等

};
