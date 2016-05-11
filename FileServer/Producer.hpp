/*
 * Producer.hpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */

#ifndef PRODUCER_HPP_
#define PRODUCER_HPP_

#define PORT 8540

#include "BoundedBuffer.hpp"

class Producer
{
public:
	Producer(boost::shared_ptr<BoundedBuffer>& connectedSockets);
	void run(void);

private:
	boost::asio::io_service ioService;
	tcp::acceptor acceptor;
	boost::shared_ptr<BoundedBuffer> connectedSockets;
};

#endif /* PRODUCER_HPP_ */
