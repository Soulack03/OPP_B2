#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
using namespace std;
// -------------------- Notification system --------------------
enum class NotificationType { ORDER_CONFIRMED, ORDER_PREPARING, ORDER_READY,PROMOTION,NEW_COMBO, RESERVATION_REQUESTED, RESERVATION_CONFIRMED, RESERVATION_CANCELLED, RESERVATION_COMPLETED};
class Notification {
    private:
    string notification_id;
    NotificationType type;
    string title;
    string message;
    string timestamp;
    inline static int notification_cnt = 0;
    public:
    Notification(NotificationType _type, string _title, string _message): type(_type), title(_title), message(_message) {
            notification_cnt++;
            stringstream ss;
            ss << "N" << setw(3) << setfill('0') << notification_cnt;
            notification_id = ss.str();
        }
        void display() const{
            cout<< " " << message <<endl;
            cout << "------------------------" << endl;
        }
        string getId() const { return notification_id;}
        NotificationType getType() const { return type; }
};
class NotificationManager {
    private:
    vector<Notification> notifications;
    public:
    void sendNotification(NotificationType type, string title, string message) {
        
        Notification notification(type, title, message);
        notifications.push_back(notification);
        notification.display();
    }

    void sendOrderUpdate(string order_id, string status) {
        string title, message;
        NotificationType type;
        
        if (status == "Confirmed") {
            type = NotificationType::ORDER_CONFIRMED;
            title = "Order Confirmed.";
            message = "Order " + order_id + " has been confirmed and is being prepared.";
        } else if (status == "Preparing") {
            type = NotificationType::ORDER_PREPARING;
            title = "Order in Kitchen";
            message = "Order " + order_id + " is now being prepared.";
        } else if (status == "Ready") {
            type = NotificationType::ORDER_READY;
            title = "Order Ready.";
            message = "Order " + order_id + " is ready for pickup.";
        } else {
            return;
        }
        
        sendNotification(type, title, message);
    }

    void sendPromotion(string promo_message) {
        sendNotification(NotificationType::PROMOTION, "Special Promotion!", promo_message);
    }

    void sendNewCombo(string combo_name, double discount) {
        string message = "New combo '" + combo_name + "' with " + 
                        to_string(int(discount * 100)) + "% discount is now available!";
        sendNotification(NotificationType::NEW_COMBO, "New Combo Added!", message);
    }

    void showAllNotifications() {
        if (notifications.empty()) {
            cout << "No notifications available." << endl;
            return;
        }
        
        cout << "\n === All notifications === " << endl;
        for (auto& notification : notifications) {
            notification.display();
        }
    }
};

// ================= Food ================= I do 
class Food {
protected:
    string id;
    string name;
    double price;
public:
    inline static int cnt = 0;
    Food(string _name, double _price) : name(_name), price(_price) {
        cnt++;
        stringstream ss;
        ss << "F" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    virtual void display() {
        cout << "ID: " << id << ", Name: " << name << ", Price: $" << price << endl;
    }

    string getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }

    virtual ~Food() {}
};

class rice_don : public Food {
private:
    string rice_type;
    string protein;
public:
    rice_don(string _name, double _price, string _rice_type = "White Rice", string _protein = "Chicken")
        : Food(_name, _price), rice_type(_rice_type), protein(_protein) {}

    void display() override {
        Food::display();
        cout << "Rice type: " << rice_type << endl;
        cout << "Protein: " << protein << endl;
    }
};

class ramen : public Food {
private:
    string broth_type;
    string noodle_type;
public:
    ramen(string _name, double _price, string _broth = "Tonkotsu", string _noodle = "Thin")
        : Food(_name, _price), broth_type(_broth), noodle_type(_noodle) {}

    void display() override {
        Food::display();
        cout << "Broth type: " << broth_type << endl;
        cout << "Noodle type: " << noodle_type << endl;
    }
};

class topping : public Food {
private:
    string category;
public:
    topping(string _name, double _price, string _category = "Vegetable")
        : Food(_name, _price), category(_category) {}

    void display() override {
        Food::display();
        cout << "Category: " << category << endl;
    }
};

class SideDish : public Food {
private:
    string dish_type;
    bool is_vegetarian;
public:
    SideDish(string _name, double _price, string _type = "Appetizer", bool _veg = false)
        : Food(_name, _price), dish_type(_type), is_vegetarian(_veg) {}

