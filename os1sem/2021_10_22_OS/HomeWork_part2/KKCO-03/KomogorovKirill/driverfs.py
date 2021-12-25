#!/usr/bin/python3

import fuse
from fuse import Fuse
import stat, errno, json
import requests
from urllib.parse import quote
import os

fuse.fuse_python_api = (0, 2)

class default_stat(fuse.Stat):
    def __init__(self):
        self.st_mode = 0
        self.st_ino = 0
        self.st_dev = 0
        self.st_nlink = 0
        self.st_uid = 0
        self.st_gid = 0
        self.st_size = 0
        self.st_atime = 0
        self.st_mtime = 0
        self.st_ctime = 0


class simple_fuse(Fuse):
    def getentry(self, path):
        r = requests.get("http://127.0.0.1:8000/library" + quote(path))
        if r.status_code == 200:
            data = json.loads(r.content)
            return data
        else:
            print("Error! There is on connection to the server!")
            return -errno.ENOENT

    def write(self, path, buf, count):
        path = str(path)
        path = path.rstrip('/')
        path = path.split('/')
        r = requests.put("http://127.0.0.1:8000/" + quote(path[len(path) - 2]) + "/" + quote(path[len(path) - 1]), params={"poem": buf[0], "count": count})
        if r.status_code != 200:
            print("Error! Sky server power off")
            exit(1)
        return 1

    def getattr(self, path):
        if path in ['.','..']:
            st = default_stat()
            st.st_mode = stat.S_IFDIR | 0o777
            st.st_nlink = 2
            return st
        entry = self.getentry(path)
        if entry.get("status_code") != None:
            return -errno.ENOENT
        st = default_stat()
        if 'author' in entry:
            st.st_mode = stat.S_IFDIR | 0o777
            st.st_nlink = 2
        else:
            st.st_mode = stat.S_IFREG | 0o777
            st.st_nlink = 1
            st.st_size = 0 if 'poem' not in entry else len(entry['poem'])*2
        return st

    def open(self, path, flags):
        entry = self.getentry(path)
        if (not entry and entry != "" ) or ("status_code" in entry):
            return -errno.ENOENT
    
    def read(self, path, size, offset):
        entry = self.getentry(path)
        if (not entry):
            return -errno.ENOENT
        data = '' if 'poem' not in entry else entry['poem']
        return data.encode('utf-8')
    
    def readdir(self, path, offset):
        entry = self.getentry(path)
        if (not entry):
            return -errno.ENOENT
        for d in entry['author']:
            yield fuse.Direntry(d)

    def mkdir(self, path, mode):
        path = path.rstrip('/')
        path = path.split('/')
        r = requests.post("http://127.0.0.1:8000/", params={"author":  path[len(path) - 1]})
        if r.status_code != 200:
            print("Error! Sky server power off")
            exit(1)

    def create(self, path, mode, fi=None):
        path = path.rstrip('/')
        path = path.split('/')
        r = requests.post("http://127.0.0.1:8000/" + quote(path[len(path) - 2]), params={"work": path[len(path) - 1].encode('utf-8')})
        if r.status_code != 200:
            print("Error! Sky server power off")
            exit(1)

    def truncate(self, path, length):
        pass

    def flush(self, path):
        pass

    def release(self, path, fh):
        pass

    def close(self, path):
        pass

    def utimens(dirfd, path, times, flags):
        pass

    def utimesat(dirfs, path, times, flags):
        pass

if __name__ == '__main__':
    driver = simple_fuse(version = "%prog " + fuse.__version__, usage=Fuse.fusage,dash_s_do="setsingle")
    driver.parse(errex=1) 
    driver.main()
