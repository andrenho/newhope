#ifndef ENGINE_WORKER_H_
#define ENGINE_WORKER_H_

#include "engine/person.h"

enum WorkerJob {
	SHOPKEEPER,
};

class Worker : public Person {
public:
	Worker(Point init);
	~Worker();

private:
	Worker(const Worker&);
	Worker& operator=(const Worker&);
};

#endif  // ENGINE_WORKER_H_
