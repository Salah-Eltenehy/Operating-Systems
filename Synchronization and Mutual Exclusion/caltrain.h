#include <pthread.h>

struct station {
	// FILL ME IN
	/**Mutex to synchronize threads*/
	pthread_mutex_t mutex_train;
	/**Condition variables to communicate between threads*/
	pthread_cond_t train_arrived;
	pthread_cond_t passenger_seated;
	/**Number of empty seats in the train*/
	int empty_seats;
	/**Number of passengers in the train*/
	int passenger_on_train;
	/**Number of passengers waiting in the station*/
	int waiting_passengers;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
