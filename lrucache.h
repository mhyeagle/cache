#ifndef OWN_CACHE_LRUCACHE_H_
#define OWN_CACHE_LRUCACHE_H_

#include <iostream>
#include <string>
#include <iterator>
#include <unordered_map>

typedef struct LruNode {
    LruNode *prev = NULL;
    LruNode *next = NULL;
    std::string value;
} LruNode;

class LruCache {
public:
    LruCache();
    LruCache(unsigned long size);
    ~LruCache();
    int Get(int key, std::string &value);
    int Set(int key, std::string value);
    int Del(int key);

private:
    int MoveNodeHead(LruNode *node);
    int DelNode(LruNode *node);

    std::unordered_map<int, LruNode> cache_;
    LruNode *start_;
    LruNode *end_;
    unsigned long cache_size_;
};

#endif //OWN_CACHE_LRUCACHE_H_
