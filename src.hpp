# ifndef _SJTU_CPP_FINAL_SRC_HPP_
# define _SJTU_CPP_FINAL_SRC_HPP_

// This header provides the required Task/CPU interfaces (with include guards
// matching the problem statement) and implements the three scheduling methods.

// ----------------------------- task.hpp -----------------------------
# ifndef _SJTU_CPP_FINAL_TASK_HPP_
# define _SJTU_CPP_FINAL_TASK_HPP_

typedef unsigned int uint;

namespace sjtu {
    struct Task {
        uint task_id;
        uint priority;
        uint time;

        explicit Task(uint _task_id = 0, uint _priority = 0, uint _time = 0) {
            task_id = _task_id;
            priority = _priority;
            time = _time;
        }

        Task(const Task &rhs) {
            task_id = rhs.task_id;
            priority = rhs.priority;
            time = rhs.time;
        }

        ~ Task() = default;
    };

    enum CPUState { idle = 0, busy = 1 };
}
# endif

// ----------------------------- cpu.hpp -----------------------------
# ifndef _SJTU_CPP_FINAL_CPU_HPP_
# define _SJTU_CPP_FINAL_CPU_HPP_

# include <vector>
using namespace std;

typedef unsigned int uint;

namespace sjtu {
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() { state = idle; }

        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        int changePriority(uint task_id, uint priority) {
            for (auto &task: tasks)
                if (task.task_id == task_id) {
                    task.priority = priority;
                    return 1;
                }
            return 0;
        }

        virtual pair<CPUState, uint> run() = 0;
        virtual ~ CPU() = default;
    };

    // FCFS method based CPU.
    class CPU_FCFS : public CPU {
        // Index into tasks of the currently running task (if any),
        // maintained to ensure non-preemptive behavior for FCFS.
        int cur_index = -1;

        // Advance cur_index to the first available task with remaining time > 0.
        void pick_next_if_needed() {
            if (cur_index >= 0 && cur_index < (int)tasks.size() && tasks[cur_index].time > 0) return;
            // Find the earliest-added task with time > 0.
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time > 0) { cur_index = i; return; }
            }
            cur_index = -1;
        }

    public:
        pair<CPUState, uint> run() override {
            pick_next_if_needed();
            if (cur_index == -1) {
                state = idle;
                return make_pair(state, 0u);
            }

            // Execute one time unit on the current task.
            state = busy;
            uint running_id = tasks[cur_index].task_id;
            if (tasks[cur_index].time > 0) {
                --tasks[cur_index].time;
                if (tasks[cur_index].time == 0) {
                    // Remove finished task and adjust cur_index
                    tasks.erase(tasks.begin() + cur_index);
                    cur_index = -1; // force repick next tick
                }
            }
            return make_pair(state, running_id);
        }
    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
        // helper: choose index of task with smallest remaining time; tie -> earliest arrival (lowest index)
        int select_index() {
            int idx = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].time < tasks[idx].time) {
                    idx = i;
                }
                // tie-breaker by earlier arrival is naturally preserved by keeping first seen
            }
            return idx;
        }

    public:
        pair<CPUState, uint> run() override {
            int idx = select_index();
            if (idx == -1) {
                state = idle;
                return make_pair(state, 0u);
            }
            state = busy;
            uint running_id = tasks[idx].task_id;
            --tasks[idx].time;
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + idx);
            }
            return make_pair(state, running_id);
        }
    };

    // priority method based CPU.
    class CPU_PRIORITY : public CPU {
        // helper: choose index of task with highest priority (smallest priority value); tie -> earliest arrival
        int select_index() {
            int idx = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (idx == -1 || tasks[i].priority < tasks[idx].priority) {
                    idx = i;
                }
                // tie-breaker by arrival order preserved by first seen
            }
            return idx;
        }

    public:
        pair<CPUState, uint> run() override {
            int idx = select_index();
            if (idx == -1) {
                state = idle;
                return make_pair(state, 0u);
            }
            state = busy;
            uint running_id = tasks[idx].task_id;
            --tasks[idx].time;
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + idx);
            }
            return make_pair(state, running_id);
        }
    };
}

# endif // _SJTU_CPP_FINAL_CPU_HPP_

# endif // _SJTU_CPP_FINAL_SRC_HPP_

