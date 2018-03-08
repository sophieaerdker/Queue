#include "Besucher.h"
#include "Warteschlange.h"
#include "Schalter.h"
#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <memory>
#include <chrono>



void make_visitor(Warteschlange<std::shared_ptr<Besucher>> & ws) {

	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<std::size_t> dist(100, 200);

	while (ws.get_status()) {

		std::size_t random = dist(gen);
		std::this_thread::sleep_for(std::chrono::milliseconds(random));
		//std::shared_ptr<Besucher> b = std::make_unique<Besucher>();
		ws.enqueue(std::shared_ptr<Besucher>());
	}
}


//void cashier(Warteschlange<std::shared_ptr<Besucher>> & ws)
//{
//	std::random_device seed;
//	std::mt19937 gen(seed());
//	std::uniform_int_distribution<std::size_t> dist(100, 2000);
//
//	while (ws.get_status()) {
//
//		std::size_t random = dist(gen);
//		std::this_thread::sleep_for(std::chrono::milliseconds(random));
//		std::shared_ptr<Besucher> B;
//		ws.dequeue(B);
//		B->status = false;
//	}
//}


int main() {

   //Warteschlange<int> test;
	Warteschlange<std::shared_ptr<Besucher>> queue(100);
   //Warteschlange<Besucher> queue;
	Schalter schalter;

	std::thread t1{ [&queue]() { make_visitor(queue); } };
	schalter.open(queue);
	schalter.open(queue);
	schalter.open(queue);
   std::this_thread::sleep_for(std::chrono::seconds(10));
   
   queue.stop();
	t1.join();

	return 0;
}