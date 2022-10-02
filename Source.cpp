#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void dashes() {
	cout << "---------------------------------" << endl;
}


class ICanStore {
public:
	virtual void Save() = 0;
	//virtual void Load() = 0;
};

class Storage {
public:
	void Store(ICanStore* i) {
		i->Save();
	}
};


class SubjectDefinition : public ICanStore {
	string SubjName, SubjDescription;
	float MaxGrade, SuccessGrade;
public:

	SubjectDefinition()
		:MaxGrade(100),SuccessGrade(50)
	{}

	void SetSubjectName(string SubjName) {
		this->SubjName = SubjName;
	}
	string GetSubjName() {
		return SubjName;
	}

	void SetSubjDescription(string SubjDescription) {
		this->SubjDescription = SubjDescription;
	}
	string GetSubjDescription() {
		return SubjDescription;
	}

	virtual void Save() {
		ofstream file("Datasheet.txt",ios::app);
		file << SubjName << ": " << SubjDescription << endl;
	}

};

class SubjectController : public ICanStore {
public:
	int NumOfSubjects;
	SubjectDefinition* SubjectsDef;

	SubjectController() {
		cout << "Enter number of subjects: "; 
		cin >> NumOfSubjects;
		SubjectsDef = new SubjectDefinition[NumOfSubjects];
	}

	void SetSubjectsInfo(SubjectDefinition& subj) {
		dashes();
		string SubjName, SubjDescription;
		cout << "Enter subject name: "; 
		cin >> SubjName; 
		cin.ignore();
		subj.SetSubjectName(SubjName);
		cout << "Enter subject description: ";
		getline(cin,SubjDescription);
		subj.SetSubjDescription(SubjDescription);
	}

	int SearchForSubjectByName(string Name) {
		for (int i = 0; i < NumOfSubjects; i++) {
			if (Name == SubjectsDef[i].GetSubjName())
				return i;
		}
		return -1;
	}

	virtual void Save() {
		for (int i = 0; i < NumOfSubjects; i++) {
			SubjectsDef[i].Save();
		}
	}

	void DisplayAllSubjects(SubjectDefinition sd[]) {
		for (int i = 0; i < NumOfSubjects; i++) {
			dashes();
			cout << "Subject Name: " << sd[i].GetSubjName() << endl;
			cout << "Subject Description: " << sd[i].GetSubjDescription() << endl;
		}
	}

	~SubjectController() {
		delete[] SubjectsDef;
		SubjectsDef = NULL;
	}
};

class SubjectData : public ICanStore{
	float Grade;
	SubjectDefinition SubjectDef;
public:

	void SetSubjectDef(SubjectDefinition& SubjectDef) {
		this->SubjectDef = SubjectDef;
	}
	SubjectDefinition GetSubjectDef() {
		return SubjectDef;
	}

	void SetGrade(float Grade) {
		this->Grade = Grade;
	}
	float GetGrade() {
		return Grade;
	}

	virtual void Save() {
		ofstream file("Datasheet.txt",ios::app);
		file << SubjectDef.GetSubjName() << ": " << GetGrade() << endl;
	}
};

class Student : public ICanStore{
	string FirstName, LastName;
	int Age;
	int PhoneNumber;
	
public:
	SubjectData* StudentSubjects = NULL;
	int NumOfStudSubjects;

	void SetNumOfStudentSubjects(int NumOfSubjs) {
		this->NumOfStudSubjects = NumOfSubjs;
		if(!StudentSubjects)
			StudentSubjects = new SubjectData[NumOfStudSubjects];
	}

	void SetFName(string FName) {
		this->FirstName = FName;
	}
	void SetLName(string LName) {
		this->LastName = LName;
	}
	string GetFName() {
		return FirstName;
	}
	string GetLName() {
		return LastName;
	}

	void SetAge(int Age){
		this->Age = Age;
	}
	int GetAge() {
		return Age;
	}

	void SetPhoneNumber(int PhoneNumber) {
		this->PhoneNumber = PhoneNumber;
	}
	int GetPhoneNumber() {
		return PhoneNumber;
	}
	
