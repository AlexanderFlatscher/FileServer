#include "ReaderWriterMutex.h"


ReaderWriterMutex::ReaderWriterMutex(void) : allowedWriters(1), readers(0) {}

void ReaderWriterMutex::readerLock(){
    {
        boost::mutex::scoped_lock lock(readersMutex);
        readers++;
        if(readers == 1)
            allowedWriters.wait();
    }
}

void ReaderWriterMutex::readerUnlock(){
    {
        boost::mutex::scoped_lock lock(readersMutex);
        readers--;
        if(readers == 0)
            allowedWriters.post();
    }
}

void ReaderWriterMutex::writerLock(){
    allowedWriters.wait();
}

void ReaderWriterMutex::writerUnlock(){
    allowedWriters.post();
}
