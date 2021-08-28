#include <iostream>
#include <vector>
#include <sstream>
#include <deque>

using namespace std;

//global vars:
    int workers_ID = 3300;
    int cars_ID = 6600;
    int stages_ID = 9900;

//Classes:
    class Car;

    class Worker
    {
        int worker_id;
        int worker_coefficient;
        bool occupancy;
        Car* onit_car;
    public:
        Worker() {worker_id = -1; worker_coefficient = -1; occupancy = false;}
        Worker(int _worker_id, int _worker_coefficient) {worker_id = _worker_id; worker_coefficient = _worker_coefficient; occupancy = false;}
        void set_worker_on(Car *_car);
        
        void print_worker();
        //getters:
        int get_worker_id() {return worker_id;}
        int get_worker_coefficient() {return worker_coefficient;}
        bool get_occupancy() {return occupancy;}
        Car* get_onit_car() {return onit_car;}
        void release() {occupancy = false; onit_car = nullptr;}
    };

    class Car
    {
        int car_id;
        int luxury_coefficient;
        Worker* in_hands_of;
        int time_left;
        bool _is_in_queue;
    public:
        Car() {car_id = -1; luxury_coefficient = -1; time_left = -1;}
        Car(int _car_id, int _luxury_coefficient) {car_id = _car_id;  luxury_coefficient = _luxury_coefficient; time_left = 0; _is_in_queue = true;}
        void set_car_on(Worker *_worker)
        {
            in_hands_of = _worker;
            time_left = luxury_coefficient * in_hands_of->get_worker_coefficient();
            _is_in_queue = false;
        }
        void reduce_time_left() {time_left -= 1;}
        bool car_visited_this_step(vector<int> vis_cars)    //prevents reproceed in <advance_time> function
        {
            for (int i = 0; i < vis_cars.size(); ++i)
                if (car_id == vis_cars[i])
                    return true;

            return false;
        }

        void show_car(bool print_t) {
            cout << "\tCar ID: " << car_id << endl;
            cout << "\tLuxury Coefficient: " << luxury_coefficient << endl;
            if (print_t)
                cout << "\tTime Left: "  << time_left << endl;
        }
        
        void release() {in_hands_of = nullptr; _is_in_queue = false;}
        //getters
        int get_car_id() {return car_id;}
        int get_luxury_coefficient() {return luxury_coefficient;}
        Worker* get_in_hands_of() {return in_hands_of;}
        int get_time_left () {return time_left;}
    };

    void Worker::set_worker_on(Car *_car) {
        onit_car =  _car;
        occupancy = true;
    }

    void Worker::print_worker() {
            cout << "\t\tWorker ID: " << worker_id << endl;
            // cout << "\t\t\tCoefficient: " << worker_coefficient << endl;  //erase this line at the end
            if (occupancy)
            {
                cout << "\t\tworking on this car:\n";
                onit_car->show_car(true);
            }
            else
                cout << "\t\t\tFree\n";
    }

    class Stage
    {
        int stage_id;
        vector<Worker* > worker_list;
        deque<Car* > cars_in_queue;
    public:
        Stage(int _stage_id, vector<Worker* > _worker_list){stage_id = _stage_id;worker_list = _worker_list;}
        
        //returns the index of first free worker in worker_list vector
        bool is_this_stg(int s_id) {return s_id == stage_id;}

        void print_stage(bool print_q) {
            cout << "Stage ID: " << stage_id << endl;
        //wrkrs
            cout << "\tworkers:\n";
            for (int j = 0; j < worker_list.size(); ++j)
            {
                worker_list[j]->print_worker();
            }
        //queue
            if (print_q)
            {
                cout << "\tCars in waiting queue:\n";
                for (int i = 0; i < cars_in_queue.size(); ++i)
                {
                    /*cout << "\t\t" << */cars_in_queue[i]->show_car(false)/* << endl*/;
                }
            }
        }

        int available_worker() {
            for (int i = 0; i < worker_list.size(); ++i)
                if (worker_list[i]->get_occupancy() == false)
                    return i;
            return -1;
        }

        void add_car_to_queue(Car *_car) {
            cars_in_queue.push_back(_car);  // deploy it on first stage queue ;;; not yet will this car be put in hands of a worker...
        }

        void dequeue() {
            cars_in_queue.pop_front();
        }

        int prior_car_idx() {
            int less_worker_id = 9999;
            int target_id;
            for (int i = 0; i < cars_in_queue.size(); ++i)
            {
                if (cars_in_queue[i]->get_in_hands_of() != nullptr && cars_in_queue[i]->get_in_hands_of()->get_worker_id() < less_worker_id)
                {
                    less_worker_id = cars_in_queue[i]->get_in_hands_of()->get_worker_id();
                    target_id = i;
                }
            }
            cout << "target_id" << target_id << endl;
            return target_id;
        }

        void print_queue()
        {
            for (int i = 0; i < cars_in_queue.size(); ++i)
                cars_in_queue[i]->show_car(false);
        }
        //getters:
        int get_stage_id() {return stage_id;}
        vector<Worker* > get_worker_list() {return worker_list;}
        deque<Car* > get_cars_in_queue() {return cars_in_queue;};
    };

    class Carwash
    {
    private:
        int time_passed = 0;
        deque<Car* > former_customers;
        vector<Stage* > stage_list;
    public:
        Carwash() { };
        void add_stage(string s);
        void add_car(int lux_co);
        void show_stage_info(int stg_id);
        void advance_time();
        void show_carwash();

        void print_cars();
        void finish();// before ending the program, be done with all cars

        bool all_cars_finished() { return (former_customers.size() == cars_ID - 6600) ? true : false; }
        //getters
        vector<Stage* > get_stage_list() { return stage_list; }
    };

