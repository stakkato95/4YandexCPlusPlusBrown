//
//  44_assignment_team_tasks_1.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/11/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo GetPersonTasksInfo(const string& person) const {
        
        
        return {};
    }
    
    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        
    }
    
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        return { };
    }
private:
    map<string, vector<TaskStatus>> tasks;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
    ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
    ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
    ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    
    //1 DONE
    tasks.AddNewTask("Ivan");
    for (int i = 0; i < 3; ++i) {
        tasks.PerformPersonTasks("Ivan", 1);
    }
    
    //4 TESTING
    for (int i = 0; i < 4; ++i) {
        tasks.AddNewTask("Ivan");
    }
    for (int i = 0; i < 2; ++i) {
        tasks.PerformPersonTasks("Ivan", 4);
    }
    
    //2 IN_PROGRESS
    for (int i = 0; i < 2; ++i) {
        tasks.AddNewTask("Ivan");
    }
    tasks.PerformPersonTasks("Ivan", 2);
    
    //3 NEW
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    tasks.PerformPersonTasks("Ivan", 4);
    
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
    
    return 0;
}