    void display() override {
        Food::display();
        cout << "Dish type: " << dish_type << endl;
        cout << "Vegetarian: " << (is_vegetarian ? "Yes" : "No") << endl;
    }
};

class Drink : public Food {
private:
    string oz;
public:
    Drink(string _name, double _price, string _oz) : Food(_name, _price), oz(_oz) {}
    void display() override {
        Food::display();
        cout << "Ounce: " << oz << endl;
    }
};

//-------------------- Manage Food --------------------
class FoodManager{
private:
    map<string, Food*> manageFood;
public:
    void addToManageFood(Food* food) {
        if (food != nullptr) {
            manageFood[food->getId()] = food;
        }
    }

    void removeFood(string id) {
        auto it = manageFood.find(id);
        if (it != manageFood.end()) {
            delete it->second; // free memory
            manageFood.erase(it);
            cout << "Food with ID " << id << " removed successfully.\n";
        } else {
            cout << "Food with ID " << id << " not found.\n";
        }
    }
    
    void displayAllFood() {
        cout << "=== All Available Food Items ===" << endl;
        for (auto& pair : manageFood) {
            pair.second->display();
        }
        cout << "===============================" << endl;
    }

    Food* findFoodById(string id) {
        auto it = manageFood.find(id);
        return (it != manageFood.end()) ? it->second : nullptr;
    }
};
FoodManager* foodwehave;

// -------------------- Combo --------------------
class Combo {
private:
    vector<Food*> FoodHavecombo;
    string combo_id;
    string combo_name;
    double price;
    double discount;
    inline static int combo_cnt = 0;
    NotificationManager& notifMgr;
public:
    Combo(string _combo_name, double _discount,NotificationManager& nm)
        : combo_name(_combo_name), discount(_discount), notifMgr(nm) {
        combo_cnt++;
        stringstream ss;
        ss << "C" << setw(3) << setfill('0') << combo_cnt;
        combo_id = ss.str();
        price = 0.0;
        notifMgr.sendNewCombo(_combo_name, _discount); 
    }

    void addFood(Food* food) {
        if (food != nullptr) {
            FoodHavecombo.push_back(food);
            calculatePrice();
        }
    }

    void removeFood(string food_id) {
        for (auto it = FoodHavecombo.begin(); it != FoodHavecombo.end(); ++it) {
            if ((*it)->getId() == food_id) {
                FoodHavecombo.erase(it);
                calculatePrice();
                break;
            }
        }
    }

    void calculatePrice() {
        double total = 0.0;
        for (Food* food : FoodHavecombo) {
            total += food->getPrice();
        }
        price = total * (1.0 - discount);
    }

    void display() {
        cout << "=== Combo Details ===" << endl;
        cout << "Combo ID: " << combo_id << endl;
        cout << "Combo Name: " << combo_name << endl;
        cout << "Discount: " << (discount * 100) << "%" << endl;
        cout << "Items in combo:" << endl;

        double original_total = 0.0;
        for (Food* food : FoodHavecombo) {
            cout << "  - ";
            food->display();
            original_total += food->getPrice();
        }

        cout << "Original Total: $" << fixed << setprecision(2) << original_total << endl;
        cout << "Discounted Price: $" << fixed << setprecision(2) << price << endl;
        cout << "You Save: $" << fixed << setprecision(2) << (original_total - price) << endl;
        cout << "========================" << endl;
    }

    string getComboId() { return combo_id; }
    string getComboName() { return combo_name; }
    double getPrice() { return price; }
    double getDiscount() { return discount; }
    vector<Food*> getFoodItems() { return FoodHavecombo; }
};

class ComboManager {
private:
    map<string, Combo*> manageCombo;
public:
    void addCombo(Combo* combo) {
        if (combo != nullptr) {
            manageCombo[combo->getComboId()] = combo;
        }
    }

    void removeCombo(string combo_id) {
        auto it = manageCombo.find(combo_id);
        if (it != manageCombo.end()) {
            delete it->second;   
            manageCombo.erase(it);
            cout << " Combo with ID " << combo_id << " removed.\n";
        } else {
            cout << " Combo ID not found.\n";
        }
    }

