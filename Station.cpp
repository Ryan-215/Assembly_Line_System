#include <cstddef>
#include <string>
#include <iomanip>
#include <iostream>
#include "Station.h"
#include "Utilities.h"
namespace seneca {
    size_t Station::m_widthField = 0;
    int Station::id_generator = 0;

    // constructor with a parameter of string for data line
    Station::Station(const std::string& str) {
        Utilities ut;
        size_t next_pos = 0;
        bool keepExtract = true;
        // extract data from string
        m_id = ++id_generator;
        m_name = ut.extractToken(str, next_pos, keepExtract);
        m_startSN = std::stoul(ut.extractToken(str, next_pos, keepExtract));
        m_stockNum = std::stoul(ut.extractToken(str, next_pos, keepExtract));
        // set width field to the maximum value
        if (Station::m_widthField < ut.getFieldWidth())
            Station::m_widthField = ut.getFieldWidth();
        // extract description from string
        m_desc = ut.extractToken(str, next_pos, keepExtract);
    }

    // query the name of Station
    const std::string& Station::getItemName() const { return m_name; }

    // get the next SN
    size_t Station::getNextSerialNumber() { return m_startSN++; }

    // query the reamining quantity of items
    size_t Station::getQuantity() const { return m_stockNum; }

    // update quantity(decrease by 1)
    void Station::updateQuantity() { m_stockNum > 0 ? m_stockNum-- : 0; }

    // display data
    void Station::display(std::ostream& os, bool full) const {
        os << std::setw(3) << std::setfill('0') << std::right << m_id << " | " 
           << std::setw(m_widthField) << std::setfill(' ') << std::left << m_name << " | " 
           << std::setw(6) << std::setfill('0') << std::right << m_startSN << " | ";
        if (full) 
            os << std::setw(4) << std::setfill(' ') << m_stockNum << " | " << m_desc;
        os << std::endl;
    }
}