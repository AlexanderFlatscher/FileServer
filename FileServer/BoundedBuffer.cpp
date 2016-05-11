/*
 * BoundedBuffer.cpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */

#include "BoundedBuffer.hpp"

BoundedBuffer::BoundedBuffer(unsigned int maxElements) : availableElements(0), freeElements(maxElements) {}

void BoundedBuffer::put(boost::shared_ptr<tcp::socket>& element)
{
	freeElements.wait();
	{
		boost::mutex::scoped_lock lock(queueMutex);
		queue.push_back(element);
	}
	availableElements.post();
}

boost::shared_ptr<tcp::socket> BoundedBuffer::get(void)
{
	boost::shared_ptr<tcp::socket> element;

	availableElements.wait();
	{
		boost::mutex::scoped_lock lock(queueMutex);
		element = queue.front();
		queue.pop_front();
	}
	freeElements.post();

	return element;
}