    void displayAllCombos() {
        if (manageCombo.empty()) {
            cout << "  No combos available.\n";
            return;
        }

        cout << "\n===  All Available Combos ===\n";
        for (auto& pair : manageCombo) {
            pair.second->display();
            cout << "-------------------------------\n";
        }
        cout << "===============================\n";
    }

    Combo* findComboById( string id) {
        auto it = manageCombo.find(id);
        return (it != manageCombo.end()) ? it->second : nullptr;
    }

    vector<Combo*> getAllCombos()  {
        vector<Combo*> combos;
        for (auto& pair : manageCombo) {
            combos.push_back(pair.second);
        }
        return combos;
    }
};
ComboManager* combowehave;

// ==================== Payment Section ====================
class PaymentMethod {
protected:
    string method_name;
    double amount;
public:
    PaymentMethod(string _method_name, double _amount): method_name(_method_name), amount(_amount) {}

    virtual void display() {
        cout << "===== Payment Info =====" << endl;
        cout << "Method: " << method_name << endl;
        cout << "Amount: $" << fixed << setprecision(2) << amount << endl;
    }

    double getAmount() { return amount; }
    string getMethodName() { return method_name; }
    virtual ~PaymentMethod() {}
};

class CashPayment : public PaymentMethod {
private:
    string currency; //VND, Euro,...
public:
    CashPayment(double _amount, string _cur) 
        : PaymentMethod("Cash", _amount), currency(_cur) {}
    
    string getCurrency() { return currency; }
    void setCurrency(string _cur) { currency = _cur; }

    void display() override {
        PaymentMethod::display();
        cout << "Currency: " << currency << endl;
        cout << "====================" << endl;
    }
};

class CreditPayment : public PaymentMethod {
private:
    string card_number;
public:
    CreditPayment(double _amount, string _card_number): PaymentMethod("Credit", _amount), card_number(_card_number) {}

    string getCardNumber() { return card_number; }
    void setCardNumber(string _card) { card_number = _card; }

    void display() override {
        PaymentMethod::display();
        if (card_number.length() >= 4)
            cout << "Card Number: ****" << card_number.substr(card_number.length() - 4) << endl;
        else
            cout << "Card Number: (invalid)" << endl;
        cout << "====================" << endl;
    }
};

class eWalletPayment : public PaymentMethod {
private:
    string wallet_name;
public:
    eWalletPayment(double _amount, string _wallet_name): PaymentMethod("e-Wallet", _amount), wallet_name(_wallet_name) {}

    string getWalletName() { return wallet_name; }
    void setWalletName(string _wallet) { wallet_name = _wallet; }

    void display() override {
        PaymentMethod::display();
        cout << "Wallet Name: " << wallet_name << endl;
        cout << "====================" << endl;
    }
};

class PaymentManager {
private:
    vector<PaymentMethod*> payments;
    int size=0;
public:
    void addPayment(PaymentMethod* payment) {
        if (payment != nullptr) {
            payments.push_back(payment);
            size++;
        }
    }

    vector<PaymentMethod*> getVec(){return payments;}
    int getSize() { return size; }
    PaymentMethod* getPaymentMethod(int n){
        if(n<0 && n>size)return nullptr;
        return payments[n];
    }

    void displayAllPayments() {
        cout << "=== All Payments ===" << endl;
        for (PaymentMethod* payment : payments) {
            payment->display();
        }
    }
};
PaymentManager paymentManager;
NotificationManager* notificationManager;

enum class OrderStatus { Pending, Preparing, Completed, Cancelled };
// -------------------- Order --------------------
class Order {
private:
    string order_id;
    vector<Food*> food_items;
    vector<Combo> combos;
    double total_price;
    OrderStatus status;
    PaymentMethod* payment;
    NotificationManager& notifMgr;
    inline static int order_cnt = 0;
public:
    Order(NotificationManager& nm) : payment(nullptr), notifMgr(nm) {
        order_cnt++;
        stringstream ss;
        ss << "O" << setw(3) << setfill('0') << order_cnt;
        order_id = ss.str();
        total_price = 0.0;
        status = OrderStatus::Pending; // mặc định
        notifMgr.sendOrderUpdate(order_id, "Confirmed"); //confirmation notification
    }

