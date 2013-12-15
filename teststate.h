#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "statetracker.h"

class TestState : public QObject, public StateTracker
{
    Q_OBJECT

public:
    enum Tasks
    {
        Task_Start,
        Task_Wait,
        Task_ResultTest,
        Task_End
    };

    TestState(QObject *parent = 0);

    virtual void RunStateMachine();

public slots:
    void OnStart();
    void OnWait();
    void OnResultTest();
    void OnEnd();

private:
    int m_iWaitCount;
};

#endif // TESTSTATE_H
