#include "Consumer.h"

Consumer::Consumer(BoundedBuffer* connectedSockets, std::map<std::string, ReaderWriterMutex*>* fileMutexes, boost::mutex* fileMutexesMapMutex) 
{
	this->connectedSockets = connectedSockets;
    this->fileMutexes = fileMutexes;
    this->fileMutexesMapMutex = fileMutexesMapMutex;
}

void Consumer::run(void)
{
	while(true)
	{
		tcp::socket* socket;
		std::string method, filename, temp, singleLine, boundary, content;
		std::stringstream httpRequest, httpResponse, payload;
		std::ifstream requestedFile;
        std::ofstream uploadedFile;
		int requestSize, contentLength;
		char requestBuffer[REQUESTED_BUFFER_SIZE];

		// get a socket from the BoundedBuffer
		socket = connectedSockets->get();

		// get the http request
        
		requestSize = socket->receive(boost::asio::buffer(requestBuffer, REQUESTED_BUFFER_SIZE));
		httpRequest << std::string(requestBuffer, requestSize);
        
		httpRequest >> method >> filename;

        if(method == "GET"){
		    requestedFile.open(DOWNLOAD_FILE_PATH + filename, std::ios::binary);

		    if(requestedFile.is_open())
		    {
			    // reading requested file
			    while(getline(requestedFile, singleLine))
			    {
				    payload << singleLine << std::endl;
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
        }
        else if(method == "POST"){
            // get content length
            httpRequest.seekg(httpRequest.str().find("Content-Length:")+16);
            httpRequest >> temp;
            sscanf_s(temp.c_str(), "%d", &contentLength);

            // get boundary (inserted from browser with enctype="multipart/form-data")
            int boundaryPosition = httpRequest.str().find("boundary=");

            if(boundaryPosition != -1){
                httpRequest.seekg(boundaryPosition + 9);
                httpRequest >> boundary;
            }

            // get content
            int firstBlankLine = httpRequest.str().find("\r\n\r\n");
            content.append(httpRequest.str().substr(firstBlankLine + 4));

            // receive rest of data
            int receivedByteCount = content.size();
            std::cout << content << std::endl;
            while(receivedByteCount <= contentLength){
                memset(requestBuffer, 0, REQUESTED_BUFFER_SIZE);
                requestSize = socket->receive(boost::asio::buffer(requestBuffer, REQUESTED_BUFFER_SIZE));
                std::cout << requestBuffer << std::endl;
                content.append(requestBuffer);
                receivedByteCount += requestSize;
            }

            //std::cout << content << std::endl;

            // crop boundary
            if(boundaryPosition != -1){
                content.erase(0, content.find("\r\n\r\n") + 4);
                int boundaryEndPosition = content.find("--" + boundary);
                std::cout << "boundaryEnd: " << boundaryEndPosition << std::endl;
                //content.erase(content.find("\r\n--" + boundary));
            }

            // write in file
            // create new map entry if file doesn't already exist
            {
                boost::mutex::scoped_lock lock(*fileMutexesMapMutex);
                if(fileMutexes->find(filename) == fileMutexes->end()){
                    fileMutexes->insert(std::pair<std::string, ReaderWriterMutex*>(filename, new ReaderWriterMutex()));
                }
            }

            fileMutexes->find(filename)->second->writerLock();
            uploadedFile.open(DOWNLOAD_FILE_PATH + filename, std::ios::binary);

		    if(uploadedFile.is_open())
		    {
			    uploadedFile << content;
			    uploadedFile.close();
            }

            fileMutexes->find(filename)->second->writerUnlock();
        }
        else {/* TODO redirect to xhamster */}

		// send the http-response header
		socket->send(boost::asio::buffer(httpResponse.str().c_str(), httpResponse.str().length()));
		
		// send the http-response payload
		socket->send(boost::asio::buffer(payload.str().c_str(), payload.str().length()));
		
		socket->shutdown(tcp::socket::shutdown_both);
		socket->close();

		delete socket;
	}
}