# Assembly Line System
The Assembly Line System is a robust C++ application designed to efficiently manage customer orders, inventory, and order processing. Leveraging advanced classes and functionalities, the system streamlines every aspect of the order fulfillment workflow.

## Key Features
### Dynamic Order Processing
The system employs dynamic memory allocation and move semantics to optimize order processing. Memory is allocated at runtime, ensuring flexible and efficient management compared to static allocation. Move semantics further enhance performance by transferring resources (like memory) between objects without unnecessary copying, reducing overhead and improving system efficiency.
```C++
// Example code demonstrating dynamic memory allocation and move semantics
// CustomerOrder constructor
CustomerOrder::CustomerOrder(const std::string& str) {
    // ...
    while (keepExtract) {
        // Allocate memory dynamically for new Item objects
        // ...
    }
}

// Move constructor for CustomerOrder
CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept {
    *this = std::move(src); 
}
```

### Customer Order Handling
The CustomerOrder class manages and represents customer orders within the system. It stores critical information, including item details and order status, while encapsulating the logic for processing orders, such as filling items and verifying their status.
```C++
// Example code demonstrating CustomerOrder class
class CustomerOrder {
    public:
        // Constructor
        CustomerOrder(const std::string& str);

        // Method to fill items in the order
        void fillItem(Station& station, std::ostream& os);

        // Method to check if the order is filled
        bool isOrderFilled() const;

        // Method to check if a specific item in the order is filled
        bool isItemFilled(const std::string& itemName) const;
};
```

### Line Management
The LineManager class manages the flow of orders across workstations in the assembly line. It ensures efficient order processing by coordinating the movement of orders between stations, optimizing the organization and workflow to streamline the overall order fulfillment process.
```C++
// Example code demonstrating LineManager class
class LineManager {
    public:
        // Constructor
        LineManager(const std::string& file, const std::vector<Workstation*>& stations);

        // Method to reorder stations based on configuration
        void reorderStations();

        // Method to run the assembly line and process orders
        bool run(ostream& os);

        // Method to display the status of stations in the assembly line
        void display(std::ostream& os) const;
};
```

### Workstation Functionality
The Workstation class models individual workstations tasked with processing orders and updating inventory. It encapsulates all the operations needed to process orders, including item fulfillment and advancing orders to the next station in the assembly line.
```C++
// Example code demonstrating Workstation class
class Workstation {
    public:
        // Method to fill items at the workstation
        void fill(std::ostream& os);

        // Method to attempt to move an order to the next station
        bool attemptToMoveOrder();

        // Method to set the next station in the assembly line
        void setNextStation(Workstation* station = nullptr);

        // Method to display the status of the workstation
        void display(std::ostream& os) const;
};
```

## Example
Testing command: ```main Stations1.txt Stations2.txt CustomerOrders.txt AssemblyLine.txt```  


### Input Files
- AssemblyLine.txt: Specifies the configuration of the assembly line, indicating the order in which stations are connected.
- CustomerOrders.txt: Contains customer orders to be processed by the system.
- Stations1.txt and Stations2.txt: Provide information about the stations involved in the fulfillment process, including item names, serial numbers, quantities, and descriptions.

### Output
Upon executing the Order Fulfillment System, the program displays processed orders, current inventory status, and all relevant information.
