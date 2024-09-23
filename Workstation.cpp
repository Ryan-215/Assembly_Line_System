#include <iostream>
#include "Workstation.h"
namespace seneca {
    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    // constructor with data for creating station
    Workstation::Workstation(const std::string& str) : Station(str) {}

    // fill the order with current Workstation
    void Workstation::fill(std::ostream &os) {
        // if order is not empty, fill the first order with this Workstation
        if (!m_orders.empty())
            m_orders.front().fillItem(*this, os);
    }

    // try to move the first order to next Workstation
    bool Workstation::attemptToMoveOrder() {
        if (!m_orders.empty()) {
            // check for completion on current Workstation
            if (m_orders.front().isItemFilled(this->getItemName()) || !this->getQuantity()) {
                if (m_pNextStation) {
                    m_pNextStation->m_orders.push_back(std::move(m_orders.front()));
                } else if (m_orders.front().isOrderFilled()) {
                    g_completed.push_back(std::move(m_orders.front()));
                } else {
                    g_incomplete.push_back(std::move(m_orders.front()));
                }
                m_orders.pop_front();
                return true;
            }
        }
        return false;
    }

    // set the next Workstation
    void Workstation::setNextStation(Workstation *station) {
        m_pNextStation = station;
    }

    // query for the address of next Workstation
    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }

    // display the Stations chain
    void Workstation::display(std::ostream &os) const {
        os << this->getItemName() << " --> ";
        if (m_pNextStation != nullptr)
            os << m_pNextStation->getItemName() << std::endl;
        else
            os << "End of Line" << std::endl;
    }

    // receive an Order then put at the back of the queue
    Workstation& Workstation::operator+=(CustomerOrder &&newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }
}