#include <vector>
#include <string>
#include <algorithm>

#include "OrderCache.h"

#include "CacheMemory.hpp"

class OrderCache : public OrderCacheInterface {

public:

    OrderCache(CacheMemory& cache) : m_cache(cache) {}
    ~OrderCache() {}

    void addOrder(Order order) {
        m_cache.add(order);
    }

    void cancelOrder(const std::string& orderId) {
        m_cache.remove(orderId);
    }

    void cancelOrdersForUser(const std::string& user) {
        std::vector<Order> orders = m_cache.getOrdersByUser(user);

        for (auto order : orders) {
            m_cache.remove(order.orderId());
        }
    }

    void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) {
        std::vector<Order> orders_by_security = m_cache.getOrdersBySecurity(securityId);
        std::vector<Order> orders_with_minimum_qty;

        std::copy_if(begin(orders_by_security), end(orders_by_security),
            std::back_inserter(orders_with_minimum_qty),
            [&](Order order) { return order.qty() >= minQty; });

        for (auto order : orders_by_security) {
            m_cache.remove(order.orderId());
        }
    }

    unsigned int getMatchingSizeForSecurity(const std::string& securityId) {
        std::vector<Order> orders = m_cache.getOrdersBySecurity(securityId);

        unsigned int matched = 0;

        for (auto order : orders) {
            order = m_cache.get(order.orderId()); // refresh order from cache

            std::vector<Order> orders_filtered;

            std::copy_if(begin(orders), end(orders),
                std::back_inserter(orders_filtered),
                [&](Order order_to_filter) {
                    return order_to_filter.company() != order.company()
                        && order_to_filter.side() != order.side();
                });

            for (auto& order_filtered : orders_filtered) {
                order_filtered = m_cache.get(order_filtered.orderId()); //refresh order from cache

                if (order.qty_matched() == order.qty()) break;

                //get quantity used in match
                unsigned int qty_used = order_filtered.match(order);

                if (qty_used > 0) {
                    order.qty_matched(order.qty_matched() + qty_used);

                    //update result
                    matched += qty_used;

                    //update cache
                    m_cache.update(order);
                    m_cache.update(order_filtered);
                }
            }
        }

        return matched;
    }

    std::vector<Order> getAllOrders() const {
        return m_cache.getOrders();
    }

private:
    CacheMemory m_cache;
};
