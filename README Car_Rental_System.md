# 🚗 Car Rental System in C++ (OOP)

This is a console-based "Car Rental System" implemented in "C++" using "Object-Oriented Programming (OOP)" principles. It allows administrators to manage a fleet of vehicles and customers to rent or return them with proper receipt generation.

Features

- Admin login for managing the fleet.
- Customer registration with name, phone, and CNIC.
- Add new vehicles (Sedan/SUV) with daily rental rate.
- Display available or currently rented vehicles.
- Rent a vehicle for a number of days.
- Return a vehicle with damage reporting.
- Receipt generation saved to a file (`receipts.txt`).
- Persistent fleet data saved to `fleet.txt`.

---

Project Structure

```
.
├── car_rental.cpp        Main source code
├── fleet.txt             File to store fleet details
├── receipts.txt          File to store rental receipts
└── README.md             Project documentation
```

---

## 🚀 How to Run

1. Compile the code using g++:
   ```
   g++ -o car_rental car_rental.cpp
   ```

2. Run the program:
   ```
   ./car_rental
   ```

3. Administrator Credentials:
   ```
   Username: admin
   Password: password
   ```

---
Concepts Used

- Classes and Inheritance (`Vehicle`, `Car`, `Customer`)
- Pure Virtual Functions (Abstract Class)
- Dynamic Memory Allocation
- File Handling (`ifstream`, `ofstream`)
- Arrays for managing records
- Encapsulation of customer and vehicle details

---

Sample Functionalities

- Add Vehicle:
  ```
  Enter vehicle type (Sedan/SUV): SUV
  Enter vehicle make: Toyota
  Enter vehicle model: Fortuner
  Enter vehicle year: 2021
  Enter vehicle daily rate: 7500
  ```

- Rent Vehicle:
  ```
  Enter your name: Ali
  Enter your phone number: 03001234567
  Enter your CNIC: 35201-1234567-8
  Enter the vehicle number: 1
  Enter the number of days: 3
  ```

- Return Vehicle:
  ```
  Is there any damage to the vehicle? (1 for Yes, 0 for No): 1
  ```

---
Author
Rana Haseeb Ahmad

---
To-Do / Improvements

- [ ] Implement advanced customer authentication.
- [ ] Add GUI using Qt or a web front-end.
- [ ] Include payment gateway simulation.
- [ ] Store data in a database (e.g., SQLite or MySQL).

