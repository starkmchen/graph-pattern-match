/**
 * HashFile.h
 *	动态可扩展哈希表，二叉树实现，用于判重
 *  Created on: 2012-3-20
 *      Author: zhgn
 */
#ifndef HASHFILE_H_
#define HASHFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

#include "Cache.h"

#define BNCOUNT 10000 //每个桶中含有BucketNode的数量
#define BUCKETCOUNT 100 //缓冲区桶的个数
#define TREECOUNT 64 //缓冲区中树节点的个数
#define TREELEVEL 16 //树层数
#define BUCKETLENGTH (BNLENGTH * BNCOUNT + sizeof(int) * 2) //桶的长度
#define BNLENGTH sizeof(BucketNode<K> ) //桶节点的长度
#define TNLENGTH sizeof(TreeNode) //树节点的长度
#define CONFIGLENGTH sizeof(Config) //配置文件节点长度
//与操作得到0与1
const int AND[] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100,
		0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };

const char bucketfile[] = "data/bucket.dat";
const char treefile[] = "data/tree.dat";
const char configfile[] = "data/bconfig.dat";

//桶中每个节点，available表示是否使用
template<class K>
class BucketNode {
public:
	BucketNode<K>() {
	}
	BucketNode<K>(const int avail, const K & k) {
		available = avail;
		key = k;
	}
	~BucketNode<K>() {
	}

	int available;
	K key;

};

//二叉树节点，isBucket是否指针为桶，flag表示层数，从0开始
//nextPos0值为0的指针，nextPos1值为1的指针，当指针为桶时，nextPos0指向桶，nextPos1无意义
class TreeNode {
public:
	bool isBucket;
	int flag;
	int nextPos0;
	int nextPos1;
};

//配置文件节点
class Config {
public:
	int tcount;
	int bcount;
};

//HashFile模板
template<class K, class HASH>
class HashFile {
public:
	HashFile<K, HASH>() {
	}
	~HashFile<K, HASH>();
	void init();
	//是否包含该键值，包含返回真
	bool containsKey(K & k);
	/*
	 //是否包含键值数组，长度为n；返回不包含项，项数为n，不包含的项会被添加到哈希表中
	 //强烈建议使用该函数
	 bool containsKey(K k[], int & n);
	 */
	//添加某一个键值，若包含K，则返回false，否则返回true
	bool putKey(K & k);

protected:
	//定位树节点的文件指针
	void seekTNPos(int treepos);
	//定位桶的文件指针
	void seekBNPos(int bucketpos);
	//搜索哈希后的键值，返回桶位置
	int searchTree(int key);
	//搜索桶，返回V值
	bool searchBucket(K & k);
	//添加键值对到桶中
	bool addBucket(K & k);
	//将缓冲区num大小的数据块写入文件
	void put_bytes(const void* bytes, int num, FILE* fp) {
		fwrite(bytes, num, 1, fp);
		fflush(fp);
	}
	//读出num大小的数据块到缓冲区
	void get_bytes(void* bytes, int num, FILE* fp) {
		fread(bytes, num, 1, fp);
	}
	//以二进制的形式写入一个数字，数据块大小为sizeof(int)
	void fwrite_number(int num, FILE* fp) {
		put_bytes((char *) &num, sizeof(int), fp);
	}
	//从二进制数据块中读出一个数字，数据块大小为sizeof(int)
	int fread_number(FILE* fp) {
		char ca[sizeof(int)];
		get_bytes(ca, sizeof(int), fp);
		return *((int *) ca);
	}

//private:
	FILE * bucketfp; //桶文件的文件指针
	FILE * treefp; //树节点文件的文件指针
	FILE * configfp; //配置文件指针
	int tcount; //树节点的总数
	int bcount; //桶的总数
	int tcurrentpos; //当前树节点的位置
	int bcurrentpos; //当前桶的位置
	bool canSplit; //树节点是否能够分裂

	Cache bucketcache; //桶缓冲区
	Cache treecache; //树缓冲区

	HASH hash; //哈希函数
};

