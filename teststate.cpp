#include "teststate.h"
#include <QDebug>

TestState::TestState(QObject *parent)
    : QObject(parent)
    , StateTracker()
{
    //Add tasks and the slots to call when they start
    AddTask(Task_Start, this, SLOT(OnStart()));
    AddTask(Task_Wait, this, SLOT(OnWait()));
    AddTask(Task_ResultTest, this, SLOT(OnResultTest()));
    AddTask(Task_End, this, SLOT(OnEnd()));

    m_iWaitCount = 0;
}

/*
 *
 *  This function runs in a seperate thread, and defines the workflow
 *
 */
void TestState::RunStateMachine()
{
    //Run and block untill complete
    StartTask(Task_Start);

    //Loop till the task succeedes
    while(StartTask(Task_Wait) == TASK_FAILURE);

    //Run, and do something interesting with the task status
    switch(StartTask(Task_ResultTest))
    {
    case 3:
        qDebug() << "3 returned";
        break;
    default:
        qDebug() << "Unexpected Result!";
    }

    //Run and block until complete
    StartTask(Task_End);

    qDebug() << "Complete!";
}

/*
 *
 * These slots define the actual tasks to be performed. They run in the GUI thread
 *
 */
void TestState::OnStart()
{
    qDebug() << "Start";
    sleep(1);
    FinishTask();
}

void TestState::OnWait()
{
    sleep(1);

    m_iWaitCount ++;

    qDebug() << "Wait" << m_iWaitCount;

    FinishTask((m_iWaitCount < 10 ? TASK_FAILURE : TASK_SUCCESS));
}

void TestState::OnResultTest()
{
    qDebug() << "Result Test";
    sleep(1);

    FinishTask(3);
}

void TestState::OnEnd()
{
    qDebug() << "End";
    sleep(1);
    FinishTask();
}
