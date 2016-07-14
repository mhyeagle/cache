#include "lrucache.h"

LruCache::LruCache() {
    start_ = NULL;
    end_ = NULL;
    cache_size_ = 0;
}

LruCache::LruCache(unsigned long size) {
    start_ = NULL;
    end_ = NULL;
    cache_size_ = size;
}

LruCache::~LruCache() {
    cache_.clear();
    start_ = NULL;
    end_ = NULL;
}

int LruCache::Get(int key, std::string &value) {
    std::unordered_map<int, LruNode>::iterator iter;
    iter = cache_.find(key);
    if (cache_.end() == iter) {
        std::cout << "do not exit the element in cache, the key is: " << key << std::endl;
        return 1;
    } else {
        value = iter->second.value;
        //resize pointer
        MoveNodeHead(&iter->second);
    }
    return 0;
}

int LruCache::Set(int key, std::string value) {
    std::unordered_map<int, LruNode>::iterator iter;
    iter = cache_.find(key);
    LruNode node;
    if (cache_.end() == iter) {
        if (cache_.size() >= cache_size_) {
            DelNode(end_->prev);
        }
        node.value = value;
        //std::pair<std::unordered_map<int, LruNode>::iterator, bool> ret;
        auto ret = cache_.insert(std::pair<int, LruNode>(key, node));
        MoveNodeHead(&ret.first->second);
    } else {
        iter->second.value = value;
        MoveNodeHead(&iter->second);
    }
    return 0;
}

int LruCache::Del(int key) {
    std::unordered_map<int, LruNode>::iterator iter;
    iter = cache_.find(key);
    if (cache_.end() != iter) {
        DelNode(&iter->second);
        cache_.erase(iter);
    }

    return 0;
}

int LruCache::MoveNodeHead(LruNode *node) {
    if (NULL == node->next && NULL == node->prev) {
        //the node is not in the double list
        node->prev = start_;
        node->next = start_->next;
        node->next->prev = node;
        start_->next = node;
    } else if (start_ != node->prev) {
        //the node is in the double list
        //remove node from double list
        node->prev->next = node->next;
        node->next->prev = node->prev;
        //add node to first position of double list
        node->next = start_;
        node->prev = &start_;
        node->next->prev = node;
        start_ = node;
    }
    return 0;
}

int LruCache::DelNode(LruNode *node) {
    //remove node from double list
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node = NULL;

    return 0;
}
