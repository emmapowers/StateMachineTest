#include "statetracker.h"
#include <assert.h>
#include <QDebug>


StateTrackerThread::StateTrackerThread(StateTracker *tracker)
    : QThread(NULL)
{
    m_StateTracker = tracker;
}

void StateTrackerThread::EmitTaskStarted(int taskId)
{
    emit TaskStarted(taskId);
}

void StateTrackerThread::run()
{
    m_StateTracker->RunStateMachine();
}

StateTracker::StateTracker()
{
    m_Thread = new StateTrackerThread(this);
}

void StateTracker::AddTask(int taskId, QObject *sender, const char *slot)
{
    Task *task = new Task();
    task->m_Sender = sender;
    task->m_Slot = slot;

    //qDebug() << "Adding task " << taskId;

    m_Tasks[taskId] = task;
}

int StateTracker::StartTask(int taskId)
{
    assert(m_Tasks.contains(taskId));

    Task *task = m_Tasks[taskId];
    m_CurrentTask = taskId;
    m_TaskStatus = TASK_FAILURE;

    QObject::connect(m_Thread , SIGNAL(TaskStarted(int)), task->m_Sender, qPrintable(task->m_Slot), Qt::QueuedConnection);
    m_Thread->EmitTaskStarted(taskId);
    QObject::disconnect(m_Thread , SIGNAL(TaskStarted(int)), task->m_Sender, qPrintable(task->m_Slot));

    //qDebug() << "running task " << taskId;

    m_Mutex.lock();
    m_WaitCondition.wait(&m_Mutex);
    m_Mutex.unlock();

    return m_TaskStatus;
}

void StateTracker::FinishTask(int taskStatus)
{
    m_TaskStatus = taskStatus;
    m_WaitCondition.wakeAll();
}

void StateTracker::start()
{
    m_Thread->start();
}
