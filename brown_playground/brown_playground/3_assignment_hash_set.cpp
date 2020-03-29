//
//  3_hash_set.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 3/29/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <forward_list>
#include <iterator>
#include <vector>

#include <cassert>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
    using BucketList = forward_list<Type>;
    
    explicit HashSet(size_t num_buckets, const Hasher& hasher = {}) : hasher_ { hasher } {
        buckets_ = vector<forward_list<Type>>(num_buckets);
    }

    void Add(const Type& value) {
        if (Has(value)) {
            return;
        }
        
        BucketList& bucket = getBucket(value);
        bucket.push_front(value);
    }
    
    bool Has(const Type& value) const {
        const BucketList& bucket = getBucket(value);
        return find(begin(bucket), end(bucket), value) != end(bucket);
    }
    
    void Erase(const Type& value) {
        getBucket(value).remove(value);
    }
    
    const BucketList& GetBucket(const Type& value) const {
        return getBucket(value);
    }
private:
    vector<forward_list<Type>> buckets_;
    const Hasher& hasher_;
    
    const BucketList& getBucket(const Type& value) const {
        size_t bucketNumber = hasher_(value) % buckets_.size();
        return buckets_[bucketNumber];
    }
    
    BucketList& getBucket(const Type& value) {
        size_t bucketNumber = hasher_(value) % buckets_.size();
        return buckets_[bucketNumber];
    }
};

struct IntHasher {
    size_t operator()(int value) const {
        //real hash-function from libc++, libstdc++.
        return value;
    }
};

struct TestValue {
    int value;

    bool operator==(TestValue other) const {
        return value / 2 == other.value / 2;
    }
};

struct TestValueHasher {
    size_t operator()(TestValue value) const {
        return value.value / 2;
    }
};

void TestSmoke() {
    HashSet<int, IntHasher> hash_set(2);
    hash_set.Add(3);
    hash_set.Add(4);

    assert(hash_set.Has(3));
    assert(hash_set.Has(4));
    assert(!hash_set.Has(5));

    hash_set.Erase(3);

    assert(!hash_set.Has(3));
    assert(hash_set.Has(4));
    assert(!hash_set.Has(5));

    hash_set.Add(3);
    hash_set.Add(5);

    assert(hash_set.Has(3));
    assert(hash_set.Has(4));
    assert(hash_set.Has(5));
}

void TestEmpty() {
    HashSet<int, IntHasher> hash_set(10);
    for (int value = 0; value < 10000; ++value) {
        assert(!hash_set.Has(value));
    }
}

void TestIdempotency() {
    HashSet<int, IntHasher> hash_set(10);
    hash_set.Add(5);
    assert(hash_set.Has(5));
    hash_set.Add(5);
    assert(hash_set.Has(5));
    hash_set.Erase(5);
    assert(!hash_set.Has(5));
    hash_set.Erase(5);
    assert(!hash_set.Has(5));
}

void TestEquivalence() {
    HashSet<TestValue, TestValueHasher> hash_set(10);
    hash_set.Add(TestValue{2});
    hash_set.Add(TestValue{3});

    assert(hash_set.Has(TestValue{2}));
    assert(hash_set.Has(TestValue{3}));

    const auto& bucket = hash_set.GetBucket(TestValue{2});
    const auto& three_bucket = hash_set.GetBucket(TestValue{3});
    assert(&bucket == &three_bucket);

    assert(1 == distance(begin(bucket), end(bucket)));
    assert(2 == bucket.front().value);
}

int main3() {
    TestSmoke();
    TestEmpty();
    TestIdempotency();
    TestEquivalence();
    return 0;
}
