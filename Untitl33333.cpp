#include <iostream>
#include <string>
#include <vector>
#include <ctime> // For handling dates
using namespace std;

// Define structure for maintenance tasks
struct MaintenanceTask {
    string description;
    tm dueDate;
    bool completed;

    MaintenanceTask(string desc, tm due) : description(desc), dueDate(due), completed(false) {}
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
public:
    // Constructor to initialize customer details
    KhachHang(string ten, string diaChi, string sdt, string hangXe, Car* loaiXe, tm ngayThue, tm ngayTra) {
        this->ten = ten;
        this->diaChi = diaChi;
        this->SDT = sdt;
        this->hangXe = hangXe;
        this->loaiXe = loaiXe;
        this->ngayThue = ngayThue;
        this->ngayTra = ngayTra;
    }

    // Method to display customer information
    virtual void layThongTinKH() const {
        cout << "Tên: " << ten << endl;
        cout << "Địa chỉ: " << diaChi << endl;
        cout << "Số điện thoại: " << SDT << endl;
        cout << "Hãng xe muốn thuê: " << hangXe << endl;
        cout << "Loại xe: " << loaiXe->getLoaiXe() << endl;
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

    virtual ~KhachHang() { delete loaiXe; } // Virtual destructor for proper cleanup
};

// Derived class to represent a VIP customer
class KhachHangVIP : public KhachHang {
private:
    double discountRate; // Discount rate for VIP customers
public:
    KhachHangVIP(string ten, string diaChi, string sdt, string hangXe, Car* loaiXe, tm ngayThue, tm ngayTra, double discountRate)
        : KhachHang(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra), discountRate(discountRate) {}

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
};

// Function to print a bill with borders
void inHoaDon(const KhachHang* kh) {
    cout << "-----------------------------------------" << endl;
    cout << "|               HÓA ĐƠN                 |" << endl;
    cout<< "-----------------------------------------" << endl;
    kh->layThongTinKH();
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
                insuranceCost = 200;
                break;
            case 'B': case 'b':
                insuranceCost = 100;
                break;
            case 'C': case 'c':
                insuranceCost = 0;
                break;
            default:
                cout << "Mức độ không hợp lệ, không tính phí bảo hiểm." << endl;
                insuranceCost = 0;
                break;
        }
    }
    double totalCost = kh->tinhTienThue() + insuranceCost;
    cout << "Phí bảo hiểm: $" << insuranceCost << endl;
    cout << "Tổng tiền phải trả: $" << totalCost << endl;
    cout << "-----------------------------------------" << endl;
}

int main() {
    vector<KhachHang*> danhSachKH; // Use vector to manage the list of customers

    // Menu options
    cout << "-----------------------------------------" << endl;
    cout << "|             MENU LỰA CHỌN             |" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "|  1. Thêm khách hàng                   |" << endl;
    cout << "|  2. Thêm khách hàng VIP               |" << endl;
    cout << "|  3. Xoá khách hàng                    |" << endl;
    cout << "|  4. Hiển thị danh sách khách hàng     |" << endl;
    cout << "|  0. Thoát                             |" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Nhập lựa chọn của bạn: ";
    int luaChon;
    do {
        cout << "Nhập lựa chọn của bạn: ";
        cin >> luaChon;
        cin.ignore(); // Clear the newline character from the input buffer
        switch (luaChon) {
            case 1: {
                string ten, diaChi, sdt, hangXe, loaiXeStr;
                Car* loaiXe;
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

                danhSachKH.push_back(new KhachHang(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra));
                break;
            }
            case 2: {
                string ten, diaChi, sdt, hangXe, loaiXeStr;
                Car* loaiXe;
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

                danhSachKH.push_back(new KhachHangVIP(ten, diaChi, sdt, hangXe, loaiXe, ngayThue, ngayTra, discountRate));
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
            case 0:
                cout << "Thoát chương trình, chúc bạn một ngày tốt lành." << endl;
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

