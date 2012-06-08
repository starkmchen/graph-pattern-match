/*
 * CacheFile.h
 *	创建磁盘缓冲区，完成磁盘高效读写操作，磁盘替换采用LRU替换策略
 *  Created on: 2012-4-11
 *      Author: zhgn
 */

#ifndef CACHEFILE_H_
#define CACHEFILE_H_

#include <stdio.h>
#include <cstring>

//表示当前块的状态
enum State {
	used, null, fixed
};

struct buffer_head {
	State state; //缓存块状态
	bool modify; //标记某一块是否被修改
	int filepos; //标记某一块存储到文件的位置
	int pos; //标记块的数组位置
	struct buffer_head * prev; //前一块
	struct buffer_head * next; //后一块

};

typedef struct buffer_head bhead;

//缓冲区类
class Cache {
public:
	Cache() {
	}
	~Cache();

	//初始化，声明后需执行此操作，b代表缓冲区的个数，c代表每个缓冲区的大小
	void init(FILE * f, const int b, const int c);
	//写入缓冲区，p表示文件块位置
	void fwrite_cache(const void * bytes, const int length, const int p);
	//读取缓冲区
	void fread_cache(void * bytes, const int length, const int p);
	//写入缓冲区一个数字
	void fwrite_number(const int num, const int p) {
		fwrite_cache((char *) &num, sizeof(int), p);
	}
	//读取一个数字
	int fread_number(const int p) {
		char ca[sizeof(int)];
		fread_cache(ca, sizeof(int), p);
		return *((int *) ca);
	}
	//将当前修改的块全部写如文件中
	void flush();

	int pos; //指向当前操作位置

protected:
	//是否在缓存中，不在返回-1，在返回位置
	int incache(const int p);
	//返回下一个可以使用的块
	int nextcache();
	//LRU算法调整
	void adjustLRU(const int p);
	//固定住某一块
	bool fix(const int p);
	//解除固定
	bool unfix(const int p);
	//解除所有固定
	bool unfixall();
	//将缓冲区num大小的数据块写入文件
	void put_bytes(const void* bytes, int num, FILE* fp) {
		fwrite(bytes, num, 1, fp);
		fflush(fp);
	}
	//读出num大小的数据块到缓冲区
	void get_bytes(void* bytes, int num, FILE* fp) {
		fread(bytes, num, 1, fp);
	}
	//定位块
	void seek(FILE * fp, const int p) {
		fseek(fp, csize * p, SEEK_SET);
	}

	FILE * fp; //文件指针
	int bsize; //缓存块的数目
	int csize; //缓存块的大小

	char ** buffer; //缓冲区

	bhead * bh;
	bhead * first;
	bhead * last;

};

void Cache::init(FILE * f, const int b, const int c) {

	bsize = b;
	csize = c;

	if (f == NULL) {
		//exit(-1);
		printf("fp == NULL!\n");
		return;
	} else {
		fp = f;
	}

	bh = new bhead[bsize];
	first = &(bh[0]);
	last = &(bh[bsize - 1]);
	bhead * p = last;
	for (int i = 0; i < bsize; i++) {
		bh[i].state = null;
		bh[i].filepos = -1;
		bh[i].modify = false;
		bh[i].pos = i;
		bh[i].prev = p;
		p = &(bh[i]);
		(bh[i].prev)->next = p;
	}

	bh[bsize - 1].next = first;
	buffer = new char *[bsize];

	for (int i = 0; i < bsize; i++) {
		buffer[i] = new char[csize];
	}

}

Cache::~Cache() {
	delete[] bh;

	for (int i = 0; i < bsize; i++) {
		delete[] buffer[i];
	}
	delete[] buffer;
}

void Cache::fwrite_cache(const void * bytes, const int length, const int p) {
	int ic = incache(p);
	if (ic == -1) {
		//不在缓冲区中
		int nc = nextcache();
		bhead * bp = &(bh[nc]);
		//如果块已被占用，则写入
		if (bp->state == used && bp->modify == true) {
			seek(fp, bp->filepos);
			put_bytes(buffer[nc], csize, fp);
		}
		//从磁盘读取
		seek(fp, p);
		get_bytes(buffer[nc], csize, fp);
		//读取至缓冲区
		memcpy(buffer[nc] + pos, bytes, length);
		pos += length;
		bp->filepos = p;
		bp->state = used;
		bp->modify = true;
		//策略调整
		adjustLRU(nc);

	} else {
		//读取至缓冲区
		memcpy(buffer[ic] + pos, bytes, length);
		pos += length;
		bh[ic].modify = true;

		adjustLRU(ic);
	}

}

void Cache::fread_cache(void * bytes, const int length, const int p) {
	int ic = incache(p);
	if (ic == -1) {
		//不在缓冲区中
		int nc = nextcache();
		bhead * bp = &(bh[nc]);
		//如果块已被占用，则写入
		if (bp->state == used && bp->modify == true) {
			seek(fp, bp->filepos);
			put_bytes(buffer[nc], csize, fp);
		}
		//从磁盘读取
		seek(fp, p);
		get_bytes(buffer[nc], csize, fp);
		//读取至缓冲区
		memcpy(bytes, buffer[nc] + pos, length);
		pos += length;
		bp->filepos = p;
		bp->state = used;
		bp->modify = true;
		//策略调整
		adjustLRU(nc);

	} else {
		memcpy(bytes, buffer[ic] + pos, length);
		pos += length;

		adjustLRU(ic);
	}

}
/**
 bool Cache::fix(const int p) {

 }

 bool Cache::unfix(const int p) {

 }

 bool Cache::unfixall() {

 }
 */
void Cache::flush() {
	for (int i = 0; i < bsize; i++) {
		if (bh[i].state == used && bh[i].modify == true) {
			int p = bh[i].filepos;
			seek(fp, p);
			put_bytes(buffer[i], csize, fp);
		}
	}

}

int Cache::incache(const int p) {
	bhead * bp = first;
	for (int i = 0; i < bsize; i++) {
		if (bp->filepos == p) {
			return bp->pos;
		}
		bp = bp->next;
	}

	return -1;
}

int Cache::nextcache() {
	return last->pos;

}

void Cache::adjustLRU(int p) {
	if (first == &(bh[p])) {
		return;

	} else if (last == &(bh[p])) {
		last = bh[p].prev;
		first = &(bh[p]);

	} else {
		(bh[p].prev)->next = bh[p].next;
		(bh[p].next)->prev = bh[p].prev;
		bh[p].next = first;
		bh[p].prev = last;
		first->prev = &(bh[p]);
		last->next = &(bh[p]);
		first = &(bh[p]);
	}

}

#endif /* CACHEFILE_H_ */
