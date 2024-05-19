#include <iostream>
#include <string>
#include <vector>
#include <ctime> // For handling dates
#include <fstream> // For file handling
#include <limits> // For numeric limits

using namespace std;

// Define structure for maintenance tasks
struct MaintenanceTask {
    string description;
    tm dueDate;
    bool completed;

    MaintenanceTask(string desc, tm due) : description(desc), dueDate(due), completed(false) {}
};

// Define structure for vehicle
struct Vehicle {
    string licensePlate;
    string brand;
    string color;
    bool available;
    string condition;
    Vehicle(string lp, string b, string c, bool av, string cond)
        : licensePlate(lp), brand(b), color(c), available(av), condition(cond) {}
};

// Base class for car
class Car {
protected:
    string loaiXe;
    vector<MaintenanceTask> maintenanceSchedule; // Maintenance schedule for each vehicle
public:
    Car(string loaiXe) : loaiXe(loaiXe) {}
    virtual double giaThueNgay() const = 0; // Pure virtual function for daily rental rate
    virtual ~Car() {} // Virtual destructor for proper cleanup
    string getLoaiXe() const { return loaiXe; }

    // Method to add maintenance task to the schedule
    void addMaintenanceTask(const string& description, const tm& dueDate) {
        maintenanceSchedule.push_back(MaintenanceTask(description, dueDate));
    }

    // Method to display maintenance schedule for the car
    void displayMaintenanceSchedule() const {
        cout << "Maintenance Schedule for " << loaiXe << " - " << getLoaiXe() << endl;
        for (const auto& task : maintenanceSchedule) {
            cout << "Description: " << task.description << endl;
            cout << "Due Date: " << task.dueDate.tm_mday << "/" << task.dueDate.tm_mon + 1 << "/" << task.dueDate.tm_year + 1900 << endl;
            cout << "Status: " << (task.completed ? "Completed" : "Pending") << endl;
            cout << "-----------------------------------------" << endl;
        }
    }
};

// Derived classes for 4-seater and 7-seater cars
class Car4Seater : public Car {
public:
    Car4Seater() : Car("4-seater") {}
    double giaThueNgay() const override {
        return 1000;
    }
};

class Car7Seater : public Car {
public:
    Car7Seater() : Car("7-seater") {}
    double giaThueNgay() const override {
        return 2000;
    }
};

// Function to read a date from the input
tm nhapNgay(const string& prompt) {
    tm date = {};
    cout << prompt;
    cout << " (dd mm yyyy): ";
    cin >> date.tm_mday >> date.tm_mon >> date.tm_year;
    date.tm_mon--; // tm_mon is 0-based in struct tm
    date.tm_year -= 1900; // tm_year is year since 1900 in struct tm
    return date;
}

// Base class to represent a customer
class KhachHang {
protected:
    string ten;
    string diaChi;
    string SDT;
    string hangXe;
    Car* loaiXe; // Pointer to Car
    tm ngayThue;
    tm ngayTra;
    Vehicle* xe;
public:
    // Constructor to initialize customer details
    KhachHang(string ten, string diaChi, string sdt, string hangXe, Car* loaiXe, tm ngayThue, tm ngayTra, Vehicle* xe) {
        this->ten = ten;
        this->diaChi = diaChi;
        this->SDT = sdt;
        this->hangXe = hangXe;
        this->loaiXe = loaiXe;
        this->ngayThue = ngayThue;
        this->ngayTra = ngayTra;
        this->xe = xe;
        this->xe->available = false;
    }

    // Method to display customer information
    virtual void layThongTinKH() const {
        cout << "Tên: " << ten << endl;
        cout << "Địa chỉ: " << diaChi << endl;
        cout << "Số điện thoại: " << SDT << endl;
        cout << "Hãng xe muốn thuê: " << hangXe << endl;
        cout << "Loại xe: " << loaiXe->getLoaiXe() << endl;
        cout << "Biển số xe: " << xe->licensePlate << endl;
        cout << "Ngày thuê: " << ngayThue.tm_mday << "/" << ngayThue.tm_mon + 1 << "/" << ngayThue.tm_year + 1900 << endl;
        cout << "Ngày trả: " << ngayTra.tm_mday << "/" << ngayTra.tm_mon + 1 << "/" << ngayTra.tm_year + 1900 << endl;
        cout << "Số ngày thuê: " << soNgayThue() << endl;
        cout << "Tổng tiền thuê: $" << tinhTienThue() << endl;
    }

