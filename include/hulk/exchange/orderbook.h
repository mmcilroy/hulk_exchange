
#ifndef _hulk_exchange_orderbook_h_
#define _hulk_exchange_orderbook_h_

#include "hulk/exchange/order.h"
#include <functional>
#include <map>

namespace hulk {
namespace exchange {

class orderbook
{
public:
    typedef std::multimap< px, order*, std::greater< px > > buy_orders;
    typedef std::multimap< px, order*, std::less< px > > sell_orders;

    struct callback
    {
        virtual void on_add( const orderbook& book, const order& order ) {}
        virtual void on_del( const orderbook& book, const order& order ) {}
    };

    orderbook()
    : _cb( 0 )
    {
    }

    void set_callback( callback& cb )
    {
        _cb = &cb;
    }

    void add( order& order )
    {
        if( order._side == BUY ) {
            _buy_orders.insert( buy_orders::value_type( order._px, &order ) );
        } else {
            _sell_orders.insert( sell_orders::value_type( order._px, &order ) );
        }

        //if( _cb ) {
        //    _cb->on_add( *this, order );
        //}
    }

    void del( order& order )
    {
        if( order._side == BUY ) {
            del_order( _buy_orders, order );
        } else {
            del_order( _sell_orders, order );
        }

        //if( _cb ) {
        //    _cb->on_del( *this, order );
        //}
    }

    template< typename TIter >
    void del( TIter it )
    {
        if( it->second->_side == BUY ) {
            _buy_orders.erase( it );
        } else {
            _sell_orders.erase( it );
        }

        //if( _cb ) {
        //    _cb->on_del( *this, *it->second );
        //}
    }

    buy_orders& get_buy_orders() {
        return _buy_orders;
    }

    sell_orders& get_sell_orders() {
        return _sell_orders;
    }

private:
    template< typename TOrders >
    void del_order( TOrders& orders, order& order )
    {
        typename TOrders::iterator it = orders.begin();
        while( it != orders.end() )
        {
            typename TOrders::iterator prev = it++;
            if( prev->second == &order ) {
                orders.erase( prev );
            }
        }
    }

    buy_orders _buy_orders;
    sell_orders _sell_orders;
    callback* _cb;
};

}
}

#endif