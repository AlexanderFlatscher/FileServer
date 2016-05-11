/*
 * Consumer.cpp
 *
 *  Created on: May 2, 2016
 *      Author: josen
 */
#include "Consumer.hpp"

//const std::string Consumer::filePath = "/home/josen/workspace/boost_threading_programs/FileServer/src/files";

Consumer::Consumer(boost::shared_ptr<BoundedBuffer>& connectedSockets)
{
	this->connectedSockets = connectedSockets;
}

Consumer::~Consumer()
{
}

void Consumer::run(void)
{
	while(true)
	{
		boost::shared_ptr<tcp::socket> socket;
		std::string method, filename, singleLine;
		std::stringstream httpRequest, httpResponse, payload;
		std::ifstream requestedFile;
		int requestSize;
		char requestBuffer[REQUESTED_BUFFER_SIZE];

		// get a socket from the BoundedBuffer
		socket = connectedSockets->get();

		// get the http request
		requestSize = socket->receive(boost::asio::buffer(requestBuffer, REQUESTED_BUFFER_SIZE));

		httpRequest << std::string(requestBuffer, requestSize);
		httpRequest >> method >> filename;

		std::cout << "DEBUG" << " " << "filename: " << filename << std::endl;

		//const std::string filePath = DOWNLOAD_FILE_PATH + filename;
		const std::string tempFilePath = std::string("/home/josen/workspace/boost_threading_programs/FileServer/src/files") + filename;
		const std::ios_base::openmode fileMode = std::ios::binary;

		//requestedFile.open(DOWNLOAD_FILE_PATH + filename, std::ios::binary);
		requestedFile.open(tempFilePath, fileMode);

		if(requestedFile.is_open())
		{
			// reading requested file
			while(getline(requestedFile, singleLine))
			{
				payload << singleLine << std::endl;
				std::cout << "DEBUG" << " " << "singleLine: " << singleLine << std::endl;
			}

			// prepare http respone
			httpResponse << "HTTP/1.0 200 OK\n";
			httpResponse << "Content-Type: application/octet-stream\n";

			requestedFile.close();
		}
		else
		{
			// prepare http error response
			payload << "<html><head><title>404 Not Found</title><body><h1>404 Not Found</h1><p>The requested file was not found.</p></body></html>";
			httpResponse << "HTTP/1.0 404 Not Found\n";
			httpResponse << "Content-Type: text/html\n";
		}

		httpResponse << "Server: FileServer/0.0.1\n";
		httpResponse << "Content-Length: " << payload.str().length() << "\n\n";

		// send the http-response header
		socket->send(boost::asio::buffer(httpResponse.str().c_str(), httpResponse.str().length()));

		// send the http-response payload
		socket->send(boost::asio::buffer(payload.str().c_str(), payload.str().length()));

		std::cout << "DEBUG" << " " << "payload: " << payload.str().c_str() << std::endl;

		socket->shutdown(tcp::socket::shutdown_both);
		socket->close();

		//delete socket;
	}
}
