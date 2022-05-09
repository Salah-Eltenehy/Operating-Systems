#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	// FILL ME IN
	/**Initialize all variables in the struct*/
	pthread_mutex_init(&station->mutex_train, NULL);
	pthread_cond_init(&station->passenger_seated, NULL);
	pthread_cond_init(&station->train_arrived, NULL);

	station->empty_seats = 0;
	station->passenger_on_train = 0;
	station->waiting_passengers = 0;
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	/**lock mutex to enter critical section*/
	pthread_mutex_lock(&station->mutex_train);
	/**Number of empty seats in the train*/
	station->empty_seats = count;
	/**check if there is empty seats and waiting passengers in the station
	*wake up all other waiting threads "train arrived"
	*/
	while(station->empty_seats > 0 && station->waiting_passengers > 0)
    {
        pthread_cond_broadcast(&station->train_arrived);
        /**unlock mutex and wait for passengers to enter the train*/
        pthread_cond_wait(&station->passenger_seated, &station->mutex_train);
    }
    station->empty_seats = 0;
    /**unlock the mutex
    ***all passengers enter the train
    **/
    pthread_mutex_unlock(&station->mutex_train);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&station->mutex_train);
	station->waiting_passengers += 1;

	while(station->passenger_on_train == station->empty_seats)
	{	
		/**Wait until trian arrived*/
        pthread_cond_wait(&station->train_arrived, &station->mutex_train);
    }

    station->passenger_on_train += 1;
    station->waiting_passengers -= 1;
    pthread_mutex_unlock(&station->mutex_train);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	pthread_mutex_lock(&station->mutex_train);
	station->passenger_on_train -= 1;
	station->empty_seats -= 1;

	if((station->empty_seats == 0) || (station->passenger_on_train) == 0)
        pthread_cond_signal(&station->passenger_seated);
    pthread_mutex_unlock(&station->mutex_train);
}


