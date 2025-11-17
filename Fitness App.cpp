// fitness_app.cpp
// Fitness & Calorie Burn Recommendation System
// Demonstrates OOP, arrays, pointers, pointer arithmetic, inheritance, polymorphism,
// operator/function overloading, exception handling, constructors/destructors, file I/O.

// Compile: g++ -std=c++17 fitness_app.cpp -o fitness_app
// Run: ./fitness_app

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------
   Custom exception types
   --------------------------- */
class FitnessException : public exception {
    string msg;
public:
    FitnessException(const string &m): msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

/* ---------------------------
   Base Person class
   --------------------------- */
class Person {
protected:
    string name;
    int age;
    double weightKg;
    double heightCm;
    char gender; // 'M' or 'F'
public:
    // Default constructor
    Person(): name("Unknown"), age(18), weightKg(70.0), heightCm(170.0), gender('M') {
        cout << "[Person] default constructed\n";
    }
    // Parameterized constructor
    Person(const string &n, int a, double w, double h, char g)
        : name(n), age(a), weightKg(w), heightCm(h), gender(g) {
        cout << "[Person] parameterized constructed\n";
    }
    // Copy constructor
    Person(const Person &other) = default;
    // Destructor
    virtual ~Person() { cout << "[Person] destroyed: " << name << "\n"; }

    // Getters / setters
    string getName() const { return name; }
    void setName(const string &n) { name = n; }
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
    double getWeight() const { return weightKg; }
    void setWeight(double w) { weightKg = w; }
    double getHeight() const { return heightCm; }
    void setHeight(double h) { heightCm = h; }
    char getGender() const { return gender; }
    void setGender(char g) { gender = g; }

    // BMI helper
    double bmi() const {
        double h_m = heightCm / 100.0;
        if (h_m <= 0) throw FitnessException("Invalid height for BMI calculation");
        return weightKg / (h_m * h_m);
    }
};

/* ---------------------------
   User class (derived)
   --------------------------- */
class User : public Person {
    string fitnessGoal; // e.g., "Lose weight", "Build muscle", "Maintain"
public:
    // Demonstrate constructor chaining
    User(): Person(), fitnessGoal("Maintain") {
        cout << "[User] default constructed\n";
    }
    User(const string &n, int a, double w, double h, char g, const string &goal)
        : Person(n, a, w, h, g), fitnessGoal(goal) {
        cout << "[User] parameterized constructed\n";
    }
    ~User() { cout << "[User] destroyed: " << name << "\n"; }

    void setGoal(const string &g) { fitnessGoal = g; }
    string getGoal() const { return fitnessGoal; }
};

/* ---------------------------
   Abstract Workout base
   --------------------------- */
class Workout {
protected:
    string name;
    int durationMinutes;
    int intensity; // 1..10
public:
    Workout(const string &n = "Generic", int d = 30, int inten = 5)
        : name(n), durationMinutes(d), intensity(inten) {}
    virtual ~Workout() {}
    virtual double estimateCalories(const Person &p) const = 0; // pure virtual

