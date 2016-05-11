/*
 * BoundedBuffer.hpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */

#ifndef BOUNDEDBUFFER_HPP_
#define BOUNDEDBUFFER_HPP_

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
	void put(boost::shared_ptr<tcp::socket>& element);
	boost::shared_ptr<tcp::socket> get(void);

private:
	std::deque<boost::shared_ptr<tcp::socket> > queue;
	boost::mutex queueMutex;
	boost::interprocess::interprocess_semaphore availableElements, freeElements;
};

#endif /* BOUNDEDBUFFER_HPP_ */