int second_string_to_int(string cmd);

int main()
{
    Carwash pipeline;
    string command;
    string first_word;
    while(true)
    {
        cout << "Command: ";
        getline(cin, command);
        stringstream ss (command);
        ss >> first_word;

        if (first_word == "add_stage")
            pipeline.add_stage(command);
        else if (first_word == "add_car")
            pipeline.add_car(second_string_to_int(command)); //construct car and deploy it in first stage queue but don`t advance time
        else if (first_word == "show_stage_info")
            pipeline.show_stage_info(second_string_to_int(command));
        else if (first_word == "advance_time")
        {
            int time_steps = second_string_to_int(command);
            for (int i = 0; i < time_steps; ++i)
            {
                pipeline.advance_time();
                pipeline.show_carwash();  //dbg             
            }
        }
        else if (first_word == "show_carwash_info")
            pipeline.show_carwash();
        else if (first_word == "finish")
        {
            while (!pipeline.all_cars_finished())
            {
                pipeline.advance_time();
                pipeline.show_carwash();  //dgb         
            }
        }
        else
            cout << "Invalid command\n";

        cout << "----------------------------------------------------------\n";
    }
}

//funcs:
    void Carwash::add_car(int lux_co)
    {
        //deploy it on first stage queue
        stage_list[0]->add_car_to_queue(new Car(cars_ID, lux_co));        

        cars_ID++;
        cout << "\nOK\n";
    }

    void Carwash::add_stage(string s)
    {
        stringstream ss (s);
        string wcount;
        ss >> wcount >> wcount;

        int worker_count = stoi(wcount);
        string tc;
        vector<Worker* > _worker_list;

        for (int i = 0; i < worker_count; ++i)
        {
            ss >> tc;
            int temp_coefficient = stoi(tc);
            _worker_list.push_back(new Worker(workers_ID, temp_coefficient));
            workers_ID++;
        }
        stage_list.push_back(new Stage(stages_ID, _worker_list));
        stages_ID++;
        cout << "\nOK\n";
    }

    void Carwash::show_stage_info(int stg_id)
    {
        // find selected stage:
        int sel_stg_idx;
        for (int i = 0; i < stage_list.size(); ++i)
        {
            if (stage_list[i]->is_this_stg(stg_id))
            {
                sel_stg_idx = i;
                break;
            }
        }

        stage_list[sel_stg_idx]->print_stage(true);     //prints stage without it queue
        cout << "\nOK\n";
    }

    void Carwash::show_carwash()
    {
        //time
        cout << "time passed: " << time_passed << endl;
        
        for (int i = 0; i < stage_list.size(); ++i)
        {
            //queues
            if (i == 0)
                cout << "Cars waiting:\n";
            else
                cout << "Cars in waiting queue:\n";
            
            stage_list[i]->print_queue();
            
            //stages
            if (i == 0)
                cout << "Stages info: \n";
            
            stage_list[i]->print_stage(false);  //prints stage without its queue
        }

        //former customers:
        cout << "Cars finished:\n";
        for (int i = 0; i < former_customers.size(); ++i)
        {
            former_customers[i]->show_car(false); 
        }

        cout << "*******************************\n";
        cout << "\nOK\n";
    }

    int second_string_to_int(string cmd)
    {
        //fetches the second word of command as an int variable
        stringstream ss (cmd);
        string t;
        ss >> t >> t;
        return stoi(t);
    }

