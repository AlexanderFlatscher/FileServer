#ifndef BOUNDEDBUFFER_H_
#define BOUNDEDBUFFER_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <deque>
#include <iostream>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

using boost::asio::ip::tcp;

class BoundedBuffer
{
public:
	BoundedBuffer(unsigned int maxElements);
	void put(tcp::socket* element);
	tcp::socket* get(void);

private:
	std::deque<tcp::socket*> queue;
	boost::mutex queueMutex;
	boost::interprocess::interprocess_semaphore availableElements, freeElements;
};

#endif