    virtual string info() const {
        ostringstream oss;
        oss << name << " (" << durationMinutes << " min, intensity " << intensity << ")";
        return oss.str();
    }
    int getDuration() const { return durationMinutes; }
    string getName() const { return name; }
};

/* ---------------------------
   Cardio workout (derived)
   --------------------------- */
// Uses rough MET-like calculation: calories = MET * weightKg * durationHours
class Cardio : public Workout {
    double metValue; // metabolic equivalent for activity
public:
    Cardio(const string &n, int d, int inten, double met)
        : Workout(n,d,inten), metValue(met) {}
    // function overloading example: same name but different params
    double estimateCalories(const Person &p) const override {
        double hours = durationMinutes / 60.0;
        // intensity modifies MET slightly
        double metAdj = metValue * (1.0 + (intensity - 5) * 0.05);
        return metAdj * p.getWeight() * hours;
    }
    // overload: estimate with an extra intensity multiplier
    double estimateCalories(const Person &p, double extraMultiplier) const {
        double base = estimateCalories(p);
        return base * extraMultiplier;
    }
    string info() const override {
        return "Cardio - " + Workout::info();
    }
};

/* ---------------------------
   Strength workout (derived)
   --------------------------- */
class Strength : public Workout {
public:
    Strength(const string &n, int d, int inten) : Workout(n,d,inten) {}
    double estimateCalories(const Person &p) const override {
        // Approximate strength training burn (simplified)
        double hours = durationMinutes / 60.0;
        double base = 6.0; // avg MET for strength-like activity
        double metAdj = base * (1.0 + (intensity - 5) * 0.04);
        return metAdj * p.getWeight() * hours;
    }
    string info() const override {
        return "Strength - " + Workout::info();
    }
};

/* ---------------------------
   Flexibility workout (derived)
   --------------------------- */
class Flexibility : public Workout {
public:
    Flexibility(const string &n, int d, int inten) : Workout(n,d,inten) {}
    double estimateCalories(const Person &p) const override {
        double hours = durationMinutes / 60.0;
        double base = 3.0; // light MET
        return base * p.getWeight() * hours;
    }
    string info() const override {
        return "Flexibility - " + Workout::info();
    }
};

/* ---------------------------
   WorkoutPlan class - demonstrates operator overloading
   --------------------------- */
class WorkoutPlan {
    vector<Workout*> workouts; // polymorphic pointers
public:
    WorkoutPlan() {}
    ~WorkoutPlan() {
        // We do not delete workouts because ownership may be external;
        // for demo, assume plan owns them and delete:
        for (Workout* w : workouts) delete w;
        workouts.clear();
    }
    void add(Workout* w) { workouts.push_back(w); }
    double totalCaloriesFor(const Person &p) const {
        double total = 0.0;
        for (Workout* w : workouts) total += w->estimateCalories(p);
        return total;
    }
    void showPlan() const {
        cout << "Workout Plan (" << workouts.size() << " items):\n";
        for (const Workout* w : workouts) cout << "  - " << w->info() << "\n";
    }
    // operator+ to merge plans (returns new plan owning copies)
    WorkoutPlan operator+(const WorkoutPlan &other) const {
        WorkoutPlan result;
        // deep-copy by re-creating similar typed workouts (simple approach)
        for (Workout* w : workouts) {
            // attempt RTTI to copy appropriate type
            if (const Cardio* c = dynamic_cast<Cardio*>(w)) {
                result.add(new Cardio(*c));
            } else if (const Strength* s = dynamic_cast<Strength*>(w)) {
                result.add(new Strength(*s));
            } else if (const Flexibility* f = dynamic_cast<Flexibility*>(w)) {
                result.add(new Flexibility(*f));
            } else {
                // fallback: skip
            }
        }
        for (Workout* w : other.workouts) {
            if (const Cardio* c = dynamic_cast<Cardio*>(w)) {
                result.add(new Cardio(*c));
            } else if (const Strength* s = dynamic_cast<Strength*>(w)) {
                result.add(new Strength(*s));
            } else if (const Flexibility* f = dynamic_cast<Flexibility*>(w)) {
                result.add(new Flexibility(*f));
            }
        }
        return result;
    }
};

/* ---------------------------
   Logger - file I/O
   --------------------------- */
class Logger {
    string filename;
public:
    Logger(const string &fname = "fitness_log.txt"): filename(fname) {}
    void logSession(const Person &p, const Workout &w, double calories) {
        ofstream ofs(filename, ios::app);
        if (!ofs) throw FitnessException("Unable to open log file");
        ofs << "[" << chrono::system_clock::to_time_t(chrono::system_clock::now())
            << "] " << p.getName() << " did " << w.getName()
            << " for " << w.getDuration() << " min, calories: " << fixed << setprecision(2) << calories << "\n";
        ofs.close();
    }
};

/* ---------------------------
   FitnessApp controller
   --------------------------- */
class FitnessApp {
    User currentUser;
    Logger logger;
    // demonstrate arrays and pointers:
    // static 1D array of recommended durations for 3 intensity levels
    int recommendedDurations[3] = {20, 30, 45};
    // 2D array for sample weekly schedule (7 days x 3 slot types)
    string weeklySchedule[7][3];

public:
    FitnessApp(): currentUser(), logger("fitness_log.txt") {
        // initialize weeklySchedule with defaults
        for (int d=0; d<7; ++d) {
            weeklySchedule[d][0] = "Rest";
            weeklySchedule[d][1] = "Cardio";
            weeklySchedule[d][2] = "Strength";
        }
    }

    // pointer arithmetic demo (shows addresses and values)
    void pointerDemo() {
        int *p = recommendedDurations; // pointer to first element
        cout << "Pointer demo: recommendedDurations addresses and values\n";
        for (int i=0; i<3; ++i) {
            cout << "  idx " << i << " address " << (p + i) << " value " << *(p + i) << "\n";
        }
    }

