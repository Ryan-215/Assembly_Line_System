#ifndef SENECA_WORKSTATION_H
#define SENECA_WORKSTATION_H
#include <deque>
#include "CustomerOrder.h"
#include "Station.h"
namespace seneca {
    extern std::deque<CustomerOrder> g_pending;
    extern std::deque<CustomerOrder> g_completed;
    extern std::deque<CustomerOrder> g_incomplete;

    class Workstation : public Station {
        std::deque<CustomerOrder> m_orders{};
        Workstation* m_pNextStation{};

    public:
        Workstation(const std::string& str);
        Workstation(const Workstation& src) = delete;
        Workstation(Workstation&& src) = delete;        
        Workstation& operator=(const Workstation& src) = delete;
        Workstation& operator=(Workstation&& src) = delete;
        void fill(std::ostream& os);
        bool attemptToMoveOrder();
        void setNextStation(Workstation* station = nullptr);
        Workstation* getNextStation() const;
        void display(std::ostream& os) const;
        Workstation& operator+=(CustomerOrder&& newOrder);
    };
}
#endif