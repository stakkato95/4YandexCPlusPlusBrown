//
//  35_lru_cache_Solution.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/9/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include "35_lru_cache_Common.h"

#include <iostream>
#include <map>

using namespace std;

struct LruCacheItem {
    ICache::BookPtr book;
    size_t rank;
};

bool operator<(const LruCacheItem& l, const LruCacheItem& r) {
    return l.rank < r.rank;
    
}

class LruCache : public ICache {
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings)
    : booksUnpacker { move(books_unpacker) }, maxMemory { settings.max_memory }, maxRank { 0 }, currentMemory { 0 } {
        // реализуйте метод
    }
    
    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> g(m);
        
        if (contains(book_name)) {
            LruCacheItem& item = cache[book_name];
            item.rank = maxRank++;
            return item.book;
        }
        
        LruCacheItem item = { booksUnpacker->UnpackBook(book_name), maxRank++ };
        size_t bookSize = item.book->GetContent().size();
        
        if (ensurePlace(book_name, bookSize)) {
            cache[book_name] = item;
            rankCache[item.rank] = book_name;
        }
        
        return item.book;
    }
private:
    bool contains(const string& book_name) const {
        try {
            cache.at(book_name);
            return true;
        } catch (out_of_range e) {
            return false;
        }
    }
    
    bool ensurePlace(const string& book_name, size_t bookSize) {
        if (bookFitsIntoMemory(bookSize)) {
            currentMemory += bookSize;
            return true;
        }
        
        size_t newCurrentMemory = currentMemory;
        
        //printCache();
        for (map<size_t, string>::iterator it = begin(rankCache); it != end(rankCache); ++it) {
            newCurrentMemory -= getBookSize(it->second);
            
            if (newCurrentMemory + bookSize <= maxMemory) {
                ++it;
                
                for (map<size_t, string>::iterator deleteIt = begin(rankCache); deleteIt != it; ++deleteIt) {
                    cache.erase(deleteIt->second);
                }
                
                rankCache.erase(begin(rankCache), it);
                currentMemory = newCurrentMemory + bookSize;
                //printCache();
                return true;
            }
        }
        
        currentMemory = 0;
        cache.clear();
        return false;
    }
    
    bool bookFitsIntoMemory(size_t bookSize) {
        return currentMemory + bookSize <= maxMemory;
    }
    
    void printCache() {
        cout << "---------------------------------" << endl;
        for (const auto& [key, val] : rankCache) {
            cout << key << " - " << val << endl;
        }
        for (const auto& [key, val] : cache) {
            cout << key << " - " << val.rank << endl;
        }
        cout << "---------------------------------" << endl;
    }
    
    size_t getBookSize(const string& name) {
        return cache[name].book->GetContent().size();
    }
    
    size_t currentMemory;
    const size_t maxMemory;
    shared_ptr<IBooksUnpacker> booksUnpacker;
    
    size_t maxRank;
    map<string, LruCacheItem> cache;
    map<size_t, string> rankCache;
    
    mutex m;
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings) {
    return make_unique<LruCache>(books_unpacker, settings);
}

