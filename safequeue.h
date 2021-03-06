#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

/*a thread-safe queue*/

template<typename T, int MAXSIZE>
class SafeQueue
{
public:
	SafeQueue():abortRequest(0){}
	void pop(T& data);
	void push(const T& data);
    void clear();
	void abort();
	unsigned int size();
private:
	std::mutex qMutex;
	std::queue<T> q;
	std::condition_variable condNotFull;
	std::condition_variable condNotEmpty;
	int abortRequest;
};

template<typename T, int MAXSIZE>
void SafeQueue<T, MAXSIZE>::push(const T& data)
{
	std::unique_lock<std::mutex> lock(qMutex);
	if(abortRequest == 1){
		return;
	}
	/*if queue was full, wait */
	if(q.size() >= MAXSIZE){
		condNotFull.wait(lock, [this]{return q.size() < MAXSIZE || abortRequest;});
	}
	q.push(data);
	condNotEmpty.notify_one();
}

template<typename T, int MAXSIZE>
void SafeQueue<T, MAXSIZE>::pop(T& data)
{
	std::unique_lock<std::mutex> lock(qMutex);
	if(abortRequest == 1){
		return;
	}
	/*if queue was empty, wait*/
	if(q.empty()){
		condNotEmpty.wait(lock, [this]{return !q.empty() || abortRequest;});
	}
    if(abortRequest)
        return;
	data = q.front();
	q.pop();
	condNotFull.notify_one();
}

template<typename T, int MAXSIZE>
void SafeQueue<T, MAXSIZE>::clear()
{
    std::unique_lock<std::mutex> lock(qMutex);
    q.swap(SafeQueue<T, MAXSIZE>());
}

template<typename T, int MAXSIZE>
void SafeQueue<T, MAXSIZE>::abort()
{
	/*set abortRequest and send signal*/
	std::unique_lock<std::mutex> lock(qMutex);
	abortRequest = 1;
	condNotFull.notify_all();
	condNotEmpty.notify_all();
}

template<typename T, int MAXSIZE>
unsigned int SafeQueue<T, MAXSIZE>::size()
{
	std::unique_lock<std::mutex> lock(qMutex);
	return q.size();
}

#endif
