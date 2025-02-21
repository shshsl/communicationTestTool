#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QDebug>

class WorkerThread : public QThread {
    Q_OBJECT

public:
    explicit WorkerThread(QThread *parent = nullptr);

    void run() override {
        // Your thread logic here
        qDebug() << "WorkerThread running (ID:" << workerId << ")";
    }

private:
    int workerId = 0; // Initialize member variable
};

//// In your main function or other code:
//int main() {
//    WorkerThread worker1; // Uses the default constructor
//    WorkerThread worker2(123); // Uses the constructor with an argument

//    worker1.start();
//    worker2.start();

//    // ...
//}

#endif // WORKERTHREAD_H
