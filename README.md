## Carwash_Simulation
A GUI-less simulation of a pipeline carwash.
This project is an introduction to OOP(to be more specific, object composition).
 
### An outline of the program:
- Contains multiple stages(which you add while running the program) in which have one or more workers
- You also are able to add cars to the simulation
- Each worker, car and stage have IDs

### Details stipulated in the descrption file:
- Cars and workers also have coeffitients that later in the code will determine how many time steps it takes to wash a car is in hands of a worker(time_left = car_coefficient * worker_coefficient)
- A worker can only wash a single car at a time
- When a car is added, it will be deployed in the initial queue before the first stage
- It takes 1 time step, every time a car advances on the next stage OR queue
- Although if the next stage has at least one availabe worker and its queue is empty, we can have our car under the hands of that worker in merely one step of time
- The carwash keeps a list of customers that go through its pipeline
  
### Commands:
##### The first word of each line is the command and the follwings are each`s arguments.
- add_car luxury_coefficient
- add_stage number_of_workers first_worker_coefficient second_worker_coefficient ...
- advance_time time_steps
- show_stage_info stage_id
- show_carwash_info
- finish
    
##### "finish" keeps advancing the time to the point that all cars reach the end of the pipeline carwash.
##### All the other commands` functionality is apparant by their expression.
  
  

