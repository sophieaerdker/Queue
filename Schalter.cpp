#include "Schalter.h"
#include <random>

Schalter::Schalter()
{
	m_aktiveSchalter = 0;
}

Schalter::~Schalter() {
	for (int i = 0; i < m_schalter.size(); ++i) {
		m_schalter[i].join();
	}
}