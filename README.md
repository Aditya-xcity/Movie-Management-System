# 🎬 Movie Management System (C Language)

A **console-based Movie / Cinema Management System** built in **C**, designed to simulate real-world cinema operations such as movie scheduling, seat booking, payment handling, and record persistence.

This project follows **safe programming practices**, clean logic, and modular design — suitable for **B.Tech coursework, viva, and GitHub portfolio**.

---


## 🖼️ Demo Screenshots

## 📌 Features

### 👨‍💼 Admin Panel

* Secure admin login
* Add or update movies (hall-wise)
* Set movie details (date, time, prices)
* Add **Coming Soon** movies
* View all bookings
* View detailed movie & seat records
* Search bookings using Booking ID

### 👥 Customer Panel

* View currently running movies
* View coming soon movies
* Book tickets with **manual seat selection**
* Real-time seat availability display
* Payment simulation (Card / Mobile)
* Automatic bill generation
* View bookings using mobile number

### 💺 Seat Management

* Premium & Regular seats
* Seats are **locked only after successful payment**
* No seat loss on cancelled payment
* Seat status persists using file storage

### 💾 Data Persistence

* Movies stored in `movies.txt`
* Bookings stored in `bookings.txt`
* Booking ID auto-increments even after restart

---

## 🧠 Key Concepts Used

* Structures (`struct`)
* File handling (`fopen`, `fprintf`, `fgets`, `strtok`)
* Transaction-style seat booking logic
* Input validation & buffer handling
* Modular programming
* Git & version control hygiene

---

## 🔐 Admin Credentials

```text
Username: admin
Password: admin123
```

---

## ▶️ How to Compile & Run

### Using GCC (Linux / macOS)

```bash
gcc main.c -o movie
./movie
```

### On Windows (CMD / PowerShell)

```bash
gcc main.c -o movie.exe
movie.exe
```

> ⚠️ Make sure **GCC / MinGW** is installed and added to PATH.

---

## 📂 Project Structure

```text
Movie-Management-System/
│
├── main.c
├── README.md
├── .gitignore
│
│
└── (runtime files generated automatically)
    ├── movies.txt
    └── bookings.txt
```

---

---

## 🧪 Example Flow

1. Admin logs in
2. Admin adds a movie and sets pricing
3. Customer views available movies
4. Customer selects seats
5. Payment is processed
6. Booking is confirmed
7. Bill is displayed and saved

---

## 🚀 Future Enhancements

* Support for multiple halls & movies
* Dynamic seat pricing
* Password hashing for admin login
* GUI version (C + SDL / GTK)
* Database integration

---

## 👨‍🎓 Author

**Aditya Bhardwaj**
B.Tech – Computer Science Engineering

---

## 📜 License

This project is created for **educational purposes only**.

---

### 🏁 Final Note

This system is built with **clarity, safety, and real-world logic** in mind.

Ideal for:

* College evaluation
* Viva explanations
* GitHub portfolio showcase
