#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "CustomerOrder.h"
#include "Station.h"
#include "Utilities.h"
namespace seneca {
    size_t CustomerOrder::m_widthField = 0;

    // defualt constructor
    CustomerOrder::CustomerOrder(){}

    // constructor with one argument for data string
    CustomerOrder::CustomerOrder(const std::string& str) {
        // prepare for extraction
        Utilities ut;
        size_t next_pos = 0;
        bool keepExtract = true;

        // extract data from string
        m_name = ut.extractToken(str, next_pos, keepExtract);
        m_product = ut.extractToken(str, next_pos, keepExtract);

        // extract items
        // backup the start position for items
        size_t next_itemPos = next_pos;
        while (keepExtract) {
            // count number of items in the order
            ut.extractToken(str, next_pos, keepExtract);
            m_cntItem++;
        };
        // allocate memory for item array
        m_lstItem = new Item*[m_cntItem];
        // reset flag for extraction
        keepExtract = true;
        for (auto i = 0u; i < m_cntItem; i++)
            // allocate memory each item
            m_lstItem[i] = new Item(ut.extractToken(str, next_itemPos, keepExtract));

        // set width field to the maximum value
        if (CustomerOrder::m_widthField < ut.getFieldWidth())
            CustomerOrder::m_widthField = ut.getFieldWidth();
    }
    
    // raise exception if Copy Constructor called
    CustomerOrder::CustomerOrder(const CustomerOrder& src) { throw "Invoking Copy Constructor is not allowed"; }

    // move assignment operator
    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept {
        // check for self-assignment
        if (this != &src) {
            // clean up resources prevent memory leak
            for (auto i = 0u; i < m_cntItem; i++)
                delete m_lstItem[i];
            delete [] m_lstItem;
            m_lstItem = nullptr;

            // move data from src
            m_lstItem = src.m_lstItem;
            src.m_lstItem = nullptr;
            m_cntItem = src.m_cntItem;
            src.m_cntItem = 0;
            m_name = src.m_name;
            src.m_name.erase();
            m_product = src.m_product;
            src.m_product.erase();
        }
        return *this;
    }

    // move constructor
    CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept { *this = std::move(src); }

    // destructor
    CustomerOrder::~CustomerOrder() {
        for (auto i = 0u; i < m_cntItem; i++)
            delete m_lstItem[i];
        delete [] m_lstItem;
    }

    // query for checking if all order is filled
    bool CustomerOrder::isOrderFilled() const {
        return std::all_of(m_lstItem, m_lstItem + m_cntItem, [](const Item* item) {
            return item->m_isFilled == true;
        });

    }

    // query for checking if the item is filled
    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        if (std::any_of(m_lstItem, m_lstItem + m_cntItem, [&itemName](const Item* item) { 
            return item->m_itemName == itemName && item->m_isFilled == false; })) 
            return false;
        return true;
    }

    // load Item to the CustomerOrder from Station
    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        bool filled = false;
        for (auto i = 0u; i < m_cntItem && !filled; i++) {
            if (m_lstItem[i]->m_itemName == station.getItemName() && m_lstItem[i]->m_isFilled == false) {
                if (station.getQuantity() > 0) {
                    // fill one Item into CustomerOrder
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    // Stock in Station minus one
                    station.updateQuantity();
                    filled = true;
                    // display result
                    os << "    Filled ";
                } else {
                    // failed to fill 
                    os << "    Unable to fill ";
                }
                os << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
            }
        }
    }

    // display CustomerOrder
    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;
        for (auto i = 0u; i < m_cntItem; i++) {
            os << "[" << std::setw(6) << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] " 
               << std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName 
               << " - " << ((m_lstItem[i]->m_isFilled) ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }
}