template<class K, class HASH>
void HashFile<K, HASH>::init() {
	/*
	 std::cout << BNLENGTH << " " << TNLENGTH << " " << BUCKETLENGTH << " " << CONFIGLENGTH << std::endl;
	 return;
	 */
	if ((bucketfp = fopen(bucketfile, "rb+")) == 0) {
		bucketfp = fopen(bucketfile, "wb+");
		if (bucketfp == 0) {
			fprintf(stderr, "open file bucket.dat error!\n");
			exit(-1);
		}

		//写入两个桶
		BucketNode<K> bn;
		bn.available = 0;

		//char * buffer = new char[BNLENGTH];
		//memset(buffer, 0, sizeof(buffer));

		//写入桶中桶节点个数
		fwrite_number(0, bucketfp);

		//写入BNCOUNT个桶节点
		for (int i = 0; i < BNCOUNT; i++) {
			put_bytes(&bn, BNLENGTH, bucketfp);
		}

		//写入桶末尾指针，指向下一个桶
		fwrite_number(-1, bucketfp);

		fwrite_number(0, bucketfp);
		for (int i = 0; i < BNCOUNT; i++) {
			put_bytes(&bn, BNLENGTH, bucketfp);
		}
		fwrite_number(-1, bucketfp);

		//fseek(bucketfp, 0, SEEK_SET);
		//fseek(bucketfp, sizeof(int), SEEK_CUR);

		//delete[] buffer;
	}

	if ((treefp = fopen(treefile, "rb+")) == 0) {
		treefp = fopen(treefile, "wb+");
		if (treefp == 0) {
			fprintf(stderr, "open file tree.dat error!\n");
			exit(-1);
		}

		//写入三个树节点
		TreeNode tn;
		tn.isBucket = false;
		tn.nextPos0 = 1;
		tn.nextPos1 = 2;
		tn.flag = 0;
		put_bytes(&tn, TNLENGTH, treefp);
		tn.isBucket = true;
		tn.nextPos0 = 0;
		tn.flag = 1;
		put_bytes(&tn, TNLENGTH, treefp);
		tn.nextPos0 = 1;
		put_bytes(&tn, TNLENGTH, treefp);
	}

	if ((configfp = fopen(configfile, "rb+")) == 0) {
		configfp = fopen(configfile, "wb+");
		if (configfp == 0) {
			fprintf(stderr, "open file config.dat error!\n");
			exit(-1);
		}

		//写入配置文件
		char * buffer = new char[sizeof(Config)];
		Config config;
		config.tcount = tcount = 3;
		config.bcount = bcount = 2;
		put_bytes(&config, CONFIGLENGTH, configfp);
		delete[] buffer;
	} else {
		Config config;
		fseek(configfp, 0, SEEK_SET);
		get_bytes(&config, CONFIGLENGTH, configfp);
		tcount = config.tcount;
		bcount = config.bcount;
	}

	bucketcache.init(bucketfp, BUCKETCOUNT, BUCKETLENGTH);
	treecache.init(treefp, TREECOUNT, TNLENGTH);

}

template<class K, class HASH>
HashFile<K, HASH>::~HashFile() {
	fclose(bucketfp);
	fclose(treefp);
	fclose(configfp);
	//bucketcache.~Cache();
	//treecache.~Cache();
	printf("close hashfile!\n");
}

template<class K, class HASH>
bool HashFile<K, HASH>::containsKey(K & k) {
	int key = hash(k);
	int bucketpos = searchTree(key);
	bcurrentpos = bucketpos;
	//seekBNPos(bucketpos);
	bucketcache.pos = 0;

	if (searchBucket(k)) {
		return true;
	} else {
		return false;
	}

}
/*
 template<class K>
 bool HashFile<K>::containsKey(K k[], int & n) {

 }
 */
template<class K, class HASH>
bool HashFile<K, HASH>::putKey(K & k) {
	if (containsKey(k)) {
		return false;
	}
	int key = hash(k);
	int bucketpos = searchTree(key);
	bcurrentpos = bucketpos;
	//seekBNPos(bucketpos);
	bucketcache.pos = 0;
	return addBucket(k);
}

template<class K, class HASH>
void HashFile<K, HASH>::seekTNPos(int treepos) {
	fseek(treefp, treepos * TNLENGTH, SEEK_SET);
}

template<class K, class HASH>
void HashFile<K, HASH>::seekBNPos(int bucketpos) {
	fseek(bucketfp, bucketpos * BUCKETLENGTH, SEEK_SET);
}

template<class K, class HASH>
int HashFile<K, HASH>::searchTree(int key) {
	int bucketpos;
	int i;
	TreeNode tn;

	//seekTNPos(0);
	treecache.pos = 0;
	tcurrentpos = 0;
	canSplit = true;
	for (i = 0; i < TREELEVEL; i++) {
		if (i == TREELEVEL - 1) {
			canSplit = false;
		}
		//get_bytes(&tn, TNLENGTH, treefp);
		treecache.fread_cache(&tn, TNLENGTH, tcurrentpos);
		if (tn.isBucket == true) {
			//如果指向桶，则返回桶位置
			bucketpos = tn.nextPos0;
			break;
		} else {
			//如果指向树节点，则继续索引
			if ((key & AND[i]) == 0) {
				//seekTNPos(tn.nextPos0);
				treecache.pos = 0;
				tcurrentpos = tn.nextPos0;
			} else {
				//seekTNPos(tn.nextPos1);
				treecache.pos = 0;
				tcurrentpos = tn.nextPos1;
			}
		}

	}

	if (i == TREELEVEL) {
		fprintf(stderr, "searchTree error!\n");

		exit(-1);
	}

	return bucketpos;
}

