# OrderCache.hpp
This is a C++ implementation of an Order Cache class that uses a CacheMemory object to store and manage orders.

The class implements the OrderCacheInterface and provides methods to add and cancel orders, as well as to get and match orders based on different criteria.

The OrderCache class implements the interface by using a CacheMemory object to store the orders.

The class provides methods to add and remove orders from the cache, as well as to cancel orders for a given user or security ID.

There is also a method to get the number of orders that match a given security ID, as well as a method to get all the orders in the cache.

The code uses standard library functions like std::vector, std::string, std::copy_if, and std::back_inserter to implement the various methods.

Overall, this implementation provides a simple and efficient way to manage orders in a cache.
