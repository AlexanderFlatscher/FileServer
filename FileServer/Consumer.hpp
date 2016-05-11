/*
 * Consumer.hpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */

#ifndef CONSUMER_HPP_
#define CONSUMER_HPP_

//#define DOWNLOAD_FILE_PATH "files"
#define REQUESTED_BUFFER_SIZE 1024

#include "BoundedBuffer.hpp"
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class Consumer
{
public:
	Consumer(boost::shared_ptr<BoundedBuffer>& connectedSockets);
	void run(void);
	~Consumer();

private:
	//static const std::string filePath;

private:
	boost::shared_ptr<BoundedBuffer> connectedSockets;
};

#endif /* CONSUMER_HPP_ */