template<class K, class HASH>
bool HashFile<K, HASH>::searchBucket(K & k) {
	int i, pos;
	BucketNode<K> bn;
	//fseek(bucketfp, sizeof(int), SEEK_CUR);
	bucketcache.pos = 0;
	bucketcache.fread_number(bcurrentpos);

	//在桶中找键值对，并返回
	for (i = 0; i < BNCOUNT; i++) {
		//get_bytes(&bn, BNLENGTH, bucketfp);
		bucketcache.fread_cache(&bn, BNLENGTH, bcurrentpos);
		if (bn.available == 1) {
			if (bn.key == k) {
				return true;
			}
		}
	}

	//pos = fread_number(bucketfp);
	pos = bucketcache.fread_number(bcurrentpos);
	if (pos != -1) {
		//seekBNPos(pos);
		bucketcache.pos = 0;
		bcurrentpos = pos;
		return searchBucket(k);
	}

	return false;

}

template<class K, class HASH>
bool HashFile<K, HASH>::addBucket(K & k) {
	//int count = fread_number(bucketfp);
	int count = bucketcache.fread_number(bcurrentpos);
	int i, nextbucket;

	if (count != BNCOUNT) {
		BucketNode<K> bn;

		count++;
		//fseek(bucketfp, -sizeof(int), SEEK_CUR);
		bucketcache.pos -= sizeof(int);
		//fwrite_number(count, bucketfp);
		bucketcache.fwrite_number(count, bcurrentpos);
		bucketcache.flush();

		//顺序找到未使用的桶节点，将键值对插入
		for (i = 0; i < BNCOUNT; i++) {
			//get_bytes(&bn, BNLENGTH, bucketfp);
			bucketcache.fread_cache(&bn, BNLENGTH, bcurrentpos);
			if (bn.available == 0) {
				//fseek(bucketfp, -BNLENGTH, SEEK_CUR);
				bucketcache.pos -= BNLENGTH;
				bn.available = 1;
				bn.key = k;
				//put_bytes(&bn, BNLENGTH, bucketfp);
				bucketcache.fwrite_cache(&bn, BNLENGTH, bcurrentpos);
				bucketcache.flush();
				return true;
			}

		}
		return false;
	}

	else {
		//如果不能分裂
		if (!canSplit) {
			//fseek(bucketfp, BUCKETLENGTH - sizeof(int) * 2, SEEK_CUR);
			bucketcache.pos += BUCKETLENGTH - sizeof(int) * 2;
			//nextbucket = fread_number(bucketfp);
			nextbucket = bucketcache.fread_number(bcurrentpos);

			//如果桶后还挂有桶，则定位到另外的桶进行添加，否则添加新桶
			if (nextbucket != -1) {
				//seekBNPos(nextbucket);
				bucketcache.pos = 0;
				bcurrentpos = nextbucket;
				return addBucket(k);

			} else {
				//写入桶位置
				//fseek(bucketfp, -sizeof(int), SEEK_CUR);
				bucketcache.pos -= sizeof(int);
				//fwrite_number(bcount, bucketfp);
				bucketcache.fwrite_number(bcount, bcurrentpos);
				bcurrentpos = bcount;

				bcount++;
				fseek(configfp, 0, SEEK_SET);
				Config config;
				config.bcount = bcount;
				config.tcount = tcount;
				put_bytes(&config, CONFIGLENGTH, configfp);

				//添加新桶
				fseek(bucketfp, 0, SEEK_END);
				bucketcache.pos = 0;
				//char * buffer = new char[BNLENGTH];
				//memset(buffer, 0, sizeof(buffer));
				BucketNode<K> bn;
				bn.available = 0;
				//fwrite_number(1, bucketfp);
				bucketcache.fwrite_number(1, bcurrentpos);
				for (i = 0; i < BNCOUNT; i++) {
					//put_bytes(&bn, BNLENGTH, bucketfp);
					bucketcache.fwrite_cache(&bn, BNLENGTH, bcurrentpos);
					//bucketcache.flush();
				}
				//fwrite_number(-1, bucketfp);
				bucketcache.fwrite_number(-1, bcurrentpos);
				//delete[] buffer;

				//写入键值
				//fseek(bucketfp, -BUCKETLENGTH + sizeof(int), SEEK_CUR);
				bucketcache.pos = sizeof(int);
				bn.available = 1;
				bn.key = k;
				//put_bytes(&bn, BNLENGTH, bucketfp);
				bucketcache.fwrite_cache(&bn, BNLENGTH, bcurrentpos);

				bucketcache.flush();
				//treecache.flush();

				return true;
			}

		} else {
			TreeNode oldtn;
			TreeNode newtn;
			BucketNode<K> bn;
			BucketNode<K> delbn;

			//建立一个新桶
			//fseek(bucketfp, 0, SEEK_END);
			bucketcache.pos = 0;
			//char * buffer = new char[BNLENGTH];
			//memset(buffer, 0, sizeof(buffer));
			//fwrite_number(0, bucketfp);
			bucketcache.fwrite_number(0, bcount);
			bn.available = 0;
			for (i = 0; i < BNCOUNT; i++) {
				//put_bytes(&bn, BNLENGTH, bucketfp);
				bucketcache.fwrite_cache(&bn, BNLENGTH, bcount);
			}
			//fwrite_number(-1, bucketfp);
			bucketcache.fwrite_number(-1, bcount);
			bucketcache.flush();
			//delete[] buffer;

			//定位到当前树节点
			seekTNPos(tcurrentpos);
			get_bytes(&oldtn, TNLENGTH, treefp);

			//分裂树节点
			//fseek(treefp, 0, SEEK_END);
			treecache.pos = 0;

			//写入一个新节点，指向原来的桶
			newtn.flag = oldtn.flag + 1;
			newtn.isBucket = true;
			newtn.nextPos0 = oldtn.nextPos0;
			//put_bytes(&newtn, TNLENGTH, treefp);
			treecache.fwrite_cache(&newtn, TNLENGTH, tcount);

			//写入一个新节点，指向新的桶
			newtn.nextPos0 = bcount;
			bcount++;
			//put_bytes(&newtn, TNLENGTH, treefp);
			treecache.pos = 0;
			treecache.fwrite_cache(&newtn, TNLENGTH, tcount + 1);

			//更改旧的树节点
			oldtn.nextPos0 = tcount;
			tcount++;
			oldtn.nextPos1 = tcount;
			tcount++;
			oldtn.isBucket = false;
			//seekTNPos(tcurrentpos);
			treecache.pos = 0;
			//put_bytes(&oldtn, TNLENGTH, treefp);
			treecache.fwrite_cache(&oldtn, TNLENGTH, tcurrentpos);
			treecache.flush();

			//修改配置文件
			fseek(configfp, 0, SEEK_SET);
			Config config;
			config.bcount = bcount;
			config.tcount = tcount;
			put_bytes(&config, CONFIGLENGTH, configfp);

			//将原来桶中的指1键值对移动到另一桶中
			int res = 10000;
			//seekBNPos(bcurrentpos);
			//fseek(bucketfp, sizeof(int), SEEK_CUR);
			bucketcache.pos = sizeof(int);
			for (i = 0; i < BNCOUNT; i++) {
				//get_bytes(&bn, BNLENGTH, bucketfp);
				bucketcache.fread_cache(&bn, BNLENGTH, bcurrentpos);
				if ((hash(bn.key) & AND[oldtn.flag]) != 0) {
					delbn.available = 0;
					//fseek(bucketfp, -BNLENGTH, SEEK_CUR);
					bucketcache.pos -= BNLENGTH;
					//put_bytes(&delbn, BNLENGTH, bucketfp);
					bucketcache.fwrite_cache(&delbn, BNLENGTH, bcurrentpos);
					bucketcache.flush();
					//seekBNPos(bcount - 1);
					int tmp1 = bcurrentpos;
					int tmp2 = bucketcache.pos;
					bcurrentpos = bcount - 1;
					bucketcache.pos = 0;

					addBucket(bn.key);

					bcurrentpos = tmp1;
					bucketcache.pos = tmp2;
					res--;
					//seekBNPos(bcurrentpos);
					//fseek(bucketfp, (i + 1) * BNLENGTH + sizeof(int), SEEK_CUR);
				}
			}
			bucketcache.pos = 0;
			bucketcache.fwrite_number(res, bcurrentpos);
			bucketcache.flush();

			//添加新键值对
			return putKey(k);

		}
	}

}

#endif
