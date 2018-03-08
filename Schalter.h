#pragma once

#include "Warteschlange.h"
#include <vector>
#include <thread>
#include <random>
#include <iostream>

class Schalter
{
public:
	Schalter();
	~Schalter();

   template<typename T>
	void open(Warteschlange<T>& ws);

private:
	std::vector<std::thread> m_schalter;
	std::size_t m_aktiveSchalter;
};

template<typename T>
void Schalter::open(Warteschlange<T>& ws)
{
   ++m_aktiveSchalter;
	m_schalter.push_back( std::move(std::thread(
	[&ws]()->void
	{
		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<std::size_t> dist(100, 2000);

		while (ws.get_status()) {

			std::size_t random = dist(gen);
			std::this_thread::sleep_for(std::chrono::milliseconds(random));
			//std::unique_ptr<Besucher> B;
         T B;
			ws.dequeue(B);
			//B->status = false;
         //B.status = false;
         std::mutex m; m.lock();
         std::cout << "Besucher abgefruehstueckt!\n";
         m.unlock();
		}
	})));
}