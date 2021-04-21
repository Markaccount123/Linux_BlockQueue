#include"BlockQueue.hpp"
using namespace std;

void *comsumer_run(void* arg)
{
  BlockQueue *bq = (BlockQueue*)arg;
  while(1)
  {
    int n = 0;
    bq->Get(n);
    cout<<"comsumer date is :"<<n<<endl;
  }
}

void *productor_run(void* arg)
{
  BlockQueue *bq = (BlockQueue*)arg;
  int count = 0;
  while(1)
  {
    //int date = rand() % 10 + 1;//随机的产生1到10的数
    count = count % 5 + 1; // 产生1-5顺序的数
    bq->Put(count);
    cout<< "productor date is :"<<count<<endl;
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