	virtual void Save() {
		ofstream file("Datasheet.txt",ios::app);
		file << FirstName << " " << LastName << " " << Age << " " << PhoneNumber << " ";
		for (int j = 0; j < NumOfStudSubjects; j++) {
			file << StudentSubjects[j].GetSubjectDef().GetSubjName() << " ";
		}
		file << endl;
		file.close();
	}

	~Student() {
		delete[] StudentSubjects;
	}

};

class StudentController : public ICanStore{
public:
	int NumOfStudents;
	Student* students;

	StudentController() {
		cout << "Enter number of students" << endl;
		cin >> NumOfStudents;
		students = new Student[NumOfStudents];
	}

	void SetStudentInfo(Student& std,SubjectController& subjects) {
		dashes();
		string FirstName, LastName;
		int Age;
		int PhoneNumber;
		int StdNumOfSubjs;

		cout << "Enter Student's First,Last names: " << endl;
		cin >> FirstName >> LastName;
		std.SetFName(FirstName); std.SetLName(LastName);

		cout << "Enter Student's Age: "; cin >> Age;
		std.SetAge(Age);

		cout << "Enter Student's Phone Number: "; cin >> PhoneNumber;
		std.SetPhoneNumber(PhoneNumber);

		do {
			cout << "Enter Student's number of subjects: "; 
			cin >> StdNumOfSubjs;
		} while (StdNumOfSubjs < 1 || StdNumOfSubjs > subjects.NumOfSubjects);

		std.SetNumOfStudentSubjects(StdNumOfSubjs);

		subjects.DisplayAllSubjects(subjects.SubjectsDef);//Show the names of total subjects
		for (int i = 0; i < std.NumOfStudSubjects; i++) {
			
			dashes();
			string SelectedSubject;
			int IsFoundIndex;
			do {
				cout << "Enter the student's subject name!" << endl;
				cin >> SelectedSubject;
				IsFoundIndex = subjects.SearchForSubjectByName(SelectedSubject);
			} while (IsFoundIndex == -1);
			//cout << "INDEX IS: " << IsFoundIndex << endl;
			std.StudentSubjects[i].SetSubjectDef(subjects.SubjectsDef[IsFoundIndex]);
		}
	}

	void SetStudentGrades(Student& std) {
		float Grade;
		for (int i = 0; i < std.NumOfStudSubjects; i++) {
			cout << "Enter student's grade in: " << std.StudentSubjects[i].GetSubjectDef().GetSubjName() << endl;
			cin >> Grade;
			std.StudentSubjects[i].SetGrade(Grade);
		}
	}

	void AddStudent(SubjectController& SubjCont) {
		Student* NewStudent = students;
		NumOfStudents++;
		students = new Student[NumOfStudents];
		for (int i = 0; i < NumOfStudents-1; i++) {
			students[i] = NewStudent[i];
		}
		SetStudentInfo(students[NumOfStudents - 1], SubjCont);
		SetStudentGrades(students[NumOfStudents - 1]);
		cout << "HERE " << endl;
		for (int i = 0; i < students[0].NumOfStudSubjects; i++) {
			cout << students[0].StudentSubjects[i].GetSubjectDef().GetSubjName() << endl;
		}
		//delete[] NewStudent;
		NewStudent = NULL;
	}

	int SearchForStudent(string Fname, string Lname) {
		for (int i = 0; i < NumOfStudents; i++) {
			if (Fname == students[i].GetFName() && Lname == students[i].GetLName())
				return i;
		}
		return -1;
	}

