#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};
// ========== HELPERS ========== //
string roomTypeName(RoomType type)
{
    switch (type)
    {
        case GENERAL_WARD:  return "General Ward";
        case ICU:           return "ICU";
        case PRIVATE_ROOM:  return "Private Room";
        case SEMI_PRIVATE:  return "Semi-Private Room";
        default:            return "Unknown";
    }
}

string departmentName(Department dept)
{
    switch (dept)
    {
        case CARDIOLOGY:    return "Cardiology";
        case NEUROLOGY:     return "Neurology";
        case ORTHOPEDICS:   return "Orthopedics";
        case PEDIATRICS:    return "Pediatrics";
        case EMERGENCY:     return "Emergency";
        case GENERAL:       return "General Practice";
        default:            return "Unknown";
    }
}

// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c){
        id=pid;
        name=n;
        age=a;
        contact=c;
        isAdmitted=false;

    };

    void admitPatient(RoomType type){
        if(isAdmitted==true){
            cout << "Patient "<< name << " is already admitted." <<endl;
        }else{
        isAdmitted=true;
        roomType=type;
        cout << "Patient " << name << " admitted to " << roomTypeName(type) << "." << endl;
        }

    };
    void dischargePatient(){
        if(isAdmitted==false){
            cout<<"Patient "<<name<<" is not currently admitted."<<endl;
        }else{
            isAdmitted=false;
        cout<<"Patient "<<name<<" has been discharged."<<endl;
        }

    };


    int getId(){
        return id;

    };
    string getName(){
        return name;
    };

    int getage(){
    return age;
    };

    string getcontact(){
    return contact;
    };

    RoomType getroomtype(){
    return roomType;
    };

    bool getAdmissionStatus(){
         return isAdmitted;
    };

    void addMedicalRecord(string record){
        medicalHistory.push(record);
        cout << "Medical record added for " << name << ": " << record << endl;
    }

    void requestTest(string testName){
        testQueue.push(testName);
        cout << "Test requested for " << name << ": " << testName << endl;
    }
    string performTest(){
        if(!testQueue.empty()){
            string test = testQueue.front();
            testQueue.pop();
            cout << "Performing test for " << name << ": " << test << endl;
            return test;
        }
        cout << "No pending tests for " << name << "." << endl;
        return "";
    }
    void displayHistory(){
        cout << "Medical History for " << name << ":" << endl;
        if (medicalHistory.empty()){
            cout << "  No medical history." << endl;
        }
        else{
            stack<string> temp = medicalHistory;
            while (!temp.empty()) {
                cout << " - " << temp.top() << endl;
                temp.pop();
            }
        }
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d)
    {
        id = did;
        name = n;
        department = d;
    }

    void addAppointment(int patientId)
    {
        appointmentQueue.push(patientId);
        cout << "Appointment added for patient " << patientId << " with " << name << endl;
    }
    int seePatient(){
       if(appointmentQueue.empty()){
        cout << "No appointments for Dr. " << name << "." << endl;
        return -1;
       }else{
       cout << "Dr. " << name << " is seeing patient " << appointmentQueue.front() << endl;
       int i = appointmentQueue.front();
       appointmentQueue.pop();
       return i;
       }


    };

    int getId()
    {
        return id;
    }
    string getName()
    {
        return name;
    }
    string getDepartment()
    {
        return departmentName(department);
    }
    int getPendingCount()
    {
        return (int)appointmentQueue.size();
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital()
    {
        patientCounter = 1;
        doctorCounter = 1;
    }

    int registerPatient(string name, int age, string contact)
    {
        Patient p(patientCounter, name, age, contact);
        patients.push_back(p);

        cout << "Patient registered: " << name << " (ID: " << patientCounter << ")" << endl;
        return patientCounter++;
    }
    int addDoctor(string name, Department dept)
    {
        Doctor d(doctorCounter, name, dept);
        doctors.push_back(d);

        cout << "Doctor added: " << name << " (ID: " << doctorCounter << ") - " << departmentName(dept) << endl;
        return doctorCounter++;
    }
    void admitPatient(int patientId, RoomType type){
    bool flag=false;
       for (auto& patient : patients){
        if(patient.getId()==patientId){
            patient.admitPatient(type);
            flag=true;
            break;
        }
       }
       if(flag==false){
        cout<<"Patient not found."<<endl;
       }


    };

    void addEmergency(int patientId){

        emergencyQueue.push(patientId);

        cout << "Emergency added for patient " << patientId << endl;
    }
    int handleEmergency(){
        if (!emergencyQueue.empty()){
            int P_Id = emergencyQueue.front();
            emergencyQueue.pop();
            cout << "Handling emergency for patient " << P_Id << endl;
            return P_Id;
        }
        cout << "No emergency cases." << endl;
        return -1;
    }

    void bookAppointment(int doctorId, int patientId){

        bool flagPatient=false;
        Doctor* foundDoctor = nullptr;

        for (auto& i: doctors) {
      	    if (i.getId()==doctorId){
                foundDoctor = &i;
            }
        }

        for (auto j: patients) {
      	    if (j.getId()==patientId){
                flagPatient=true;
            }
        }

        if (foundDoctor==nullptr){
            cout << "Doctor not found." << endl;
        }

        else if (!flagPatient)
        {
            cout << "Patient not found." << endl;
        }

        else{
            foundDoctor->addAppointment(patientId);
        }


    }
    void displayPatientInfo(int patientId){
        for(int i = 0; i< patients.size(); i++){
            if(patients[i].getId() == patientId){
                cout << "=== Patient Information ===" << endl << "ID: " << patients[i].getId() << endl << "Name: " << patients[i].getName() << endl << "Age: " << patients[i].getage() << endl;
                cout << "Contact: " << patients[i].getcontact() << endl;
                cout << "Admission Status: " << (patients[i].getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                if (patients[i].getAdmissionStatus()){
                        cout << "Room Type: " << roomTypeName(patients[i].getroomtype()) << endl;
                }
                cout << "===========================" << endl;
                return;
            }
        }
        cout << "Patient not found." << endl;
        return;
    }


    void displayDoctorInfo(int doctorId)
    {
        for (auto& doctor : doctors)
        {
            if (doctor.getId() == doctorId)
            {
                cout << "=== Doctor Information ===" << endl;
                cout << "ID: "                  << doctor.getId()           << endl;
                cout << "Name: "                << doctor.getName()         << endl;
                cout << "Department: "          << doctor.getDepartment()   << endl;
                cout << "Pending Appointments: "<< doctor.getPendingCount() << endl;
                cout << "=========================" << endl;
                return;
            }
        }
        cout << "Doctor not found." << endl;
    }

    Patient* getPatientById(int patientId) {
        for (auto& patient : patients) {
            if (patient.getId() == patientId) {
                return &patient;
            }
        }
        cout << "Patient not found." << endl;
        return nullptr;
    }

     Doctor* getDoctorById(int doctorId) {
        for (auto& doctor : doctors) {
            if (doctor.getId() == doctorId) {
                return &doctor;
            }
        }
        cout << "Doctor not found." << endl;
        return nullptr;
    }
};

// ========== MAIN PROGRAM ========== //
int main() {
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients
    Patient *pant1= hospital.getPatientById(p1); 
    pant1->addMedicalRecord("Diagnosed with hypertension");
    pant1->requestTest("Blood Test");
    pant1->performTest();
    

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects
    pant1->dischargePatient();
    

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors
    Doctor *doc1=hospital.getDoctorById(d1);
    doc1->addAppointment(p1);


    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    

    return 0;
}