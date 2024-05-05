#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

//The Following Code aims at creating a Car Rental Management System
//The code allows adminds and users reserve cars, look at data reports, and make return requests.




struct Car {        /*create DS for cars, customers and repairs*/
    string make;
    string model;
    int year;
    int mileage;
    string status;
    double rental_rate;
};

struct Customer {
    string name;
    string email;
    string phone;
};

struct Rental {
    Car* car;
    Customer* customer;
    time_t start_date;
    time_t end_date;
};

struct Repair {
    Car* car;
    time_t repair_date;
    double repair_cost;
};

class CarRentalService {            /*class containing methods to add cars, customers*/
private:
    vector<Car*> cars;              
    vector<Customer*> customers;        /*holds pointer to customer/car/rental.*/
    vector<Rental*> rentals;               /*used to store data in care rental service*/
    vector<Repair*> repairs;            /*vectors can dynamically manage and store many objects*/

public:
    void add_car(Car* car) {
        cars.push_back(car);
    }

    void add_customer(Customer* customer) {
        customers.push_back(customer);
    }

    void rent_car(Car* car, Customer* customer, time_t start_date, time_t end_date) {
        if (car->status == "available") {
            Rental* rental = new Rental{ car, customer, start_date, end_date };         
            car->status = "rented";                                                         /*adds rental data*/
            rentals.push_back(rental);
            cout << "Car rented successfully." << endl;
        }
        else {
            cout << "Sorry, the car is not available for rent." << endl;
        }
    }

    void return_car(Car* car) {
        for (int i = 0; i < rentals.size(); i++) {                      /*function for returning cars*/
            if (rentals[i]->car == car && rentals[i]->end_date == 0) {
                rentals[i]->end_date = time(0);
                car->status = "available";
                cout << "Car returned successfully." << endl;
                break;
            }
        }
    }

    void add_repair(Car* car, time_t repair_date, double repair_cost) {
        Repair* repair = new Repair{ car, repair_date, repair_cost };       /*function for adding repair data*/
        car->status = "in repair";
        repairs.push_back(repair);
        cout << "Repair added successfully." << endl;
    }

    void generate_utilization_report() {
        int rented_cars = rentals.size();
        int repaired_cars = repairs.size();                                /*untilization function*/
        int available_cars = cars.size() - rented_cars - repaired_cars;
        cout << "Number of rented cars: " << rented_cars << endl;
        cout << "Number of repaired cars: " << repaired_cars << endl;
        cout << "Number of available cars: " << available_cars << endl;
    }

    void generate_cost_report() {
        double repair_costs = 0;                                /*repair cost function*/
        for (int i = 0; i < repairs.size(); i++) {
            repair_costs += repairs[i]->repair_cost;        /*adds $50 to each repair*/
        }

        double rental_costs = 0;
        for (int i = 0; i < rentals.size(); i++) {          /*rental cost function*/
            if (rentals[i]->end_date != 0) {
                double rental_days = difftime(rentals[i]->end_date, rentals[i]->start_date) / (24 * 3600);
                rental_costs += rental_days * rentals[i]->car->rental_rate;
            }
        }

        cout << "Total repair costs: $" << repair_costs << endl;
        cout << "Total rental costs: $" << rental_costs << endl;
    }

    void generate_customer_satisfaction_report() {
        int satisfied_customers = 0;                    /*satisfaction report function*/
        for (int i = 0; i < rentals.size(); i++) {
            if (rentals[i]->end_date != 0) {
                satisfied_customers++;
            }
        }

        double satisfaction_rate = static_cast<double>(satisfied_customers) / rentals.size();
        cout << "Customer satisfaction rate: " << satisfaction_rate << endl;
    }

    const vector<Car*>& get_cars() const {      /*returns constant refrences to the vector objects*/
        return cars;                            /*does not modify values*/
    }

    const vector<Customer*>& get_customers() const {
        return customers;
    }
};

int main() {
    CarRentalService rental_service;                                    /*entering some test data*/
    Car car1{ "Toyota", "Camry", 2018, 20000, "available", 50.0 };
    Car car2{ "Honda", "Civic", 2020, 10000, "available", 45.0 };
    rental_service.add_car(&car1);
    rental_service.add_car(&car2);

    Customer customer1{ "John Smith", "john.smith@example.com", "555-1234" };       /*entering some test data*/
    Customer customer2{ "Jane Doe", "jane.doe@example.com", "555-5678" };
    rental_service.add_customer(&customer1);
    rental_service.add_customer(&customer2);

    int choice;
    while (true) {
        cout << "Welcome to the Car Rental System!\n";
        cout << "1. Rent a Car\n";
        cout << "2. Return a Car\n";
        cout << "3. Add Repair\n";
        cout << "4. Generate Utilization Report\n";
        cout << "5. Generate Cost Report\n";
        cout << "6. Generate Customer Satisfaction Report\n";
        cout << "7. Quit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Rent a Car
        {
            int car_choice;
            cout << "Available Cars:" << endl;
            const vector<Car*>& cars = rental_service.get_cars();
            for (int i = 0; i < cars.size(); i++) {
                cout << i + 1 << ". " << cars[i]->make << " " << cars[i]->model << endl;
            }
            cout << "Enter the car number to rent: ";
            cin >> car_choice;

            if (car_choice >= 1 && car_choice <= cars.size()) {
                Car* selected_car = cars[car_choice - 1];
                rental_service.rent_car(selected_car, &customer1, time(0), time(0) + 86400);
            }
            else {
                cout << "Invalid car selection. Please try again." << endl;          /*error handling*/
            }
        }
        break;
        case 2:
            // Return a Car
        {
            int car_choice;
            cout << "Rented Cars:" << endl;                             /*return car function*/
            const vector<Car*>& cars = rental_service.get_cars();
            for (int i = 0; i < cars.size(); i++) {
                if (cars[i]->status == "rented") {
                    cout << i + 1 << ". " << cars[i]->make << " " << cars[i]->model << endl;
                }
            }
            cout << "Enter the car number to return: ";
            cin >> car_choice;

            if (car_choice >= 1 && car_choice <= cars.size()) {
                Car* selected_car = cars[car_choice - 1];
                rental_service.return_car(selected_car);
            }
            else {
                cout << "Invalid car selection. Please try again." << endl;         /*error handling*/
            }
        }
        break;
        case 3:
            // Add Repair
        {
            int car_choice;
            cout << "Available Cars:" << endl;
            const vector<Car*>& cars = rental_service.get_cars();                   /*loop through available cars*/
            for (int i = 0; i < cars.size(); i++) {
                cout << i + 1 << ". " << cars[i]->make << " " << cars[i]->model << endl;
            }
            cout << "Enter the car number to add repair: ";
            cin >> car_choice;

            if (car_choice >= 1 && car_choice <= cars.size()) {
                Car* selected_car = cars[car_choice - 1];
                rental_service.add_repair(selected_car, time(0), 100.0);
            }
            else {
                cout << "Invalid car selection. Please try again." << endl;         /*error handling*/
            }
        }
        break;
        case 4:
            // Generate Utilization Report
            rental_service.generate_utilization_report();
            break;
        case 5:
            // Generate Cost Report
            rental_service.generate_cost_report();
            break;
        case 6:
            // Generate Customer Satisfaction Report
            rental_service.generate_customer_satisfaction_report();
            break;
        case 7:
            // Quit
            cout << "Thank you for using the Car Rental System. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;        /*error handling*/
            break;
        }

        cout << endl;
    }
}
