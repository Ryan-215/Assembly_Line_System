#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "LineManager.h"
#include "Utilities.h"
namespace seneca {
    // load Workstations to LineManager based on the linkage of file
    LineManager::LineManager(const std::string &file, const std::vector<Workstation *> &stations) {
        std::ifstream fin(file);
        if (fin.is_open()) {
            Utilities ut;

            // 1. load linkages from file
            while (fin.good()) {
                std::string line{}, current{}, next{};
                size_t next_pos = 0;
                bool notLast = true;
                getline(fin, line);
                // extract current station'name
                current = ut.extractToken(line, next_pos, notLast);
                // find the iterator of current Workstation in collection
                auto it_Cur = std::find_if(stations.begin(), stations.end(), [current](Workstation* Workstation) {
                    return (Workstation->getItemName() == current);
                });
                // check for Assembly Line file mismatched with Workstations
                if (it_Cur == stations.end())
                    throw std::string("Assembly Line file dont't match with Workstations");

                // check for the last station
                if (notLast) {
                    // extract next station's name or next keep empty
                    next = ut.extractToken(line, next_pos, notLast);
                    // find the iterator of next Workstation in collection
                    auto it_Nxt = std::find_if(stations.begin(), stations.end(), [next](Workstation* Workstation) {
                        return (Workstation->getItemName() == next);
                    });
                    // check for Assembly Line file mismatched with Workstations
                    if (it_Nxt == stations.end())
                        throw std::string("Assembly Line file dont't match with Workstations");
                    // pair next station with current station
                    (*it_Cur)->setNextStation(*it_Nxt);
                }
                m_activeLine.push_back(*it_Cur);
            }

            // 2. Set the firstStation
            // find the current station that does not shows in the next station
            bool found{false};
            std::for_each(stations.begin(), stations.end(), [stations, this, &found](Workstation* stationForCurrent) {
                // compare the current station name with all the next station name
                bool isFirstStation = std::none_of(stations.begin(), stations.end(), [stationForCurrent](Workstation* stationForNext) {
                    return (stationForCurrent == stationForNext->getNextStation());
                });

                if (isFirstStation) {
                    m_firstStation = stationForCurrent;
                    found = true;
                }
            });
            if (!found) {
                throw std::string("Cannot find the first Workstation in station collection");
            }

            // 3. Set the number of Orders
            m_cntCustomerOrder = g_pending.size();
        } else {
            throw std::string("Failed to open Assembly Line file.");
        }
    }

    // reorder Workstations as the assembly line
    void LineManager::reorderStations() {
        std::vector<Workstation*> temp{};
        Workstation* curStation = m_firstStation;
        temp.push_back(curStation);
        while(curStation->getNextStation()){
            temp.push_back(curStation->getNextStation());
            curStation = curStation->getNextStation();
        }
        m_activeLine = temp;
    }

    // run the assembly line to fill items
    bool LineManager::run(std::ostream& os) {
        static size_t counter{1};
        // display function called times
        os << "Line Manager Iteration: "<< counter++ << std::endl;
        // put the customer order from pending to the first Workstation
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            // remove from the pending queue
            g_pending.pop_front();
        }

        // all Workstation fill once
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* Workstation) {
            Workstation->fill(os);
        });

        // all Workstation attempte to move order
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* Workstation) {
            Workstation->attemptToMoveOrder();
        });

        // check for complete all order
        auto runStatus = (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
        return runStatus;
    }

    // display the current assmbly line
    void LineManager::display(std::ostream& os) const {
        std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* Workstation) {
            Workstation->display(os);
        });
    }
}