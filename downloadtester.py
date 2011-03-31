#!/usr/bin/python

import os
import hashlib
import sys
import time
import httplib
from threading import Thread


def calculateMD5(fh):
    hash = hashlib.md5()
    data = fh.read(4096)
    size = 0
    while len(data)>0:
        size += len(data)
        hash.update(data)
        data = fh.read(4096)
    return hash.hexdigest(), size        

class Downloader(Thread):
    host = None
    path = None
    md5sum = ""
    size = 0
    success = True
    def __init__(self, host, path):
        Thread.__init__(self)
        self.host = host
        self.path = path
    def run(self):
        try:
            connection = httplib.HTTPConnection(self.host)
            connection.request("GET",self.path)
            response = connection.getresponse()
            if response.status == 200:
                self.md5sum, self.size = calculateMD5(response)
                success = False
        except:
            print "Connection Error"            


def checkDownloads(downloader):
    size = -1
    md5sum = ""
    test_ok = True
    for d in downloader:
        if d.success:
            if size < 0:
                size = d.size
                md5sum = d.md5sum
            if d.md5sum != md5sum or d.size != size:
                test_ok = False
                break        
        else:
            test_ok = False
            print "Download error"
            break
    return test_ok



def downloadTest1(host):
    downloader = []
    for i in range(20):
        downloader.append(Downloader(host,"/file1"))

    print "Starting Downloads"

    start = time.time()

    for d in downloader:        
        d.start()
    
    print "Waiting for Downloads to finish"

    for d in downloader:        
        d.join()

    stop = time.time()

    test_ok = checkDownloads(downloader)
    return test_ok, (stop-start)

def downloadTest2(host):
    downloader1 = []
    downloader2 = []
    for i in range(10):
        downloader1.append(Downloader(host,"/file1"))
    for i in range(10):
        downloader2.append(Downloader(host,"/file2"))

    print "Starting Downloads"

    start = time.time()

    for d in downloader1:        
        d.start()
    for d in downloader2:        
        d.start()
    
    print "Waiting for Downloads to finish"

    for d in downloader1:        
        d.join()
    for d in downloader2:        
        d.join()

    stop = time.time()
    test_ok = checkDownloads(downloader1)
    test_ok = test_ok & checkDownloads(downloader2)

    return test_ok, (stop-start)



if __name__=="__main__":
    if len(sys.argv) < 2:
        print "Usage: %s <host>"%(sys.argv[0])
        sys.exit(0)

    test_ok, duration = downloadTest1(sys.argv[1])
    if test_ok:
        print "Download test 1 [PASSED]"
        print "The Downloads took %.3f seconds"%(duration)
    else:
        print "Download test 1 [FAILED]"

    test_ok, duration = downloadTest2(sys.argv[1])
    if test_ok:
        print "Download test 2 [PASSED]"
        print "The Downloads took %.3f seconds"%(duration)
    else:
        print "Download test 2 [FAILED]"
