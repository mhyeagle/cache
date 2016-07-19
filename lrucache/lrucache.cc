#include "lrucache.h"

LruCache::LruCache() {
    start_ = NULL;
    end_ = NULL;
    cache_size_ = 0;
    cache_log_ = NULL;
}

LruCache::~LruCache() {
    cache_.clear();
    start_ = NULL;
    end_ = NULL;
    cache_log_ = NULL;
    log4cpp::Category::shutdown();
}

int LruCache::Init(const std::string &config_file) {
	using boost::property_tree::ptree;
	ptree config_tree;
	read_xml(config_file, config_tree);
	cache_size_ = config_tree.get("container.container_num", 100);

	std::string log4cpp_config = config_tree.get<std::string>("container.log4cpp");
	log4cpp::PropertyConfigurator::configure(log4cpp_config);
	log4cpp::Category& temp_log = log4cpp::Category::getInstance(std::string("cachelog"));
	cache_log_ = &temp_log;

	cache_log_->info("lrucache init success !, the cache size is: %d", cache_size_);

	return 0;
}

int LruCache::Get(int key, std::string &value) {
    std::unordered_map<int, LruNode>::iterator iter;
    iter = cache_.find(key);
    if (cache_.end() == iter) {
        cache_log_->info("The key %d is not exist in the cache.", key);
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
        node->prev = start_;
        node->next->prev = node;
        start_ = node;
    }
    cache_log_->debug("Move node head, the node value is %s.", node->value);

    return 0;
}

int LruCache::DelNode(LruNode *node) {
    //remove node from double list
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node = NULL;

    cache_log_->debug("Delete node which value is %s.", node->value);

    return 0;
}