    void setStatus(OrderStatus s) { //updated status based on status change
         status = s;
         switch(s) { 
            case OrderStatus::Preparing:
                notifMgr.sendOrderUpdate(order_id, "Preparing");
                break;
            case OrderStatus::Completed:
                notifMgr.sendOrderUpdate(order_id, "Ready");
                break;
            case OrderStatus::Cancelled:
                if(payment != nullptr) {
                    cout << "Refunding $" << fixed << setprecision(2) << payment->getAmount() << " to customer." << endl;
                }
            default:
                break;
        }
    }
    void setPaymentMethod(PaymentMethod* pm){payment = pm;}
    double getTotalPrice() { return total_price; }
    OrderStatus getStatus() { return status; }
    string getOrderId() { return order_id; }
    PaymentMethod* getPaymentMethod() { return payment; }
    vector<Combo> getCombo(){return combos;}
    vector<Food*>getFood(){return food_items;}

    void addFood(Food* food) {
        if (food != nullptr) {
            food_items.push_back(food);
            calculateTotal();
        }
    }

    void addCombo(Combo combo) {
        combos.push_back(combo);
        calculateTotal();
    }

    void display() {
        cout << "=== Order Details ===" << endl;
        cout << "Order ID: " << order_id << endl;
        cout << "Status: ";
        switch (status) {
            case OrderStatus::Pending: cout << "Pending"; break;
            case OrderStatus::Preparing: cout << "Preparing"; break;
            case OrderStatus::Completed: cout << "Completed"; break;
            case OrderStatus::Cancelled: cout << "Cancelled"; break;
        }
        cout << endl;

        cout << "Items in order:" << endl;
        for (Food* food : food_items) {
            cout << "  - ";
            food->display();
        }
        for (Combo& combo : combos) {
            cout << "  - Combo: " << combo.getComboName() << endl;
            combo.display();
        }
        cout << "Total Price: $" << fixed << setprecision(2) << total_price << endl;

        if(payment){
            cout << "Payment Details: " << endl;
            payment->display();
        } else {
            cout << "Payment Method: Not set" << endl;
        }
        cout << "=====================" << endl;
    }

    void calculateTotal() {
        double total = 0.0;
        for (Food* food : food_items) {
            total += food->getPrice();
        }
        for (Combo& combo : combos) {
            total += combo.getPrice();
        }
        total_price = total;
    }
};

// -------------------- User --------------------
class User {
protected:
    string username;
    string password;
    string id;
public:
    User(string _username, string _password){
        username = _username;
        password = _password;
    }

    string getUserId(){ return id; }
    string getUsername(){ return username; }
    string getPassword(){ return password; }

    void setId(string _id){ id = _id; }
    void setUsername(string _username){ username = _username; }
    void setPassword(string _password){ password = _password; }

    virtual bool login(string _username, string _password) {
        return (username == _username && password == _password);
    }
    
    virtual string getRole() = 0;
    virtual ~User() {}
};


