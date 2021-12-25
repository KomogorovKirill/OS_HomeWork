#!/usr/bin/python3

import fuse
from fuse import Fuse
import stat, errno, json
import requests
from urllib.parse import quote

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
        
    def getattr(self, path):
        if path in ['.','..']:
            st = default_stat()
            st.st_mode = stat.S_IFDIR | 0o644
            st.st_nlink = 2
            return st
        entry = self.getentry(path)
        if not entry: 
            return -errno.ENOENT
        st = default_stat()
        if 'author' in entry:
            st.st_mode = stat.S_IFDIR | 0o644
            st.st_nlink = 2
        else:
            st.st_mode = stat.S_IFREG | 0o644
            st.st_nlink = 1
            st.st_size = 0 if 'poem' not in entry else len(entry['poem'])*2
        return st

    def open(self, path, flags):
        if (not self.getentry(path)):
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
        
if __name__ == '__main__':
    driver = simple_fuse(version = "%prog " + fuse.__version__, usage=Fuse.fusage,dash_s_do="setsingle")
    driver.parse(errex=1) 
    driver.main()
