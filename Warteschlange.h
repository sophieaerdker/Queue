#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>

template <typename T> 
class Warteschlange
{

public:
   Warteschlange(){
      m_maxElements = 10;
	   m_isActive = true;
    }
	Warteschlange(int n_maxElements);

	bool enqueue(const T & item);
	bool dequeue(T & result);

	void stop();
	bool get_status();

private:

	bool m_isActive;
	size_t m_maxElements;
	std::queue<T> m_queue;
	std::mutex m_mut;
	std::condition_variable cond_dequeue;
	std::condition_variable cond_enqueue;

};

template<typename T>
Warteschlange<T>::Warteschlange(int n_maxElements) {

	 m_maxElements = n_maxElements;
	 m_isActive = true;
}

template<typename T>
bool Warteschlange<T>::get_status() {
	 return m_isActive;
 }

 
template<typename T>
bool Warteschlange<T>::enqueue(const T & item) {

	 if (m_isActive == false) return false;

	 std::unique_lock<std::mutex> lock{ m_mut };

	 while (m_queue.size() == m_maxElements) { 
		 cond_enqueue.wait(lock);
		 //wait while queue is max size
	 }
	
	 m_queue.push(std::move(item));
    std::cout << "Hier steht wieder einer...\n";
    cond_dequeue.notify_one();
	 return true;
 }

 template<typename T>
 bool Warteschlange<T>::dequeue( T &result) {

	 std::unique_lock<std::mutex> lock{ m_mut };

	 while (m_queue.size() == 0 && m_isActive) {
		 cond_dequeue.wait(lock);
		 //wait while queue is empty
	 }

	 if (m_isActive == false) return false;

	 result = std::move(m_queue.front());
	 m_queue.pop();
    cond_enqueue.notify_one();
	 return true;

 }

template<typename T>
void Warteschlange<T>::stop() {

	m_isActive = false;
	// sodass keine neuen gestartet werden
	// andere threads beenden!
 }