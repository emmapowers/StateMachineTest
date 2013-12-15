#ifndef STATETRACKER_H
#define STATETRACKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QHash>

class StateTracker;

class StateTrackerThread : public QThread
{
    Q_OBJECT

public:
    StateTrackerThread(StateTracker *tracker);

    virtual void run();
    void EmitTaskStarted(int taskId);

signals:
    void TaskStarted(int taskId);

private:
    StateTracker *m_StateTracker;
};

class StateTracker
{
    struct Task
    {
        QObject *m_Sender;
        QString m_Slot;
    };

public:
    enum TASK_STATUS
    {
        TASK_SUCCESS,
        TASK_FAILURE
    };

    explicit StateTracker();
    
    //Task Functions
    void AddTask(int taskId, QObject *sender, const char *slot);

    void FinishTask(int taskStatus = TASK_SUCCESS);

    //Thread stuff
    void start();
    virtual void RunStateMachine() = 0;

protected:
    int StartTask(int taskId);

private:
    QHash<int, Task *> m_Tasks;
    int m_TaskStatus;
    int m_CurrentTask;
    QMutex m_Mutex;
    QWaitCondition m_WaitCondition;
    StateTrackerThread *m_Thread;
};

#endif // STATETRACKER_H
