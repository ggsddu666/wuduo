#include <iostream>
#include"CLFileHandle.h"

using namespace std;
#define BUFFER_SIZE  20

// 构造函数
CLFileHandle::CLFileHandle(const char* FileName) {
	// 打开文件，并初始化成员变量
	fd = open(FileName, O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	if(fd == -1) {
		throw "file open failed";
	}
	buffer = new char[BUFFER_SIZE];
	UsedOfBuffer = 0;
	mutex = new pthread_mutex_t;
	if(pthread_mutex_init(mutex, 0) != 0) {
		delete mutex;
		delete []buffer;
		close(fd);
		throw "init failed";
	}
}

// 析构函数，将成员变量删除
CLFileHandle::~CLFileHandle() {
	if(fd != 0)
		close(fd);
	if(mutex != 0) {
		delete mutex;
	}
	if(buffer != 0) {
		delete[] buffer;
	}
}

// 读文件
int CLFileHandle::ReadFromFile(int pos, int len) {
	// 刷新缓存，保证读到的数据是最新的
	flushCache();
	// 上互斥锁
	if(pthread_mutex_lock(mutex) != 0)
		throw "pthread_mutex_lock falied";
	int r;
	if(len > 0) {
		// 读出长度为len的文件，放入buffer缓存区
		// 修改偏移量
		lseek(fd, pos, SEEK_SET);
		r = read(fd, buffer, len);
		if(r == -1) {
			cout << "read failed" << endl;
		}
		if(r == 0) {
			cout << "read end of the file" << endl;
		}
		// 打印读出的字符串
		cout << buffer << endl;

		// 减少文件中字符串长度
		len -= r;
	}

	// 文件读完，解锁
	if(pthread_mutex_unlock(mutex) != 0) {
		throw "pthread_mutex_unlock failed";
	}
	return 0;
}

// 写文件
int CLFileHandle::WriteToFile(const char* pstrMsg) {
	// 上互斥锁
	if(pthread_mutex_lock(mutex) != 0)
		throw "pthread_mutex_lock falied";
	int msgLength = strlen(pstrMsg);
	int bufferRemainLength = BUFFER_SIZE - UsedOfBuffer;

	// 写入长度大于buffer长度，可以直接写入文件
	if(msgLength >= BUFFER_SIZE) {
		if(fd == -1)
			return -1;
		// 写入缓存
		int w = write(fd, pstrMsg, msgLength);
		cout << "字符串：" << pstrMsg << " 写入" << endl;
		// 解锁
		if(pthread_mutex_unlock(mutex) != 0) {
			throw "pthread_mutex_unlock failed";
		}
		if(w == -1) {
			return -1;
		}
		return w;
	}
	// 写入长度大于buffer剩余部分，刷新缓存
	if(msgLength > bufferRemainLength) {
		if(flushCache() == -1) {
			// 解锁
			if(pthread_mutex_unlock(mutex) != 0) {
				throw "pthread_mutex_unlock failed";
			}
			return -1;
		}

	}

	memcpy(buffer+UsedOfBuffer, pstrMsg, msgLength);
	UsedOfBuffer += msgLength;
	cout << "字符串：" << pstrMsg << " 写入" << endl;

	// 文件写完，解锁
	if(pthread_mutex_unlock(mutex) != 0) {
		throw "pthread_mutex_unlock failed";
	}
}

// 刷新操作，将缓存池的数据写入到文件中
int CLFileHandle::flushCache() {

	// return 0;
	// 将缓存池的数据写入到文件中
	int w = write(fd, &buffer, UsedOfBuffer);
	// 将缓存池的长度置为0
	UsedOfBuffer = 0;

	return 0;
}