#ifndef SENECA_STATION_H
#define SENECA_STATION_H
namespace seneca {
    class Station {
        int m_id{};
        std::string m_name{};
        std::string m_desc{};
        unsigned int m_startSN{};
        size_t m_stockNum{};
        static size_t m_widthField;
        static int id_generator;
    
    public:
        Station(const std::string& str);
        const std::string& getItemName() const;
        size_t getNextSerialNumber();
        size_t getQuantity() const;
        void updateQuantity();
        void display(std::ostream& os, bool full) const;
    };
}
#endif