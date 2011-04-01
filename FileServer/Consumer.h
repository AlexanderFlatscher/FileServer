#ifndef CONSUMER_H_
#define CONSUMER_H_

#define DOWNLOAD_FILE_PATH "files"
#define REQUESTED_BUFFER_SIZE 1024

#include "BoundedBuffer.h"
#include "ReaderWriterMutex.h"
#include <fstream>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

class Consumer
{
public:
	Consumer(BoundedBuffer* connectedSockets, std::map<std::string, ReaderWriterMutex*>* fileMutexes, boost::mutex* fileMutexesMapMutex);
	void run(void);

private:
	BoundedBuffer* connectedSockets;
    std::map<std::string, ReaderWriterMutex*>* fileMutexes;
    boost::mutex *fileMutexesMapMutex;
};

#endif

