# Airline-Booking-System

## 📌 Project Overview
This repository contains a lightweight, terminal-based **Booking and Management System** implemented in C++. The application provides an all-in-one console environment to handle user profiles, manage reservations, process simulated transactions, and present structured menus using customized terminal text formatting.

---

## 🚀 Key Features

* **User Authentication:** Secure registration and login flow for multiple user accounts (`readUser`, `login`, `Registration`).
* **Booking Engine:** Complete capability to create (`performBooking`), view (`readBooking`), and modify (`editBooking`) reservation logs.
* **Payment Processing Module:** Integrated step-by-step module to finalize and process transactions safely (`payment`).
* **Custom Terminal UI:** Enhanced terminal aesthetics featuring custom-drawn title screens and multi-color ANSI text formatting (`printBlue`, `printPink`).

---

## 🛠️ System Architecture

The software is structured into four main components to keep the console architecture modular and efficient:

```text
┌───────────────────────────────────────┐
│        Application Entry (main)       │
└───────────────────┬───────────────────┘
                    ▼
┌───────────────────────────────────────┐
│         Authentication Module         │
│     (Registration & Secure Login)     │
└───────────────────┬───────────────────┘
                    ▼
┌───────────────────────────────────────┐
│            Core Operations            │
│  (Booking, Modifications, Payments)   │
└───────────────────┬───────────────────┘
                    ▼
┌───────────────────────────────────────┐
│         Utility & UI Styling          │
│     (ANSI Rendering & Persistence)    │
└───────────────────────────────────────┘