	void ModifyStudent(string Fname,string Lname,SubjectController& SubjCont) {
		int index = SearchForStudent(Fname, Lname);
		if (index != -1) {
			int Choice;
			cout << "Enter to modify: " << endl;
			cout << "1.FirstName" << endl;
			cout << "2.LastName" << endl;
			cout << "3.Age" << endl;
			cout << "4.Phone Number" << endl;
			cout << "5.Students Subjects" << endl;
			cin >> Choice;
			switch (Choice) {
				case 1:
				{
					string Name; 
					cout << "Enter first name: "; cin >> Name;
					students[index].SetFName(Name);
					break;
				}
				case 2:
				{
					string Name;
					cout << "Enter last name: "; cin >> Name;
					students[index].SetLName(Name);
					break;
				}
				case 3:
				{
					int Age;
					cout << "Enter Age: "; cin >> Age;
					students[index].SetAge(Age);
					break;
				}
				case 4:
				{
					int PhoneNumber;
					cout << "Enter Phone Number: "; cin >> PhoneNumber;
					students[index].SetPhoneNumber(PhoneNumber);
					break;
				}
				case 5:
				{
					int StdNumOfSubjs;
					do {
						cout << "Enter Student's number of subjects: ";
						cin >> StdNumOfSubjs;
					} while (StdNumOfSubjs < 1 || StdNumOfSubjs > SubjCont.NumOfSubjects);
					SubjCont.DisplayAllSubjects(SubjCont.SubjectsDef);//Show the names of total subjects
					for (int i = 0; i < students[index].NumOfStudSubjects; i++) {

						dashes();
						string SelectedSubject;
						int IsFoundIndex;
						do {
							cout << "Enter the student's subject name!" << endl;
							cin >> SelectedSubject;
							IsFoundIndex = SubjCont.SearchForSubjectByName(SelectedSubject);
						} while (IsFoundIndex == -1);
						//cout << "INDEX IS: " << IsFoundIndex << endl;
						students[index].StudentSubjects[i].SetSubjectDef(SubjCont.SubjectsDef[IsFoundIndex]);
					}
					break;
				}
			}
		}
		else {
			cout << "Cannot find student" << endl;
		}
	}

	virtual void Save() {
		for (int i = 0; i < NumOfStudents; i++) {
			students[i].Save();
		}
	}

	void DisplayAllStudentsInfo(Student stds[],SubjectController& SubjCont) {
		dashes();
		cout << "Number of students: " << NumOfStudents << endl;
		for (int i = 0; i < NumOfStudents; i++) {
			cout << "Student #" << i + 1 << endl;
			cout << "Student Name: " << stds[i].GetFName() << " " << stds[i].GetLName() << endl;
			cout << "Student Age: " << stds[i].GetAge() << endl;
			cout << "Student Phone Number: " << stds[i].GetPhoneNumber() << endl;
			for (int j = 0; j < stds[i].NumOfStudSubjects; j++) {
				cout << "Student's Grade in (" << stds[i].StudentSubjects[j].GetSubjectDef().GetSubjName()
					 << ") is: " << stds[i].StudentSubjects[j].GetGrade() << endl;
			}
		}
	}

	~StudentController() {
		delete[] students;
		students = NULL;
	}
};


void main() {
	SubjectController Subjs;
	for (int i = 0; i < Subjs.NumOfSubjects; i++) {
		Subjs.SetSubjectsInfo(Subjs.SubjectsDef[i]);
	}
	StudentController Stds;
	for (int i = 0; i < Stds.NumOfStudents; i++) {
		Stds.SetStudentInfo(Stds.students[i],Subjs);
		Stds.SetStudentGrades(Stds.students[i]);
	}
	
	ICanStore* ican = 0;
	Storage stg;

	
	int MenuChoice;
	
	char choice;
	do {
		cout << "Press 1 to display all students info!" << endl;
		cout << "Press 2 to display all subjects info!" << endl;
		cout << "Press 3 to save all subjects info!" << endl;
		cout << "Press 4 to save all students info!" << endl;
		cout << "Press 5 to add a student" << endl;
		cout << "Press 6 to modify a student" << endl;
		cin >> MenuChoice;
		switch (MenuChoice) {
		case 1:
			Stds.DisplayAllStudentsInfo(Stds.students, Subjs);
			break;
		case 2:
			Subjs.DisplayAllSubjects(Subjs.SubjectsDef);
			break;

		case 3:
			ican = &Subjs;//Act as SubjectController
			stg.Store(ican);//Save the total subjects
			break;
		case 4:
			ican = &Stds;//Act as StudentController
			stg.Store(ican);//Save all the students data
			break;
		case 5:
			Stds.AddStudent(Subjs);
			break;
		case 6:
			string f_name, l_name;
			cout << "Enter first,last names of the student you want to modify! " << endl;
			cin >> f_name >> l_name;
			Stds.ModifyStudent(f_name, l_name, Subjs);
			break;
		}
		cout << "Enter y/Y to have another choice: "; 
		cin >> choice;
	} while (choice == 'y' || choice == 'Y');
	ican = NULL;
}