    // Method to calculate the number of rental days
    int soNgayThue() const {
        time_t tThue = mktime(const_cast<tm*>(&ngayThue));
        time_t tTra = mktime(const_cast<tm*>(&ngayTra));
        double seconds = difftime(tTra, tThue);
        return seconds / (60 * 60 * 24); // Convert seconds to days
    }

    // Method to calculate the rental cost
    virtual double tinhTienThue() const {
        return soNgayThue() * loaiXe->giaThueNgay();
    }

    virtual ~KhachHang() {
        xe->available = true;
        delete loaiXe;
    } // Virtual destructor for proper cleanup
};

// Derived class to represent a VIP customer
class KhachHangVIP : public KhachHang {
private:
    double discountRate; // Discount rate for VIP customers
public:
    KhachHangVIP(string ten, string diaChi, string sdt, string hangXe, Car* loaiXe, tm ngayThue, tm ngayTra, Vehicle* xe, double discountRate)
        : KhachHang(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra, xe), discountRate(discountRate) {}

    // Overridden method to display VIP customer information
    void layThongTinKH() const override {
        KhachHang::layThongTinKH();
        cout << "Loại khách hàng: VIP" << endl;
        cout << "Tỷ lệ giảm giá: " << discountRate * 100 << "%" << endl;
        cout << "Tổng tiền thuê sau giảm giá: $" << tinhTienThue() << endl;
    }

    // Overridden method to calculate the rental cost with discount
    double tinhTienThue() const override {
        return KhachHang::tinhTienThue() * (1 - discountRate);
    }

    // Method to calculate the original rental cost before discount
    double tinhTienThueGoc() const {
        return KhachHang::tinhTienThue();
    }
};

// Function to print a bill with borders
void inHoaDon(const KhachHang* kh) {
    cout << "-----------------------------------------" << endl;
    cout << "|               HÓA ĐƠN                 |" << endl;
    cout << "-----------------------------------------" << endl;
    kh->layThongTinKH();

    double originalCost = kh->tinhTienThue();
    double discountedCost = originalCost;

    const KhachHangVIP* vipCustomer = dynamic_cast<const KhachHangVIP*>(kh);
    if (vipCustomer) {
        originalCost = vipCustomer->tinhTienThueGoc();
        discountedCost = vipCustomer->tinhTienThue();
        cout << "Tổng tiền thuê (trước giảm giá): $" << originalCost << endl;
        cout << "Tổng tiền thuê sau giảm giá: $" << discountedCost << endl;
    } else {
        cout << "Tổng tiền thuê: $" << originalCost << endl;
    }

    cout << "-----------------------------------------" << endl;
    char damage;
    cout << "Khách hàng có gây hư hỏng xe không (y/n)? ";
    cin >> damage;
    double insuranceCost = 0;
    if (damage == 'y' || damage == 'Y') {
        char tier;
        cout << "Nhập mức độ hư hỏng (A/B/C): ";
        cin >> tier;
        switch (tier) {
            case 'A': case 'a':
                insuranceCost = 500;
                break;
            case 'B': case 'b':
                insuranceCost = 200;
                break;
            case 'C': case 'c':
                insuranceCost = 100;
                break;
            default:
                cout << "Mức độ không hợp lệ, không tính phí bảo hiểm." << endl;
                insuranceCost = 0;
                break;
        }
    }
    double totalCost = discountedCost + insuranceCost;
    cout << "Phí bảo hiểm: $" << insuranceCost << endl;
    cout << "Tổng tiền phải trả: $" << totalCost << endl;
    cout << "-----------------------------------------" << endl;
}

// Function to read the password from a file
string docMatKhauTuFile(const string& filePath) {
    ifstream file(filePath);
    string password;
    if (file.is_open()) {
        getline(file, password);
        file.close();
    } else {
        cout << "Không thể mở file mật khẩu." << endl;
    }
    return password;
}

// Function to check if the entered password is correct
bool kiemTraMatKhau(const string& correctPassword) {
    string enteredPassword;
    cout << "*****************************************" << endl;
    cout << "*                                       *" << endl;
    cout << "*       Nhập mật khẩu để truy cập       *" << endl;
    cout << "*                                       *" << endl;
    cout << "*****************************************" << endl;
    cout << "* Mật khẩu: ";
    cin >> enteredPassword;
    cout << "*****************************************" << endl;
    return enteredPassword == correctPassword;
}