// -------------------- Guest + Staff --------------------
class Guest : public User {
private:
    string role;
    inline static int cnt = 0;
    NotificationManager& notifMgr;
    vector<Order> orderHistory;

public:
    Guest(string _username, string _password, NotificationManager& nm): User(_username, _password), notifMgr(nm) {
        
        role = "Guest";
        cnt++;
        stringstream ss;
        ss << "G" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    string getRole() override { return role; }
    vector<Order>getOrderHistory(){return orderHistory;}
    void displayMenu() {
        cout << "\n=== Food Menu ===\n";
        if (foodwehave) foodwehave->displayAllFood();
        else cout << "No FoodManager linked.\n";

        cout << "\n=== Combo Menu ===\n";
        if (combowehave) combowehave->displayAllCombos();
        else cout << "No ComboManager linked.\n";
    }

    void createNewOrder(){
        Order newOrder=Order(*notificationManager);
        orderHistory.push_back(newOrder);
    }

    void addFoodLatestOrder(string fId){
    if(orderHistory.empty()) {
        cout << "No orders available." << endl;
        return;
    }
    
    Order latestO = orderHistory.back();
    Food* e = foodwehave->findFoodById(fId);
    
    if(e) {
        latestO.addFood(e);
        cout << "Added " << e->getName() << " to order " 
             << latestO.getOrderId() << endl;
    } else {
        cout << "Food ID not found." << endl;
        }
    }

    void paymentMethodForLatestOrder(int i){
    if(orderHistory.empty()) {
        cout << "No orders available." << endl;
        return;
    }
    
    Order latestO = orderHistory.back();
    PaymentMethod* p = paymentManager.getPaymentMethod(i);
    
    if(p == nullptr){
        cout << "Invalid payment method." << endl;
    } else {
        latestO.setPaymentMethod(p);
        cout << "Order: " << latestO.getOrderId() << endl;
        cout << "Pay by: " << endl;
        p->display();
        }
    }

    void displayAllOrder(){
    if(orderHistory.size() == 0){
        cout << "No orders available." << endl;
        return;
    }
    
    cout << "\n=== All Orders ===" << endl;
    for(auto& x : orderHistory){
        x.display();
        cout << "-------------------" << endl;
        }
    }

    
/*
    void createOrder() {
        if (!foodhave && !combohave) {
            cout << "Manager not set.\n";
            return;
        }

        cout << "\n--- Create New Order ---\n";
        Order newOrder(notifMgr);

        while (true) {
            int mode;
            cout << "\n1. Add Food\n2. Add Combo\n0. Finish\nChoice: ";
            cin >> mode;

            if (mode == 0) break;

            if (mode == 1) {
                string id;
                cout << "Enter Food ID: ";
                cin >> id;
                Food* foodItem = foodhave->findFoodById(id);
                if (foodItem) {
                    newOrder.addFood(foodItem);
                    cout << "Added: " << foodItem->getName() << "\n";
                } else cout << "Invalid Food ID.\n";
            } 
            else if (mode == 2) {
                string id;
                cout << "Enter Combo ID: ";
                cin >> id;
                Combo* comboItem = combohave->findComboById(id);
                if (comboItem) {
                    newOrder.addCombo(*comboItem);
                    cout << "Added: " << comboItem->getComboName() << "\n";
                } else cout << "Invalid Combo ID.\n";
            } 
            else cout << "Invalid choice.\n";
        }

        if (newOrder.getFood().empty() && newOrder.getCombo().empty()) {
            cout << "No items added. Order canceled.\n";
            return;
        }

        cout << "\n--- Payment Methods ---\n";
        paymentManager.displayAllPayments();

        int choice;
        do {
            cout << "Select payment method (1 - " << paymentManager.getSize() << "): ";
            cin >> choice;
            if (choice < 1 || choice > paymentManager.getSize() || cin.fail()) {
                cout << "Invalid choice. Try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice < 1 || choice > paymentManager.getSize() || cin.fail());

        newOrder.setPaymentMethod(paymentManager.getVec()[choice - 1]);

        cout << "\n--- Order Summary ---\n";
        newOrder.display();
        cout << "Total Price: " << fixed << setprecision(2)
             << newOrder.getTotalPrice() << "\n";

        orderHistory.push_back(newOrder);
        cout << "Order successfully created.\n";
    }
*/
};

class Staff : public User {
private:
    string role;
    NotificationManager& notifMgr;
    inline static int cnt = 0;

public:
    Staff(string _username, string _password, NotificationManager& nm)
        : User(_username, _password), notifMgr(nm) {
        role = "Staff";
        cnt++;
        stringstream ss;
        ss << "S" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    string getRole() override { return role; }
    // === Display All Food ===
    void displayAllFood() {
        if (!foodwehave) {
            cout << "FoodManager not set.\n";
            return;
        }
        cout << "\n--- All Food Items ---\n";
        foodwehave->displayAllFood();
    }

    // === Display All Combos ===
    void displayAllCombos() {
        if (!combowehave) {
            cout << "ComboManager not set.\n";
            return;
        }
        cout << "\n--- All Combo Items ---\n";
        combowehave->displayAllCombos();
    }

    // === Add New Food Menu ===
    void addFoodToMenu(Food* food) {
        if (!foodwehave) {
            cout << "FoodManager not set.\n";
            return;
        }
        foodwehave->addToManageFood(food);
        cout << "Food '" << food->getName() << "' added successfully.\n";
    }

    // === Create a new combo ===
    void createCombo(string comboName, double discount) {
        if (!combowehave) {
            cout << "ComboManager not set.\n";
        }
        
        Combo* newCombo = new Combo(comboName, discount, notifMgr);
        combowehave->addCombo(newCombo);
    }

    // === Add food to an existing combo ===
    void addFoodToCombo(string comboid, string foodId) {
        Combo* combo= combowehave->findComboById(comboid);
        Food*  food = foodwehave->findFoodById(foodId);
        if(food==nullptr||combo==nullptr){
            cout<<"Invalid Id "<<endl;
            return ;
        }
        combo->addFood(food);
        cout<<food->getName()<<" has been added to combo."<<endl;
    }

    // === Add combo to menu ===
    void addComboToMenu(Combo* combo) {
        if (!combowehave) {
            cout << "ComboManager not set.\n";
            return;
        }
        
        if (!combo) {
            cout << "Invalid combo pointer.\n";
            return;
        }
        
        combowehave->addCombo(combo);
        cout << "Combo '" << combo->getComboName() << "' added to menu successfully.\n";
    }

    // === Remove Food ===
    void removeFood(string foodId) {
        if (!foodwehave) {
            cout << "FoodManager not set.\n";
            return;
        }
        foodwehave->removeFood(foodId);
    }
    

    // === Remove Combo ===
    void removeCombo(string comboId) {
        if (!combowehave) {
            cout << "ComboManager not set.\n";
            return;
        }
        combowehave->removeCombo(comboId);
    }

    // === Display Staff Info ===
    void display() {
        cout << "\n--- Staff Information ---\n";
        cout << "ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Role: " << role << "\n";
    }

    // === Staff Main Menu ===
    // void staffMenu() {
    //     int choice;
    //     do {
    //         cout << "\n=============================\n";
    //         cout << "Staff Menu\n";
    //         cout << "1. View All Foods\n";
    //         cout << "2. Add New Food\n";
    //         cout << "3. Remove Food\n";
    //         cout << "4. View All Combos\n";
    //         cout << "5. Create New Combo\n";
    //         cout << "6. Remove Combo\n";
    //         cout << "0. Logout\n";
    //         cout << "=============================\n";
    //         cout << "Enter choice: ";
    //         cin >> choice;

    //         switch (choice) {
    //             case 1: displayAllFood(); break;
    //             case 2: addFoodToMenu(); break;
    //             case 3: removeFood(); break;
    //             case 4: displayAllCombos(); break;
    //             case 5: addComboToMenu(); break;
    //             case 6: removeCombo(); break;
    //             case 0: cout << "Logging out...\n"; break;
    //             default: cout << "Invalid choice. Try again.\n"; break;
    //         }
    //     } while (choice != 0);
    // }
};

class AccountManager {
private:
    map<string, User*> guests;   // quản lý Guest
    Staff* staffAccount;         // 1 staff duy nhất
    NotificationManager& notifMgr; // Thêm reference đến NotificationManager
    
public:
    AccountManager(NotificationManager& nm) : notifMgr(nm) {
        staffAccount = new Staff("admin", "123", notifMgr);
    }

