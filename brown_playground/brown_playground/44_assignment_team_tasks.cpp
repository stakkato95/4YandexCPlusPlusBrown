//
//  44_assignment_team_tasks.cpp
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

namespace team_tasks {

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
            TasksInfo info;
            
            vector<TaskStatus>::const_iterator start = begin(tasks.at(person));
            vector<TaskStatus>::const_iterator finish = lower_bound(start, end(tasks.at(person)), TaskStatus::IN_PROGRESS);
            info[TaskStatus::NEW] = (int)(finish - start);
            
            start = finish;
            finish = lower_bound(start, end(tasks.at(person)), TaskStatus::TESTING);
            info[TaskStatus::IN_PROGRESS] = (int)(finish - start);
            
            start = finish;
            finish = lower_bound(start, end(tasks.at(person)), TaskStatus::DONE);
            info[TaskStatus::TESTING] = (int)(finish - start);
            
            start = finish;
            info[TaskStatus::DONE] = (int)(end(tasks.at(person)) - start);
            
            return info;
        }
        
        // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
        void AddNewTask(const string& person) {
            tasks[person].insert(begin(tasks[person]), TaskStatus::NEW);
        }
        
        // Обновить статусы по данному количеству задач конкретного разработчика,
        // подробности см. ниже
        tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
            TasksInfo untouchedTasks;
            TasksInfo updated;
            int updateCount = task_count;
            
            TaskStatus status = *begin(tasks[person]);
            vector<TaskStatus>::iterator previousRangeEnd = begin(tasks[person]);
            while (status != TaskStatus::DONE && updateCount != 0) {
                vector<TaskStatus>::iterator newRangeEnd = upper_bound(previousRangeEnd, end(tasks[person]), status);
                int itemsCount = (int)(newRangeEnd - previousRangeEnd);
                
                bool allItemsInRangeChecked = updateCount >= itemsCount;
                int startOffset = allItemsInRangeChecked ? 0 : itemsCount - updateCount;
                updateCount -= allItemsInRangeChecked ? itemsCount : updateCount;
                if (!allItemsInRangeChecked) {
                    untouchedTasks[*previousRangeEnd] = startOffset;
                }
                
                for (vector<TaskStatus>::iterator it = next(previousRangeEnd, startOffset); it != newRangeEnd; ++it) {
                    TaskStatus newValue = transition.at(*it);
                    updated[newValue]++;
                    *it = newValue;
                }
                
                previousRangeEnd = newRangeEnd;
                
                if (newRangeEnd == end(tasks[person])) {
                    break;
                }
                
                status = *newRangeEnd;
            }
            
            
            if (previousRangeEnd != end(tasks[person]) && *previousRangeEnd != TaskStatus::DONE) {
                vector<TaskStatus>::iterator startOfDone = lower_bound(previousRangeEnd, end(tasks[person]), TaskStatus::DONE);
                for (vector<TaskStatus>::iterator it = previousRangeEnd; it != startOfDone;) {
                    vector<TaskStatus>::iterator upperBound = upper_bound(it, startOfDone, *it);
                    untouchedTasks[*it] = (int)(upperBound - it);
                    it = upperBound;
                }
            }
            
            return { updated, untouchedTasks };
        }
    private:
        const unordered_map<TaskStatus, TaskStatus> transition {
            { TaskStatus::NEW, TaskStatus::IN_PROGRESS },
            { TaskStatus::IN_PROGRESS, TaskStatus::TESTING },
            { TaskStatus::TESTING, TaskStatus::DONE },
            { TaskStatus::DONE, TaskStatus::DONE }
        };
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
}

int main44() {
    using namespace team_tasks;
    
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