// Function to initialize the vehicle list
void khoiTaoXe(vector<Vehicle>& danhSachXe) {
    danhSachXe.push_back(Vehicle("4S1234", "Toyota", "Red", true, "Good"));
    danhSachXe.push_back(Vehicle("4S5678", "Honda", "Blue", true, "Good"));
    danhSachXe.push_back(Vehicle("7S2345", "Ford", "Black", true, "Good"));
    danhSachXe.push_back(Vehicle("7S6789", "Chevrolet", "White", true, "Good"));
}

// Function to find a vehicle by license plate
Vehicle* timXe(const vector<Vehicle>& danhSachXe, const string& bienSo) {
    for (auto& xe : danhSachXe) {
        if (xe.licensePlate == bienSo && xe.available) {
            return const_cast<Vehicle*>(&xe);
        }
    }
    return nullptr;
}

// Function to display the list of vehicles
void hienThiDanhSachXe(const vector<Vehicle>& danhSachXe) {
    cout << "-----------------------------------------" << endl;
    cout << "|         DANH SÁCH XE CÓ SẴN          |" << endl;
    cout << "-----------------------------------------" << endl;
    for (const auto& xe : danhSachXe) {
        cout << "Biển số: " << xe.licensePlate << endl;
        cout << "Hãng: " << xe.brand << endl;
        cout << "Màu: " << xe.color << endl;
        cout << "Trạng thái: " << (xe.available ? "Có sẵn" : "Đã thuê") << endl;
        cout << "Tình trạng: " << xe.condition << endl;
        cout << "-----------------------------------------" << endl;
    }
}

// Function to add maintenance schedule for a vehicle
void themBaoDuongXe(vector<Vehicle>& danhSachXe) {
    string bienSo;
    cout << "Nhập biển số xe cần bảo dưỡng: ";
    cin >> bienSo;
    Vehicle* xe = timXe(danhSachXe, bienSo);
    if (xe == nullptr) {
        cout << "Xe không tồn tại hoặc đang được thuê." << endl;
        return;
    }
    string description;
    cout << "Nhập mô tả bảo dưỡng: ";
    cin.ignore();
    getline(cin, description);
    tm dueDate = nhapNgay("Nhập ngày bảo dưỡng");
    // Find the corresponding Car and add maintenance task
    for (auto& xe : danhSachXe) {
        if (xe.licensePlate == bienSo) {
            Car* loaiXe;
            if (xe.licensePlate[0] == '4') {
                loaiXe = new Car4Seater();
            } else {
                loaiXe = new Car7Seater();
            }
            loaiXe->addMaintenanceTask(description, dueDate);
            delete loaiXe; // Cleanup temporary object
            cout << "Bảo dưỡng đã được thêm cho xe " << bienSo << endl;
            break;
        }
    }
}

