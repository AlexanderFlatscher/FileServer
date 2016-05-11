/*
 * main.cpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */

#include "BoundedBuffer.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"

int main() {
	std::cout << "Initialize bounded buffer for the sockets." << std::endl;
	//BoundedBuffer *boundedBuffer = new BoundedBuffer(50);
	boost::shared_ptr<BoundedBuffer> boundedBuffer (new BoundedBuffer(50));

	std::cout << "Initialize producer." << std::endl;
	Producer producer(boundedBuffer);

	std::cout << "Initialize consumers." << std::endl;

	for(int i = 0; i < 20; i++)
	{
		Consumer consumer(boundedBuffer);
		boost::thread consumerThread(&Consumer::run, &consumer);
	}

	producer.run();

	system("PAUSE");
	return 0;
}
