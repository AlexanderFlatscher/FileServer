#ifndef READERWRITERMUTEX_H_
#define READERWRITERMUTEX_H_

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/mutex.hpp>

class ReaderWriterMutex {
public:
	ReaderWriterMutex(void);

	void readerLock();
	void readerUnlock();
	void writerLock();
	void writerUnlock(); 

private:
	int readers;
	boost::mutex readersMutex;
	boost::interprocess::interprocess_semaphore allowedWriters;
};

#endif