    bool registerGuest(string username, string password) {
        if(guests.find(username) != guests.end()) {
            cout << "Username already exists!\n";
            return false; // đã tồn tại
        }
        guests[username] = new Guest(username, password, notifMgr);
        cout << "Guest '" << username << "' registered successfully!\n";
        return true;
    }

    User* login(string username, string password) {
        if(username == staffAccount->getUsername()) { 
            if (staffAccount->login(username, password)) {
                cout << "Staff login successful!\n";
                return staffAccount;
            }
        }
        
        auto it = guests.find(username);
        if(it != guests.end()) {
            if (it->second->login(username, password)) {
                cout << "Guest login successful!\n";
                return it->second; 
            }
        }
        
        cout << "Invalid username or password!\n";
        return nullptr;
    }

    void displayAllAccounts() {
        cout << "\n=== All Accounts ===\n";
        cout << "Staff Account: " 
             << staffAccount->getUsername() 
             << " [ID: " << staffAccount->getUserId() << "]\n";

        cout << "\nGuest Accounts (" << guests.size() << "):\n";
        int i = 1;
        for(auto &p : guests) {
            cout << "  " << i++ << ". " 
                 << p.second->getUsername() 
                 << " [ID: " << p.second->getUserId() << "]\n";
        }
        cout << "====================\n";
    }

    Staff* getStaffAccount() { return staffAccount; }

