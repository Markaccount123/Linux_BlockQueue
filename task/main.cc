#include"BlockQueue.hpp"
using namespace std;

void *comsumer_run(void* arg)
{
  BlockQueue *bq = (BlockQueue*)arg;
  while(1)
  {
    Task t;
    bq->Get(t);
    cout<<"comsumer: "<<t._x<<" + "<<t._y<<"="<<t.Run()<<endl;
  }
}

void *productor_run(void* arg)
{
  BlockQueue *bq = (BlockQueue*)arg;
  while(1)
  {
    int x = rand()%10 + 1;
    int y = rand()%100 + 1;
    Task t(x,y);
    bq->Put(t);
    cout<< "productor Task is :"<<x<<" + "<<y<< "= ?"<<endl;
    sleep(2);
  }
}



int main()
{
  BlockQueue *bq = new BlockQueue(5);//一个交易场所
  pthread_t c,p;
  pthread_create(&c,nullptr,comsumer_run,(void*)bq);
  pthread_create(&p,nullptr,productor_run,(void*)bq);

  pthread_join(c,nullptr);
  pthread_join(p,nullptr);

  delete bq;
  return 0;
}
