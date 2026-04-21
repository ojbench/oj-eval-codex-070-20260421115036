# Problem 070 - CPU 任务调度问题

**ACMOJ Problem ID**: 1448

## Table of Contents

- [Problem 070 - CPU 任务调度问题](#problem-070-cpu-任务调度问题)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [Submission Format](#submission-format)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Implement CPU task scheduler: support task arrival, scheduling policies (FIFO, Priority, SJF). Multi-class design.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **1448. CPU 任务调度问题** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/070/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

**时间限制：1000ms，空间限制：256MB**

**请一定认真阅读题目最后的注意事项！**

**在本题中，你只需要实现 `src.hpp` 中 `TODO` 标识符后面的部分，其余代码文件（包括 `task.hpp` 与 `main.cpp`）请不要改动。**


在你写代码的时候，你可能同时打开了 Clion、QQ、浏览器等许多应用，可是你的计算机可能只有一个 CPU，这时候就需要 CPU 进行进程调度来解决每个进程所发送的任务请求。一般来说，CPU 会维护一个任务列表包含需要完成的任务，并且在每个时间段都会选择下面两项中的一项：

- 做一个任务列表中需要完成的任务；
- 空闲（此时当前任务列表中的所有任务都已完成）。

所谓 CPU 任务调度，就是安排 CPU 在每个时间段内做哪一个任务。也就是说，你的计算机可能前一个单位时间在执行 Clion 的编译任务，后一个单位时间在执行 QQ 的实时聊天任务。在计算机中，由于时间段非常小，所以你通常感觉不到这种调度的存在。

言归正传，我们提供如下三种 CPU 任务调度问题的方法，而你需要实现这三个方法。

**FCFS 方法**：CPU 在下个时间段（时间单位）执行先加入任务列表的任务。

**SRTF 方法**：CPU 在下个时间段（时间单位）执行任务列表中**剩余时间最少**的任务；如果有多个任务剩余时间相同，则执行先加入任务列表的那个。

**Priority 方法**：CPU 在下个时间段（时间单位）内执行**优先级最高**的任务；如果有多个任务优先级相同，则执行先加入任务列表的那个。

### 题目解释

具体来说，我们在 `task.hpp` 中提供了一个 `Task` 类来描述任务（在下文代码中 `uint` 代表 `unsigned int`）：

```c++
# ifndef _SJTU_CPP_FINAL_TASK_HPP_
# define _SJTU_CPP_FINAL_TASK_HPP_

typedef unsigned int uint;

namespace sjtu {
    // Description for each task:
    //   task_id: uint, unique for each task;
    //   priority: uint;
    //   time: uint, (remaining) time of the task.
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

    // CPUState: idle and busy.
    enum CPUState {
        idle = 0, busy = 1
    };
}
# endif
```

其中，

- `task_id` 是每个任务的编号，为一个 `unsigned int` 范围内的**非 0 整数**。
- `priority` 是每个任务的优先级，为一个 `unsigned int` 范围内的整数，**优先级越高的任务 `priority` 值越小**；
- `time` 是每个任务需要的时间，为一个 `unsigned int` 范围内的整数，最小单位为 1。

然后，我们提供了一个 `CPU` 基类：

```c++
# ifndef _SJTU_CPP_FINAL_CPU_HPP_
# define _SJTU_CPP_FINAL_CPU_HPP_

# include <vector>
# include "task.hpp"

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    // CPU base class, modifications is not allowed.
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() {
            state = idle;
        }

        // Add a new task.
        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        // Change the priority of one process, return 1 if success and return 0 if fail.
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
        // TODO: complete the FCFS method.

    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
        // TODO: complete the SRTF method.

    };

    // priority method based CPU.
    class CPU_PRIORITY : public CPU {
        // TODO: complete the priority method.

    };
}

# endif
```

其中，

- `state` 表示目前 CPU 的状态，有 `idle` 与 `busy` 两种状态；
- `tasks` 是一个**任务列表**，包含 CPU 当前的所有任务；
  - 对于不会使用 `vector` 的同学，将 `tasks` 看成一个长度为 `tasks.size()` 的数组即可，其中的元素都是 `Task` 类型的。使用该知识即可完成本题。
- `addTask(t)` 已为你写好，表示向 CPU 的任务列表中加入 `t`；
- `changePriority(task_id, priority)` 已为你写好，表示将 CPU 的任务列表中，编号为 `task_id` 的任务的优先级修改为 `priority`；
- `run()` 是**你需要在派生类**中实现的部分，你需要从当前的任务列表中，根据指定的方法找到**下个时间单位 CPU 需要执行的任务**（时间单位为 1）。并返回一个 `pair <CPUState, unsigned int>`，前者表示 CPU 的状态 (busy/idle)，后者表示正在执行的任务编号 `task_id`。
  - 如果 CPU 处于忙碌 (busy) 状态，则 `task_id` 不会是 0；
  - 如果 CPU 处于空闲 (idle) 状态，则返回的 `task_id` 必须为 0（此时没有任务在执行）。
  - 对于不会使用 `pair` 的同学，假设你需要返回的两个值为 `x, y`，只需使用 `make_pair(x, y)` 即可产生一个 `pair` 类型。

而你需要实现的部分，就是下面三个由基类派生的来的派生类，**你至少需要实现每个派生类中的 `run()` 接口**；当然，你也可以对 `addTask(t)` 与 `changePriority(task_id, priority)` 在派生类中进行修改，但是**不能改变这两个函数的参数接口**。我们将会模拟 1000​ 个连续时间单位，调用 1000 次 `run()` 来评估你的代码的实现情况。

```c++
// FCFS method based CPU.
class CPU_FCFS : public CPU {
    // TODO: complete the FCFS method.

};

// SRTF method based CPU.
class CPU_SRTF : public CPU {
    // TODO: complete the SRTF method.

};

// priority method based CPU.
class CPU_PRIORITY : public CPU {
    // TODO: complete the priority method.

};
```
test.cpp 如下：
```c++
# include <iostream>
# include <vector>
# include "cpu.hpp"
# include "task.hpp"

using namespace std;

namespace cpu_testing {
    enum operationType {AddTask, ChangePriority};

    struct Operation {
        operationType type;
        uint trig_time;
        sjtu::Task task;

        Operation(operationType _type, uint _trig_time, const sjtu::Task &_task) : task(_task) {
            type = _type;
            trig_time = _trig_time;
        }

        ~ Operation() = default;
    };

    vector <Operation> readInputs() {
        vector<Operation> operations;
        int m, op;
        uint trig_time, task_id, priority, time;
        cin >> m;
        for (int i = 0; i < m; ++i) {
            cin >> op >> trig_time;
            if (op == 0) {
                cin >> task_id >> priority >> time;
                operations.emplace_back(Operation(AddTask, trig_time, sjtu::Task(task_id, priority, time)));
            } else {
                cin >> task_id >> priority;
                operations.emplace_back(Operation(ChangePriority, trig_time, sjtu::Task(task_id, priority)));
            }
        }
        return operations;
    }

    void processOutputs(const vector<pair<sjtu::CPUState, uint> > &ans) {
        for (auto log: ans) {
            if (log.first == sjtu::busy && log.second == 0) {
                cerr << "[Error] Busy CPU is not processing any task.\n";
                exit(1);
            }
            if (log.first == sjtu::idle && log.second != 0) {
                cerr << "[Error] Idle CPU is still processing tasks.\n";
                exit(1);
            }
            cout << log.first << ' ' << log.second << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    vector<cpu_testing::Operation> operations(cpu_testing::readInputs());

    sjtu::CPU *processor;
    int CPUType;
    cin >> CPUType;
    switch (CPUType) {
        case 0:
            processor = new sjtu::CPU_FCFS();
            break;
        case 1:
            processor = new sjtu::CPU_SRTF();
            break;
        case 2:
            processor = new sjtu::CPU_PRIORITY();
            break;
        default:
            cerr << "[Error] Unexpected CPU type.";
            return 1;
    }

    vector<pair<sjtu::CPUState, uint> > ans;
    int time = 0, cur = 0;
    while (time <= 1000) {
        while (cur < operations.size() && operations[cur].trig_time == time) {
            int success = 0;
            if (operations[cur].type == cpu_testing::AddTask)
                success = processor->addTask(operations[cur].task);
            if (operations[cur].type == cpu_testing::ChangePriority)
                success = processor->changePriority(operations[cur].task.task_id, operations[cur].task.priority);
            if (success == 0) {
                cerr << "[Error] Fail to execute operation " << cur << ".\n";
            }
            ++ cur;
        }
        ans.push_back(processor->run());
        ++ time;
    }

    cpu_testing::processOutputs(ans);
    delete processor;
    return 0;
}
```
###

### Input Format

输入包含若干行。第一行为一个数 `m`，表示任务数量。

接下来 `m` 行，每行前两个数 `op, trig_time`，表示操作类型与操作的时间。

- 操作类型为 0 表示为添加任务；操作类型为 1 表示更改任务优先级；
- 操作的时间需要是一个 $[0,999]$ 内的整数。

接下来，如果操作类型为 0，接下来三个数 `task_id, priority, time` 描述一个任务；如果操作类型为 1，接下来两个数 `task_id, priority` 表示将编号为 `task_id` 的任务的优先级值修改为 `priority`。

**保证操作时间按照升序排序**。

最后一行，一个数 `CPUType` 表示 CPU 所采取的方法：

- `CPUType = 0` 表示采用 FCFS 方法；
- `CPUType = 1` 表示采用 SRTF 方法；
- `CPUType = 2` 表示采用 Priority 方法。

### Output Format

输出包含 1000 行，每行两个数，表示 $[0,999]$ 这 1000 个连续时间单位内 CPU 的任务安排情况。第一个数表示 CPU 状态，0 表示 idle，1 表示 busy；第二个数表示下个时间单位 CPU 执行的进程的 `task_id`，如果当前 CPU 空闲，则这个数应该为 0。**并且从添加任务到执行任务我们认为可以在一个时间单位内完成。**

### Samples

#### Sample 1

**Input:**
```
**输入样例 1**

```
3
0 1 1 10 3
0 1 2 10 1
0 1 3 10 1
0
```

**输出样例 1**

```
0 0
1 1
1 1
1 1
1 2
1 3
0 0
... (后面均为 0 0)
```

**输入样例 2**

```
3
0 1 1 10 3
0 2 2 10 1
0 3 3 10 1
1
```

**输出样例 2**

```
0 0
1 1
1 2
1 3
1 1
1 1
0 0
... (后面均为 0 0)
```

**输入样例 3**

```
4
0 1 1 10 3
0 2 2 9 2
1 3 1 8
0 3 3 11 1
2
```

**输出样例 3**

```
0 0
1 1
1 2
1 1
1 1
1 2
1 3
0 0
... (后面均为 0 0)
```

###
```

**Output:**
```
见样例输入
```

### Data Constraints

### 

$1 \leq m \leq 200$

测试数据中，满足

- 30% 的数据，测试 FCFS 方法；

- 30% 的数据，测试 SRTF 方法；
- 40% 的数据，测试 Priority 方法。

### 注意事项

1. **不能改动基类中的任何代码**；
2. **不能向 `src.hpp` 中添加头文件**；
3. **不能改动 `task.hpp` 中的任何代码**；
4. **输入和输出已经在 `main.cpp` 中为你写好，你不需要、且不应该考虑这两个部分（这两个部分的格式仅提供作调试用）。你可以直接在 `src.hpp` 中实现代码**。
5. 可以在三个派生类中加入成员与成员函数；
6. 我们的测试代码会在每个时间单位开始时调用 `run()`；
7. **CPU 是抢占式的**，也就是说，在 SRTF/Priority 方法中，上个时间单位执行的未执行完的任务，在下个时间单位不一定继续执行（比如来了一个优先级更高的任务，或来了一个剩余时间更短的任务）；
8. **优先级越高的任务，`priority` 值越小**。

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 1000 ms
- **Memory Limit (per test case)**: 256 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/070/data_test/`.

## Submission Requirements

### Submission Format

Submit a single C++ source file to the Online Judge. The OJ provides its own `main.cpp` which `#include "src.hpp"`. Your submission will be written to `src.hpp` and compiled together with the OJ's driver code.

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
