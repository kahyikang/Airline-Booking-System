# Airline-Booking-System

## 📌 Project Overview

A console-based airline reservation system developed in C++. The system enables users to register accounts, manage flight bookings, perform payments, complete passenger check-in, and generate invoices through an integrated workflow.


## 🚀 Features

- User registration and login authentication
- Flight booking management
- Booking modification and updates
- Payment processing
- Passenger check-in
- Invoice generation
- Input validation for user and booking information
- File-based data storage

## Technologies Used

- C++
- Standard Template Library (STL)
- File Handling
- Structured Programming
- Windows Console API


## Project Structure

```text
airline.cpp
user.txt
<username>_Booking.txt
<username>_paymentCheckIn.txt
<username>_invoice.txt
```


## System Workflow

```text
Register / Login
       ↓
Book Flight
       ↓
Make Payment
       ↓
Check-In
       ↓
Generate Invoice
```


## Data Storage

The system stores data using text files:

| File | Description |
|--------|-------------|
| user.txt | User account information |
| username_Booking.txt | Flight booking records |
| username_paymentCheckIn.txt | Payment and check-in information |
| username_invoice.txt | Generated invoices |


## Validation

The system includes validation for:

- Email addresses
- Password strength
- Mobile phone numbers
- Dates
- Passport numbers
- Credit/Debit card information
- Payment security codes


## ⚙️ Getting Started

### Prerequisites

- Windows Operating System
- C++ Compiler (GCC, MinGW, Visual Studio, or equivalent)

### Compilation

```bash
g++ airline.cpp -o airline
```

### Run

```bash
./airline
```