int main() {
    string filePath = "D:\\pb\\pass.txt";
    string correctPassword = docMatKhauTuFile(filePath);

    if (!kiemTraMatKhau(correctPassword)) {
        cout << "*****************************************" << endl;
        cout << "* Mật khẩu không chính xác. Thoát chương trình. *" << endl;
        cout << "*****************************************" << endl;
        return 1;
    }

    vector<KhachHang*> danhSachKH; // Use vector to manage the list of customers
    vector<Vehicle> danhSachXe; // List of vehicles
    khoiTaoXe(danhSachXe); // Initialize vehicle list

    // Menu options
    cout << "-----------------------------------------" << endl;
    cout << "|      CHƯƠNG TRÌNH QUẢN LÝ THUÊ XE     |" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "|  1. Thêm khách hàng                   |" << endl;
    cout << "|  2. Thêm khách hàng VIP               |" << endl;
    cout << "|  3. Xoá khách hàng                    |" << endl;
    cout << "|  4. Hiển thị danh sách khách hàng     |" << endl;
    cout << "|  5. Hiển thị danh sách xe             |" << endl;
    cout << "|  6. Thêm bảo dưỡng cho xe             |" << endl;
    cout << "|  0. Thoát                             |" << endl;
    cout << "-----------------------------------------" << endl;

    int luaChon;
    do {
        cout << "Nhập lựa chọn của bạn: ";
        cin >> luaChon;
        cin.ignore(); // Clear the newline character from the input buffer
        switch (luaChon) {
            case 1: {
                string ten, diaChi, sdt, hangXe, loaiXeStr, bienSo;
                Car* loaiXe;
                Vehicle* xe;
                cout << "Nhập tên khách hàng: ";
                getline(cin, ten);
                cout << "Nhập địa chỉ: ";
                getline(cin, diaChi);
                cout << "Nhập số điện thoại: ";
                getline(cin, sdt);
                cout << "Nhập hãng xe khách muốn thuê: ";
                getline(cin, hangXe);
                cout << "Nhập loại xe (4-seater/7-seater): ";
                getline(cin, loaiXeStr);
                cout << "Nhập biển số xe: ";
                getline(cin, bienSo);
                xe = timXe(danhSachXe, bienSo);
                if (!xe) {
                    cout << "Xe không có sẵn hoặc biển số không hợp lệ!" << endl;
                    break;
                }
                if (loaiXeStr == "4-seater") {
                    loaiXe = new Car4Seater();
                } else if (loaiXeStr == "7-seater") {
                    loaiXe = new Car7Seater();
                } else {
                    cout << "Loại xe không hợp lệ!" << endl;
                    break;
                }

                tm ngayThue = nhapNgay("Nhập ngày thuê");
                tm ngayTra = nhapNgay("Nhập ngày trả");

                danhSachKH.push_back(new KhachHang(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra, xe));
                break;
            }
            case 2: {
                string ten, diaChi, sdt, hangXe, loaiXeStr, bienSo;
                Car* loaiXe;
                Vehicle* xe;
                double discountRate;
                cout << "Nhập tên khách hàng: ";
                getline(cin, ten);
                cout << "Nhập địa chỉ: ";
                getline(cin, diaChi);
                cout << "Nhập số điện thoại: ";
                getline(cin, sdt);
                cout << "Nhập hãng xe khách muốn thuê: ";
                getline(cin, hangXe);
                cout << "Nhập loại xe (4-seater/7-seater): ";
                getline(cin, loaiXeStr);
                cout << "Nhập biển số xe: ";
                getline(cin, bienSo);
                xe = timXe(danhSachXe, bienSo);
                if (!xe) {
                    cout << "Xe không có sẵn hoặc biển số không hợp lệ!" << endl;
                    break;
                }
                if (loaiXeStr == "4-seater") {
                    loaiXe = new Car4Seater();
                } else if (loaiXeStr == "7-seater") {
                    loaiXe = new Car7Seater();
                } else {
                    cout << "Loại xe không hợp lệ!" << endl;
                    break;
                }

                tm ngayThue = nhapNgay("Nhập ngày thuê");
                tm ngayTra = nhapNgay("Nhập ngày trả");
                cout << "Nhập tỷ lệ giảm giá cho khách VIP (vd: 0.1 cho 10%): ";
                cin >> discountRate;
                cin.ignore();

                danhSachKH.push_back(new KhachHangVIP(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra, xe, discountRate));
                break;
            }
            case 3: {
                int viTri;
                cout << "Nhập vị trí khách hàng muốn xóa: ";
                cin >> viTri;
                if (viTri >= 1 && viTri <= danhSachKH.size()) {
                    inHoaDon(danhSachKH[viTri - 1]);
                    delete danhSachKH[viTri - 1];
                    danhSachKH.erase(danhSachKH.begin() + (viTri - 1));
                    cout << "Xóa thành công!" << endl;
                } else {
                    cout << "Vị trí không hợp lệ!" << endl;
                }
                break;
            }
            case 4: {
                if (danhSachKH.empty()) {
                    cout << "Danh sách khách hàng trống!" << endl;
                } else {
                    cout << "Danh sách khách hàng:" << endl;
                    for (int i = 0; i < danhSachKH.size(); ++i) {
                        cout << "Khách hàng thứ " << i + 1 << ":" << endl;
                        danhSachKH[i]->layThongTinKH();
                    }
                }
                break;
            }
            case 5: {
                hienThiDanhSachXe(danhSachXe);
                break;
            }
            case 6: {
                themBaoDuongXe(danhSachXe);
                break;
            }
            case 0:
                cout << "Thoát chương trình, chúc bạn một ngày tốt lành" << endl;
                break;
            default:
                cout << "Lựa chọn không hợp lệ!" << endl;
                break;
        }
    } while (luaChon != 0);

    // Clean up dynamically allocated memory
    for (KhachHang* kh : danhSachKH) {
        delete kh;
    }

    return 0;
}