//where the magic happens!
    void Carwash::advance_time()
    {
        vector<int> visited_cars_ids;

        int free_worker_idx = -2;
        for (int i = stage_list.size() - 1; i >= 0; --i)
        {
            //first handle in-queue Cars
            while (stage_list[i]->get_cars_in_queue().size() != 0 && free_worker_idx != -1)
            {
                free_worker_idx = stage_list[i]->available_worker();

                if (free_worker_idx != -1)
                {
                    if (i != 0) //bc added cars can go to next stage in no time if workers are free
                        visited_cars_ids.push_back(stage_list[i]->get_cars_in_queue()[0]->get_car_id());

                    stage_list[i]->get_cars_in_queue()[0]->set_car_on(stage_list[i]->get_worker_list()[free_worker_idx]);
            
            
                    stage_list[i]->get_worker_list()[free_worker_idx]->set_worker_on(stage_list[i]->get_cars_in_queue()[0]);
                    
                    stage_list[i]->dequeue();
                }
            }

            //then cars in hands of workers (not sure if i`m commenting correctly)
            for (int k = 0; k < stage_list[i]->get_worker_list().size() && stage_list[i]->get_worker_list()[k]->get_occupancy() && 
                                !stage_list[i]->get_worker_list()[k]->get_onit_car()->car_visited_this_step(visited_cars_ids); ++k)
            {
                if (stage_list[i]->get_worker_list()[k]->get_onit_car()->get_time_left() == 0)
                {
                    if (i < stage_list.size() - 1)
                        free_worker_idx = stage_list[i + 1]->available_worker();

                    if (i == stage_list.size() - 1) //car finished all stages
                    {
                        stage_list[i]->get_worker_list()[k]->get_onit_car()->release();

                        //and push that car back to former_customers
                        former_customers.push_back(stage_list[i]->get_worker_list()[k]->get_onit_car());
                    }
                    else if (free_worker_idx != -1 && stage_list[i + 1]->get_cars_in_queue().size() == 0)   //next stage is available
                    {
                        stage_list[i]->get_worker_list()[k]->get_onit_car()->set_car_on(stage_list[i + 1]->get_worker_list()[free_worker_idx]);
                        stage_list[i + 1]->get_worker_list()[free_worker_idx]->set_worker_on(stage_list[i]->get_worker_list()[k]->get_onit_car());
                    }
                    else
                    {
                        //go to q
                        stage_list[i + 1]->add_car_to_queue(stage_list[i]->get_worker_list()[k]->get_onit_car());
                    }
                    stage_list[i]->get_worker_list()[k]->release();
                }
                else
                    stage_list[i]->get_worker_list()[k]->get_onit_car()->reduce_time_left();
            }
        }

        time_passed++;
        return;
    }
