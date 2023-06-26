#include <iostream>
#include <pthread.h> // Library for threads
#include <vector>
#include <chrono> //  Library for runtime calculation
#include <unistd.h>
#include <fstream>
#include <random> // Library for generating random values of times
#include <string>
#include <semaphore.h> // Library for implementing semaphore

using namespace std;
using namespace chrono;

int P = 0; // value of the total number of passengers
int C = 0; // value of the total number of cars
int lamb1 = 0;
int lamb2 = 0;
int K = 0;

vector<string> ret;               // Stores all the output strings
// This array stores the availability of a particular car at an index
// value = 1 then the car is available
// value = 0 then the car is unavailable
int *cars_availability;
pthread_mutex_t mtx_writing;      // invoked when adding of a string to vector array is done
sem_t *sem_array;                 // invoked when writing to the array is done in a thread
double total_time_passengers = 0; // This adds up all the times execution of the passengers to complete their journey
double total_time_cars = 0;       // This adds up all the times execution of the cars to complete their journey

struct data_to_passenger_func
{
  int passenger_number; // passenger number
  pthread_t *cars;      // availability array of the cars
  pthread_attr_t attr;
};
typedef struct data_to_passenger_func parampass;

struct data_to_car_func
{
  int car_number;       // car number
  int passenger_number; // passenger number
  double time_sleep;    // Value of sleep time
};
typedef struct data_to_car_func paramcar;

void *cars(void *para)
{

  auto start = high_resolution_clock::now(); // Clock starts
  string adde;
  auto *d = (paramcar *)para;
  pthread_mutex_lock(&mtx_writing);
  time_t raw_time;
  struct tm *timeinfo;
  time(&raw_time);
  timeinfo = localtime(&raw_time);
  adde = "Car " + to_string(d->car_number) + " accepts Passenger " + to_string(d->passenger_number) + "'s tour\n";
  ret.push_back(adde);
  adde = "Passenger " + to_string(d->passenger_number) + " started riding at " + to_string(timeinfo->tm_hour) + ":" + to_string(timeinfo->tm_min) + ":" + to_string(timeinfo->tm_sec) + "\n";
  ret.push_back(adde);
  adde = "Car " + to_string(d->car_number) + " is riding Passenger " + to_string(d->passenger_number) + "\n";
  ret.push_back(adde);

  // Entry section for car thread
  pthread_mutex_unlock(&mtx_writing);

  // CS for car thread
  usleep(d->time_sleep*1000);

  // Exit section for car thread
  auto stop = high_resolution_clock::now(); // Time of EXIT
  auto timees = duration_cast<microseconds>(stop - start);
  total_time_cars += (double)timees.count();

  // Remainder section
  pthread_mutex_lock(&mtx_writing);
  adde = "Car " + to_string(d->car_number) + " has finished Passenger " + to_string(d->passenger_number) + "'s tour\n";
  ret.push_back(adde);
  time(&raw_time);
  timeinfo = localtime(&raw_time);
  adde = "Passenger " + to_string(d->passenger_number) + " has finished riding at " + to_string(timeinfo->tm_hour) + ":" + to_string(timeinfo->tm_min) + ":" + to_string(timeinfo->tm_sec) + "\n";
  ret.push_back(adde);
  pthread_mutex_unlock(&mtx_writing);

  return nullptr;
}

void *passenger(void *param)
{

  int seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  exponential_distribution<double> t_1(1.0 / lamb1);
  exponential_distribution<double> t_2(1.0 / lamb2);

  double t1 = t_1(generator);
  double t2 = t_2(generator);

  string adde;
  auto start = high_resolution_clock::now(); // Clock starts
  auto *dat = (parampass *)param;
  time_t raw_time;
  struct tm *timeinfo;

  pthread_mutex_lock(&mtx_writing);
  time(&raw_time);
  timeinfo = localtime(&raw_time);
  adde = "Passenger " + to_string(dat->passenger_number) + " enters the museum at " + to_string(timeinfo->tm_hour) + ":" + to_string(timeinfo->tm_min) + ":" + to_string(timeinfo->tm_sec) + "\n";
  ret.push_back(adde);
  pthread_mutex_unlock(&mtx_writing);

  // Entry section of the passenger thread
  for (int j = 0; j < K; j++)
  {
    int index_car = 0;
    sem_wait(sem_array); // Blocked this section inorder to remove race conditions for array value choosing
    for (int i = 0; i < C; i++)
    {
      if (cars_availability[i] == 1)
      {
        index_car = i;
        cars_availability[i] = 0;
        break;
      }
    }

    auto *a = (paramcar *)calloc(1, sizeof(paramcar));
    a->passenger_number = dat->passenger_number;
    a->car_number = index_car + 1;
    a->time_sleep = t2;
    pthread_create(&dat->cars[index_car], &dat->attr, cars, a);
    pthread_join(dat->cars[index_car], nullptr);
    cars_availability[index_car] = 1;
    sem_post(sem_array); // Incrementing the sem_array value

    // CS of passenger thread
    if (t1 > t2)
    usleep((int)(t1-t2)*1000);

    // Exit section of passenger
  }

  // Remainder section of the passenger
  auto stop = high_resolution_clock::now(); // Time of EXIT
  auto timees = duration_cast<microseconds>(stop - start);
  pthread_mutex_lock(&mtx_writing);
  time(&raw_time);
  timeinfo = localtime(&raw_time);
  adde = "Passenger " + to_string(dat->passenger_number) + " exits the museum at " + to_string(timeinfo->tm_hour) + ":" + to_string(timeinfo->tm_min) + ":" + to_string(timeinfo->tm_sec) + "\n";
  ret.push_back(adde);
  pthread_mutex_unlock(&mtx_writing);
  total_time_passengers += (double)timees.count();
  return nullptr;
}

int main()
{

  // Reading input data
  ifstream in;
  in.open("input.txt", ios_base::in);
  in >> P >> C >> lamb1 >> lamb2 >> K;

  // initializing the mutex variables
  pthread_mutex_init(&mtx_writing, nullptr);

  // initializing semaphore variable to cars count
  sem_array = sem_open("/arraysem", O_CREAT | O_EXCL, 0644, C);

  // initializing the cars availability array
  cars_availability = (int *)calloc(C, sizeof(int));
  for (int i = 0; i < C; i++)
    cars_availability[i] = 1;

  pthread_t passengers[P]; // This is the thread pool of passengers
  pthread_t cars[C];       // This is the thread pool of cars
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  for (int i = 0; i < P; i++)
  {
    auto *dat = (parampass *)calloc(1, sizeof(parampass));
    dat->passenger_number = i + 1;
    dat->cars = cars;
    dat->attr = attr;
    pthread_create(&passengers[i], &attr, passenger, dat);
  }

  for (int i = 0; i < P; i++)
  {
    pthread_join(passengers[i], nullptr);
  }

  cout << "Total avg time of execution of passengers is in secs : " << total_time_passengers / P << endl;
  cout << "Total avg time of execution of cars is in secs : " << total_time_cars / C << endl;

  // Writing to the output file
  // strings are written to output file in the strings generated order
  ofstream op;
  op.open("output.txt", ios_base::out);
  for (const auto & i : ret)
  {
    op << i;
  }
  return 0;
}


