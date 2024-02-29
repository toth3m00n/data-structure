from myqueue import Queue

class LRUCache:

    def __init__(self, n: int):
        self.hashTable = {}
        self.queue = Queue(n)
        self.capacity = n

    def get(self, key):
        return self.hashTable[key]

    def set(self, key, value):
        if self.queue.size % self.capacity == 0 and self.queue.size != 0:
            latest = self.queue.Front()
            print(latest)
            del self.hashTable[latest]
        self.queue.Push(key)
        self.hashTable[key] = value


cache = LRUCache(2)
cache.set("k1", "val1")
cache.set("k2", "val2")
cache.set("k3", "val3")
cache.set("k4", "val4")
cache.set("k5", "val5")

print(cache.hashTable, cache.queue)
