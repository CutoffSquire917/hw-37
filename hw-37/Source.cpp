#include <iostream>
#include <iomanip>
#include <string>
#include <typeinfo>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
using namespace std;

#pragma region First_realization

class Vehicle {
protected:
	string brand;
	string model;
	int year;
public:
	Vehicle(const string& brand, const string& model, const int year)
		: brand(brand), model(model), year(year) {
	}
	virtual void DisplayInfo() const {
		cout << "Name : " << brand << " " << model << "\n"
			<< "Year : " << year << "\n";
	}
};

class Car : public Vehicle {
private:
	int numberOfDoors;
	string fuelType;
public:
	Car(const string& brand, const string& model, const int year, const int numberOfDoors, const string& fuelType)
		: Vehicle(brand, model, year), numberOfDoors(numberOfDoors), fuelType(fuelType) {
	}
	void DisplayInfo() const override {
		Vehicle::DisplayInfo();
		cout << "Fuel type : " << fuelType << "\n"
			<< "Number of doors : " << numberOfDoors << endl;
	}
};
class Truck : public Vehicle {
private:
	double loadCapacity;
	string axleCount;
public:
	Truck(const string& brand, const string& model, const int year, const double loadCapacity, const string& axleCount)
		: Vehicle(brand, model, year), loadCapacity(loadCapacity), axleCount(axleCount) {
	}
	void DisplayInfo() const override {
		Vehicle::DisplayInfo();
		cout << "Load capacity : " << loadCapacity << "\n"
			<< "Axle count : " << axleCount << endl;
	}
};
class Motorcycle : public Vehicle {
private:
	double engineVolume;
public:
	Motorcycle(const string& brand, const string& model, const int year, const double engineVolume)
		: Vehicle(brand, model, year), engineVolume(engineVolume) {
	}
	void DisplayInfo() const override {
		Vehicle::DisplayInfo();
		cout << "Engine volume : " << engineVolume << endl;
	}
};

class AutoPark {
private:
	vector<unique_ptr<Vehicle>> cars;
public:
	AutoPark() {}
	void AddVehicle(unique_ptr<Vehicle> newVehicle) {
		cars.push_back(move(newVehicle));
	}
	void RemoveVehicle(const int index) {
		if (index < 0 || index >= cars.size()) {
			return;
		}
		cars.erase(cars.begin() + index);
	}
	void ShowAllVehicles() {
		for (size_t i = 0; i < cars.size(); i++)
		{
			if (typeid(*cars.at(i)) == typeid(Car)) {
				cout << "Car: " << endl;
			}
			else if (typeid(*cars.at(i)) == typeid(Truck)) {
				cout << "Truck: " << endl;
			}
			else if (typeid(*cars.at(i)) == typeid(Motorcycle)) {
				cout << "Motorcycle: " << endl;
			}
			else {
				cout << "Vehicle: " << endl;
			}
			cars.at(i)->DisplayInfo();
			cout << endl;
		}
	}
};

#pragma endregion

#pragma region Second_realization

class Room {
protected:
	int roomNumber;
	double pricePerNight;
	bool isBooked;
public:
	Room(const int roomNumber, const double pricePerNight, const bool isBooked)
		: roomNumber(roomNumber), pricePerNight(pricePerNight), isBooked(isBooked) {}
	virtual string GetRoomType() const = 0;
	virtual void PrintInfo() const {
		cout << "Room number: " << roomNumber << "\n"
			<< "Room price: " << pricePerNight << "$\n"
			<< "Available: " << (isBooked ? "false" : "true") << endl;
	}
	void BookRoom() {
		isBooked = true;
	}
	void FreeRoom() {
		isBooked = false;
	}
	int GetRoomNumber() const {
		return roomNumber;
	}
	double GetPricePerNight() const {
		return pricePerNight;
	}
	bool GetIsBooked() const {
		return isBooked;
	}
};

class SingleRoom : public Room {
public:
	SingleRoom(const int roomNumber, const double pricePerNight, const bool isBooked)
		: Room(roomNumber, pricePerNight, isBooked) {}
	string GetRoomType() const override {
		return "Single room";
	}
};
class DoubleRoom : public Room {
public:
	DoubleRoom(const int roomNumber, const double pricePerNight, const bool isBooked)
		: Room(roomNumber, pricePerNight, isBooked) {
	}
	string GetRoomType() const override {
		return "Double room";
	}
};
class SuiteRoom : public Room {
private:
	bool hasJacuzzi;
public:
	SuiteRoom(const int roomNumber, const double pricePerNight, const bool isBooked, const bool hasJacuzzi)
		: Room(roomNumber, pricePerNight, isBooked), hasJacuzzi(hasJacuzzi) {}
	string GetRoomType() const override {
		return "Suite room";
	}
	void PrintInfo() const override {
		cout << "Room number: " << roomNumber << "\n"
			<< "Room price: " << pricePerNight << "$\n"
			<< "Available: " << (isBooked ? "false" : "true") << "\n"
			<< "Jacuzzi: " << (hasJacuzzi ? "false" : "true") << endl;
	}
	bool GetHasJacuzzi() const {
		return hasJacuzzi;
	}
};

