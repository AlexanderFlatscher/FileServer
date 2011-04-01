#include "BoundedBuffer.h"
#include "ReaderWriterMutex.h"
#include "Consumer.h"
#include "Producer.h"

int main() {
	std::cout << "Initialize bounded buffer for the sockets." << std::endl;
	BoundedBuffer *boundedBuffer = new BoundedBuffer(50);

    boost::mutex *fileMutexesMapMutex = new boost::mutex();
    std::map<std::string, ReaderWriterMutex*> *fileMutexes = new std::map<std::string, ReaderWriterMutex*>();

	std::cout << "Initialize producer." << std::endl;
	Producer producer(boundedBuffer);

	std::cout << "Initialize consumers." << std::endl;

	for(unsigned int i = 0; i < 20; i++)
	{
		Consumer consumer(boundedBuffer, fileMutexes, fileMutexesMapMutex);
		boost::thread consumerThread(boost::bind(&Consumer::run, &consumer));
	}
	
	producer.run();

	system("PAUSE");
	return 0;
}