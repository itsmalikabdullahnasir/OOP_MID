#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <fstream>

class Booking;  // Forward declaration

class Member {
private:
    std::string name;
    std::vector<Booking> bookings;

public:
    Member(std::string name) : name(name) {}

    std::string getName() const { return name; }
    std::vector<Booking>& getBookings() { return bookings; }

    void addBooking(const Booking& booking);
};

class FitnessClass {
private:
    std::string name;
    int maxCapacity;
    std::vector<bool> availableSlots;
    std::unordered_map<std::string, Member*> members;

public:
    FitnessClass(std::string name, int maxCapacity, int numSlots) {
        this->name = name;
        this->maxCapacity = maxCapacity;
        for (int i = 0; i < numSlots; i++) {
            availableSlots.push_back(true);
        }
    }

    std::string getName() const {
        return name;
    }

    bool bookSlot(std::string memberName, int slotIndex);
    void printAvailableSlots(std::ostream& os) const;
    void printBookings() const;

    void cancelBooking(std::string basicString, int i);

private:
    Member* getOrCreateMember(const std::string& memberName);
};

class Booking {
private:
    std::string memberName;
    FitnessClass* bookedClass;
    int slotIndex;
    time_t bookingTime;

public:
    Booking(std::string memberName, FitnessClass* bookedClass, int slotIndex)
            : memberName(memberName), bookedClass(bookedClass), slotIndex(slotIndex) {
        bookingTime = time(nullptr);
    }

    std::string getMemberName() const { return memberName; }
    FitnessClass* getBookedClass() const { return bookedClass; }
    int getSlotIndex() const { return slotIndex; }
    time_t getBookingTime() const { return bookingTime; }

    void printConfirmation() const {
        std::cout << "Booking confirmed for " << memberName << ":" << std::endl;
        std::cout << "Class: " << bookedClass->getName() << std::endl;
        std::cout << "Slot: " << slotIndex << std::endl;
        std::cout << "Booking Time: " << ctime(&bookingTime);
    }
};

void Member::addBooking(const Booking& booking) {
    bookings.push_back(booking);
}

bool FitnessClass::bookSlot(std::string memberName, int slotIndex) {
    if (slotIndex >= 0 && slotIndex < availableSlots.size() && availableSlots[slotIndex]) {
        availableSlots[slotIndex] = false;
        Member* member = getOrCreateMember(memberName);
        Booking booking(memberName, this, slotIndex);
        member->addBooking(booking);
        return true;
    }
    return false;
}

void FitnessClass::printAvailableSlots(std::ostream& os) const {
    os << "Available slots for " << name << ":" << std::endl;
    for (int i = 0; i < availableSlots.size(); i++) {
        os << "Slot " << i << ": " << (availableSlots[i] ? "Available" : "Booked") << std::endl;
    }
}

void FitnessClass::printBookings() const {
    std::cout << "Bookings for " << name << ":" << std::endl;
    for (const auto& pair : members) {
        Member *member = pair.second;
        std::cout << "Member: " << member->getName() << std::endl;
        for (const Booking& booking : member->getBookings()) {
            std::cout << "  Slot: " << booking.getSlotIndex() << std::endl;
        }
    }
}

Member* FitnessClass::getOrCreateMember(const std::string& memberName) {
    auto it = members.find(memberName);
    if (it != members.end()) {
        return it->second;
    }
    Member* newMember = new Member(memberName);
    members[memberName] = newMember;
    return newMember;
}

void FitnessClass::cancelBooking(std::string basicString, int i) {
    Member* member = getOrCreateMember(basicString);
    std::vector<Booking>& bookings = member->getBookings();
    if (i >= 0 && i < bookings.size()) {
        Booking& booking = bookings[i];
        availableSlots[booking.getSlotIndex()] = true;
        bookings.erase(bookings.begin() + i);
    }
}

int main() {

    std::ofstream outputFile("output.txt");

    if (!outputFile) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    FitnessClass yoga("Yoga", 20, 10);
    FitnessClass zumba("Zumba", 30, 15);

    std::string memberName;
    int classChoice, slotChoice, bookingChoice;

    while (true) {
        std::cout << "\nWelcome to the Fitness Class Booking System!" << std::endl;
        outputFile << "\nWelcome to the Fitness Class Booking System!" << std::endl;
        std::cout << "1. Book a class" << std::endl;
        outputFile << "1. Book a class" << std::endl;
        std::cout << "2. Cancel a booking" << std::endl;
        outputFile << "2. Cancel a booking" << std::endl;
        std::cout << "3. View class bookings" << std::endl;
        outputFile << "3. View class bookings" << std::endl;
        std::cout << "4. Exit" << std::endl;
        outputFile << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        outputFile << "Enter your choice: ";
        std::cin >> bookingChoice;

        switch (bookingChoice) {
            case 1: {
                std::cout << "Select a class:" << std::endl;
                outputFile << "Select a class:" << std::endl;
                std::cout << "1. Yoga" << std::endl;
                outputFile << "1. Yoga" << std::endl;
                std::cout << "2. Zumba" << std::endl;
                outputFile << "2. Zumba" << std::endl;
                std::cout << "Enter the class number: ";
                outputFile << "Enter the class number: ";
                std::cin >> classChoice;

                FitnessClass* selectedClass;
                switch (classChoice) {
                    case 1:
                        selectedClass = &yoga;
                        break;
                    case 2:
                        selectedClass = &zumba;
                        break;
                    default:
                        std::cout << "Invalid class choice." << std::endl;
                        outputFile << "Invalid class choice." << std::endl;
                        continue;
                }

                selectedClass->printAvailableSlots(outputFile);
                std::cout << "Enter the slot number you want to book: ";
                outputFile << "Enter the slot number you want to book: ";
                std::cin >> slotChoice;

                std::cout << "Enter your name: ";
                outputFile << "Enter your name: ";
                std::cin >> memberName;

                if (selectedClass->bookSlot(memberName, slotChoice - 1)) {
                    std::cout << "Booking successful!" << std::endl;
                    outputFile << "Booking successful!" << std::endl;
                } else {
                    std::cout << "Failed to book the selected slot." << std::endl;
                    outputFile << "Failed to book the selected slot." << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "Select a class:" << std::endl;
                outputFile << "Select a class:" << std::endl;
                std::cout << "1. Yoga" << std::endl;
                outputFile << "1. Yoga" << std::endl;
                std::cout << "2. Zumba" << std::endl;
                outputFile << "2. Zumba" << std::endl;
                std::cout << "Enter the class number: ";
                outputFile << "Enter the class number: ";
                std::cin >> classChoice;

                FitnessClass* selectedClass;
                switch (classChoice) {
                    case 1:
                        selectedClass = &yoga;
                        break;
                    case 2:
                        selectedClass = &zumba;
                        break;
                    default:
                        std::cout << "Invalid class choice." << std::endl;
                        outputFile << "Invalid class choice." << std::endl;
                        continue;
                }

                selectedClass->printBookings();
                std::cout << "Enter your name: ";
                outputFile << "Enter your name: ";
                std::cin >> memberName;

                std::cout << "Enter the booking number you want to cancel: ";
                outputFile << "Enter the booking number you want to cancel: ";
                std::cin >> slotChoice;

                selectedClass->cancelBooking(memberName, slotChoice - 1);
                break;
            }
        }
    }

    outputFile.close();

    return 0;
}