    ~AccountManager() {
        for(auto &p : guests) delete p.second;
        delete staffAccount;
    }
};


int main() {
    // ===== Initialize Global Managers =====
    notificationManager = new NotificationManager();
    foodwehave = new FoodManager();
    combowehave = new ComboManager();
    AccountManager accMgr(*notificationManager);

    cout << "=============================================" << endl;
    cout << "#         FOOD ORDERING SYSTEM TEST CASE         #" << endl;
    cout << "=============================================" << endl;

    // --- STEP 1: Staff Login & Menu Population ---
    cout << "\n### STEP 1: Staff Login & Menu Population ###" << endl;
    User* staffUser = accMgr.login("admin", "123");
    Staff* staff = dynamic_cast<Staff*>(staffUser);

    if (staff) {
        cout << "\n===== Staff Adds Food =====" << endl;
        staff->addFoodToMenu(new rice_don("Chicken Rice Don", 5.50, "White Rice", "Chicken"));
        staff->addFoodToMenu(new ramen("Tonkotsu Ramen", 7.20, "Tonkotsu", "Thin"));
        staff->addFoodToMenu(new topping("Boiled Egg", 1.00, "Protein"));
        staff->addFoodToMenu(new Drink("Green Tea", 2.50, "12 oz"));
        staff->displayAllFood();

        // --- STEP 2: Staff Creates and Manages a Combo ---
        cout << "\n### STEP 2: Staff Creates and Manages a Combo ###" << endl;
        cout << "\n===== Staff Creates Combo =====" << endl;
        staff->createCombo("Lunch Special", 0.15); // This will have ID "C001"
        staff->addFoodToCombo("C001", "F002"); // Add Tonkotsu Ramen
        staff->addFoodToCombo("C001", "F004"); // Add Green Tea
        staff->displayAllCombos();
    }
    
    // --- STEP 3: Register and Log In a Guest User ---
    cout << "\n### STEP 3: Register and Log In a Guest User ###" << endl;
    cout << "\n===== Register Guest Accounts =====" << endl;
    accMgr.registerGuest("alice", "1234");
    
    cout << "\n===== Guest Login =====" << endl;
    User* guestUser = accMgr.login("alice", "1234");
    Guest* guest = dynamic_cast<Guest*>(guestUser);

    if (guest) {
        // --- STEP 4: Guest Places an Order ---
        cout << "\n### STEP 4: Guest Places an Order ###" << endl;
        cout << "\n===== Guest Menu =====" << endl;
        guest->displayMenu();

        cout << "\n===== Guest Creates Order =====" << endl;
        guest->createNewOrder(); // Order O001 is created, confirmation sent
        guest->addFoodLatestOrder("F001"); // Add Chicken Rice Don
        guest->addFoodLatestOrder("F003"); // Add Boiled Egg
        
        // --- STEP 5: Guest Finalizes Order with Payment ---
        cout << "\n### STEP 5: Guest Finalizes Order with Payment ###" << endl;
        cout << "\n===== Add Payment Methods =====" << endl;
        paymentManager.addPayment(new CashPayment(20.0, "VND"));
        paymentManager.addPayment(new CreditPayment(15.0, "1234567812343456"));
        paymentManager.addPayment(new eWalletPayment(25.0, "Momo"));
        paymentManager.displayAllPayments();
        
        cout << "\n===== Guest Finalizes Order =====" << endl;
        guest->paymentMethodForLatestOrder(2); // Use third payment method (e-Wallet)
        guest->displayAllOrder();

    }

    if (staff) {
        cout << "\n### STEP 7: Staff Modifies the Menu ###" << endl;
        cout << "\n===== Staff Modifies Menu =====" << endl;
        staff->removeFood("F003"); // Remove Boiled Egg
        staff->removeCombo("C001"); // Remove Lunch Special

        cout << "\n===== Displaying Updated Menu =====" << endl;
        staff->displayAllFood();
        staff->displayAllCombos();
    }
    
    // --- STEP 8: Cleanup Memory ---
    cout << "\n### STEP 8: Cleanup Memory ###" << endl;
    cout << "\nTEST CASE COMPLETED SUCCESSFULLY" << endl;
    cout << "=============================================" << endl;

    delete foodwehave;
    delete combowehave;
    delete notificationManager;

    return 0;
    
}