    // create sample workouts (dynamically allocated to show pointer management)
    WorkoutPlan createSamplePlan() {
        WorkoutPlan plan;
        plan.add(new Cardio("Jogging", 30, 6, 7.0));
        plan.add(new Strength("Circuit training", 40, 7));
        plan.add(new Flexibility("Yoga", 20, 3));
        return plan;
    }

    // mapping user goal to recommended plan
    WorkoutPlan recommendPlanForUser(const User &u) {
        string g = u.getGoal();
        WorkoutPlan plan;
        if (g.find("Lose") != string::npos || g.find("lose") != string::npos) {
            plan.add(new Cardio("HIIT", 25, 9, 10.0));
            plan.add(new Strength("Full-body strength", 30, 7));
            plan.add(new Flexibility("Stretch", 15, 2));
        } else if (g.find("Build") != string::npos || g.find("build") != string::npos) {
            plan.add(new Strength("Hypertrophy", 50, 8));
            plan.add(new Cardio("Light cardio", 20, 4, 5.5));
            plan.add(new Flexibility("Mobility", 20, 3));
        } else {
            // maintain
            plan.add(new Cardio("Steady-state", 30, 5, 6.0));
            plan.add(new Strength("Maintenance strength", 30, 5));
        }
        return plan;
    }

    // main interactive menu (kept minimal, but demonstrates control structures)
    void runDemo() {
        cout << "=== Fitness & Calorie Burn Recommendation System ===\n";
        // create demo user
        currentUser = User("Devin M.", 22, 72.5, 175.0, 'M', "Lose weight");
        cout << "User: " << currentUser.getName() << ", Goal: " << currentUser.getGoal() << "\n";

        pointerDemo();

        WorkoutPlan recommended = recommendPlanForUser(currentUser);
        recommended.showPlan();
        double total = recommended.totalCaloriesFor(currentUser);
        cout << "Estimated total calories for plan: " << fixed << setprecision(2) << total << "\n";

        // demonstrate operator+ merging plans
        WorkoutPlan extras = createSamplePlan();
        WorkoutPlan merged = recommended + extras; // operator+
        cout << "\nMerged plan:\n";
        merged.showPlan();
        cout << "Merged calories estimate: " << merged.totalCaloriesFor(currentUser) << "\n";

        // log first workout
        try {
            // pick first workout pointer via internal copy: get pointer by creating sample again and delete
            WorkoutPlan single = createSamplePlan();
            // For demo, we extract first workout to log (ptr ownership tricky: we'll create a temp cardio)
            Cardio tempCardio("Temp Jog", 30, 6, 7.0);
            double cal = tempCardio.estimateCalories(currentUser);
            logger.logSession(currentUser, tempCardio, cal);
            cout << "Logged session: " << tempCardio.info() << " calories: " << cal << "\n";
        } catch (FitnessException &ex) {
            cerr << "Logging failed: " << ex.what() << "\n";
        }

        // demonstrate array (1D) traversal with control structures
        cout << "\nRecommended durations by intensity index:\n";
        for (int i=0; i<3; ++i) {
            if (recommendedDurations[i] < 30) cout << "  Short: ";
            else if (recommendedDurations[i] == 30) cout << "  Medium: ";
            else cout << "  Long: ";
            cout << recommendedDurations[i] << " min\n";
        }

        // 2D array display (weekly schedule)
        cout << "\nWeekly schedule sample (days x 3 slots):\n";
        const char* days[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
        for (int d=0; d<7; ++d) {
            cout << days[d] << ": ";
            for (int s=0; s<3; ++s) cout << weeklySchedule[d][s] << (s==2? "": " | ");
            cout << "\n";
        }

        // demonstrate string / pointer mixing and pointer arithmetic
        string sample = "HelloFitness";
        const char *cstr = sample.c_str();
        cout << "\nString pointer traversal: ";
        for (size_t i=0; i<sample.size(); ++i) {
            cout << *(cstr + i); // pointer arithmetic on C-string
        }
        cout << "\n";

        // Demonstrate exception: BMI with zero height
        try {
            User badUser("ZeroHeight", 30, 70, 0.0, 'F', "Maintain");
            cout << "Attempting BMI for " << badUser.getName() << "\n";
            cout << "BMI: " << badUser.bmi() << "\n"; // should throw
        } catch (FitnessException &ex) {
            cout << "Caught exception as expected: " << ex.what() << "\n";
        }

        cout << "\nDemo finished.\n";
    }
};

/* ---------------------------
   Main — Executes tests and demo
   --------------------------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Starting Fitness App demo...\n\n";
    FitnessApp app;
    app.runDemo();

    cout << "\nAll done. Check 'fitness_log.txt' for log entries.\n";
    return 0;
}