class HotelManager {
private:
	vector<unique_ptr<Room>> rooms;
public:
	HotelManager() {}
	void AddRoom(unique_ptr<Room> newRoom) {
		rooms.push_back(move(newRoom));
	}
	void BookRoom(const int roomNumber) {
		for (const auto& room : rooms)
		{
			if (room->GetRoomNumber() == roomNumber) {
				room->BookRoom();
				break;
			}
		}
	}
	void FreeRoom(const int roomNumber) {
		for (const auto& room : rooms)
		{
			if (room->GetRoomNumber() == roomNumber) {
				room->FreeRoom();
				break;
			}
		}
	}
	void ShowAvailableRooms() const {
		cout << "Rooms available: ";
		for (const auto& room : rooms)
		{
			if (!room->GetIsBooked()) {
				cout << room->GetRoomNumber() << ", ";
			}
		}
		cout << "\b\b.\n";
	}
	void ShowAllRooms() const {
		cout << "All rooms: " << endl;
		for (const auto& room : rooms)
		{
			cout << room->GetRoomType() << ": " << endl;
			room->PrintInfo();
			cout << endl;
		}
	}
	void FindByCriteria(const double price, const string& type, const bool available) {
		auto it = find_if(rooms.begin(), rooms.end(), [&](const unique_ptr<Room>& room) {
			return room->GetPricePerNight() == price &&
				room->GetRoomType() == type &&
				(!available || !room->GetIsBooked());
			});

		if (it != rooms.end()) {
			cout << "Found room: " << (*it)->GetRoomNumber() << " (" << (*it)->GetRoomType()
				<< ") - $" << (*it)->GetPricePerNight() << " per night.\n";
		}
		else {
			cout << "No rooms match the given criteria.\n";
		}
	}
};

class BookingSystem {
private:
	list<pair<string, int>> reservation;
public:
	BookingSystem() {}
	void MakeReservation(const string& clientName, const int roomNumber) {
		reservation.push_back(pair<string, int>(clientName, roomNumber));
	}
	void CancelReservation(const string& clientName) {
		for (auto client = reservation.begin(); client != reservation.end(); client++) {
			if (client->first == clientName) {
				client = reservation.erase(client);
				break;
			}
		}
	}
	void PrintReservations() const {
		cout << "Booked:" << endl;
		for (const auto& client : reservation)
		{
			cout << "Name: " << client.first << " - room number: " << client.second << endl;
		}
	}
};


//	Розробіть систему керування бронюванням готельних номерів, використовуючи ООП та динамічні контейнери(vector, list).
//
//	Умови:
//Базовий клас Room :
//
//Поля:
//roomNumber(номер кімнати).
//pricePerNight(ціна за ніч).
//isBooked(стан бронювання).
//Чисто віртуальний метод GetRoomType() const.
//Метод BookRoom(), який змінює стан isBooked.
//Метод FreeRoom(), який скасовує бронювання.
//Похідні класи :
//
//SingleRoom(одномісний).
//DoubleRoom(двомісний).
//SuiteRoom(люкс, додаткове поле hasJacuzzi).
//
//Клас HotelManager :
//
//Контейнер vector<unique_ptr<Room>>.
//Методи :
//	AddRoom(unique_ptr<Room>).
//	BookRoom(int roomNumber).
//	FreeRoom(int roomNumber).
//	ShowAvailableRooms() const.
// 
//	Клас BookingSystem :
//
//Контейнер list<pair<string, int>>, де :
//	string — ім’я клієнта.
//	int — номер кімнати.
//	Метод MakeReservation(string clientName, int roomNumber).
//	Метод CancelReservation(string clientName).
//	Додатково :
//
//	Можливість сортувати vector за ціною номерів(sort).
//	Пошук номерів по критеріях(find_if).


#pragma endregion

int main()
{
#pragma region First_usage

	//AutoPark autopark;
	//autopark.AddVehicle(make_unique<Car>("Audi", "A5", 2019, 4, "Gas"));
	//autopark.AddVehicle(make_unique<Truck>("Toyota", "FM", 2012, 1500.0, "6x2"));
	//autopark.AddVehicle(make_unique<Motorcycle>("Audi", "Ninja 650 MY", 2023, 68.0));
	//autopark.ShowAllVehicles();

#pragma endregion

#pragma region First_usage

	//HotelManager motel;
	//motel.AddRoom(make_unique<SingleRoom>(12, 12.00, false));
	//motel.AddRoom(make_unique<DoubleRoom>(4, 42.00, true));
	//motel.AddRoom(make_unique<SuiteRoom>(23, 100.00, false, true));
	//motel.BookRoom(12);
	//motel.ShowAllRooms();
	//motel.ShowAvailableRooms();
	//motel.FindByCriteria(100.00, "Suite room", true);
	//cout << "=============\n";
	//BookingSystem motelBook;
	//motelBook.MakeReservation("John Patison", 12);
	//motelBook.MakeReservation("Mr. Bob", 8);
	//motelBook.MakeReservation("Julia Baravkova", 9);
	//motelBook.CancelReservation("Mr. Bob");
	//motelBook.PrintReservations();

#pragma endregion

}