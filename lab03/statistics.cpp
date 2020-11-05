#include "statistics.hpp"

#include <chrono>
#include <fstream>
#include <vector>

#include <pthread.h>

static pthread_mutex_t visits_mutex = PTHREAD_MUTEX_INITIALIZER;
static std::vector<std::time_t> visits;

void write_statistics() {
	pthread_mutex_lock(&visits_mutex);
	visits.push_back(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	pthread_mutex_unlock(&visits_mutex);
}

void save_statistics() {
	std::ofstream ofs("statistics/visits.txt", std::ofstream::out | std::ofstream::app);
	for (auto&& visit : visits) {
		ofs << visit << '\n';
	}
}
