

Fitness & Calorie Burn Recommendation System

A C++ Object-Oriented Programming Project

Overview

The Fitness & Calorie Burn Recommendation System is a console-based C++ application designed to help users track workouts, calculate estimated calorie burn, and receive personalized exercise recommendations.
The project demonstrates practical use of OOP principles, control structures, arrays, strings, pointers, inheritance, polymorphism, operator/function overloading, constructors, and exception handling.




Features

User Profile Creation (age, weight, gender, fitness goal)

Workout Selection Menu (Cardio, Strength, HIIT, Stretching)

Calorie Burn Calculator using formulas based on MET values

Workout Recommendations based on user goal and activity level

History Storage using arrays

Pointer-based calorie calculations

Exception handling for invalid inputs

OOP Concepts Used

Encapsulation (private data + setters/getters)

Inheritance (Base: Workout → Derived: Cardio, Strength, HIIT…)

Polymorphism (virtual functions for calorie calculation)

Operator Overloading (+ to combine workouts)

Function Overloading (multiple calculateCalories functions)

Constructors & Destructors

Use of arrays, strings, pointers, control structures, loops






How the System Works

1. Start the Program

The user launches the console application.
A welcome message loads the main menu.

2. Create a User Profile

The system asks for:

Age

Gender

Weight

Fitness Goal
These inputs are validated using exception handling.


3. Select Workout Type

User chooses from:

Cardio

Strength

HIIT

Stretching


Each workout is a derived class inheriting from the base Workout class.

4. Enter Duration

User specifies how long they exercised.
Pointers are used to calculate calorie burn dynamically.

5. System Generates Output

Estimated calories burned

Recommended next workout

Motivation message

History stored in an array


6. View Workout History

The user can display all previously logged workouts from the array.

7. Exit

Destructor triggers cleanup and exit message.


---

Technologies & Concepts

Category	Concepts Used

OOP Principles	Classes, objects, inheritance, polymorphism, abstraction, encapsulation
C++ Features	Constructors, destructors, exception handling, operator overloading
Data Structures	Arrays (1D & 2D), strings, pointers
Control Structures	If/Else, Switch, loops (for, while, do-while), break/continue
Algorithms	Calorie burn calculations using MET values



---

How to Compile

Use any C++ compiler:

g++

g++ main.cpp -o fitness
./fitness

Windows (MinGW)

g++ main.cpp -o fitness.exe
fitness.exe




Project Structure

/FitnessSystem
│── main.cpp
│── User.h / User.cpp
│── Workout.h / Workout.cpp
│── Cardio.h / Cardio.cpp
│── Strength.h / Strength.cpp
│── HIIT.h / HIIT.cpp
│── utils.h
│── README.md   
