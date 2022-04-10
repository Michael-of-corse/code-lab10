#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
#include<conio.h>
#include<Windows.h>

using namespace std;

vector<string> names;

template<typename... Rest> void menu(string first, Rest... rest) {
	int k = sizeof...(rest);
	bool f = false;
	if (k > 0)f = true;
	((cout << "*****" << endl << first << ":" << endl), ..., (cout << "    " << sizeof... (rest) - (--k) << ". " << rest << endl));
	if (f)cout << "    0. Выход" << endl;
};
bool comp(vector<string> a,string b) {
	return(a.end() != find(a.begin(), a.end(), b));
};
class user
{
private:
	string name_;
	string pass_;
public:
	user() {};
	user(string name, string pass) : name_(name), pass_(pass) {};
	bool operator==(const user& t1) {
		return (t1.name_ == name_) && (t1.pass_ == pass_);
	};
	void operator()(string name, string pass) {
		this->name_ = name; this->pass_ = pass;
	};
	void view_data() {
		ifstream f1("input1.txt");
		ifstream f2("input2.txt");
		try {
			if (!f1 || !f2)throw"Не удалось открыть файлы";
			menu("Просмотр данных текстового файла");
			cout << "     файл1       файл2" << endl;
			while (f1.peek() != EOF || f2.peek() != EOF) {
				int a, b;
				 //if (!f2.eof())
				cout << "     " << setw(12) << left;
				if (!f1.eof()) {
					f1 >> a;
					cout << a;
				}
				else { cout << " -"; };
				cout << right;
				if (!f2.eof()) {
					f2 >> b;
					cout << b;
				}
				else { cout << " -"; }
				cout<< endl;
			};
			f1.clear();
			f2.clear();
			f1.close();
			f2.close();
		}
		catch (const char* s) {
			cout << s << endl;
			f1.close();
			f2.close();
		}
	}
	~user() {};
};
class upuser :public user{
public:
	using user::user;
	using user::operator==;
	void do_file() {
		ofstream f("data.txt");
		ifstream f1("input1.txt");
		ifstream f2("input2.txt");
		try {
			menu("Добавить данные в файл");
			int a, b;
			while (f1.peek() != EOF&&f2.peek()!=EOF) {
				f1 >> a; f2 >> b;
			};
			if (!(f2.eof()&&f1.eof()))throw"Количество чисел в файлах не равно, ошибка доступа!\n";
			f1.clear(); f2.clear();
			f1.seekg(0, f1.beg); f2.seekg(0, f2.beg);
			while (f1.peek() != EOF)
			{
				f1 >> a; f2 >> b;
				if (b % a == 0) f << b / a << endl;
			};
			f.close();
			cout << "Данные успешно добавлены\n";
			f1.close(); f2.close();
		}
		catch(const char*s){
			cout << s;
			f1.close(); f2.close();
		}
	}
};
vector<upuser> upusers;
vector<user> users;
class admin : public user{
public:
	using user::user;
	using user::operator();
	using user::operator ==;
	void create_user() {
		ofstream f("users.txt", ios::app);
		menu("Создать нового пользователя");
		try {
			string name,pass;
			int lvl;
			cout << "    Введите имя пользователя - ";
			cin >> name;
			if (comp(names,name))throw"\nТакой пользователь уже есть!";
			cout << "     Введите уровень доступа (1 для расширенного пользователя, 2 для обычного) ";
			cin >> lvl;
			if (lvl != 1 && lvl !=2)throw"Некорректный ввод уровня доступа";
			cout << "     Введите пароль - ";
			cin >> pass;
			if (lvl == 1) {
				upusers.push_back(upuser(name, pass));
			}
			else if (lvl == 2) {
				users.push_back(user(name,pass));
			};
			names.push_back(name);
			f <<endl <<lvl << endl << name << endl << pass;
			cout << "Пользователь добавлен" << endl;
		}
		catch(const string *s){
			cout <<endl<< s;
		}
		f.clear();
		f.close();
	}
};
template<class T> bool comp(vector<T> t, T d) {
	typename vector<T>::iterator it = find(t.begin(), t.end(), d);
	if (it != t.end()) {
		if constexpr (std::is_same<T, user>::value) {
			menu("Меню", "Просмотр данных текстового файла");
			while (!_kbhit()) {
				if ((char)_getch() == '1')it->view_data();
				if ((char)_getch() == '0')throw"Успешно вышли из записи";
			};
		}
		else if constexpr (std::is_same<T, upuser>::value) {
			menu("Меню", "Просмотр данных текстового файла", "Добавить данные в файл");
			while (!_kbhit()) {
				if ((char)_getch() == '1')it->view_data();
				if ((char)_getch() == '2')it->do_file();
				if ((char)_getch() == '0')throw"Успешно вышли из записи";
			};
		};
		return true;
	}else { return false; };
};
admin myad;
int main() {
	ifstream f("users.txt");  
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	try {
		int k = 0;
		if (!f)throw"Не удалось открыть файл";
		while (f.peek() != EOF) {//заполнение векторов
			int a; string name; string pass;
			f >> a;
			if (f.eof())throw"Неполная запись\nИсправьте файл users.txt";
			if (a != 2 && a != 3&&a!=1)throw"Неправильный уровень доступа в файле\nИсправьте файл users.txt";
			if (a == 3)k++; if (k > 1)throw"Два пользователя с правами администратора!\nИсправьте файл users.txt";
			f >> name;
			if (f.eof())throw"Неполная запись\nИсправьте файл users.txt";
			(comp(names, name)) ? throw"Одинаковые имена в файле ошибка!\nИсправьте файл users.txt" : names.push_back(name);
			f >> pass;
			if (a == 3) { myad(name, pass); }
			else if (a == 1) {
				upusers.push_back(upuser(name, pass));
			}
			else if (a == 2) {
				users.push_back(user(name, pass));
			}
		};
		while (true) {
			try {
				bool flag = false;
				string name, pass;
				cout << "Введите имя пользоватея - ";
				cin >> name;
				if (!comp(names, name))throw"Неверное имя пользователя!";
				cout << "Введите пароль - ";
				cin >> pass;
				flag=flag||comp(users, user(name, pass));
				flag=flag||comp(upusers, upuser(name, pass));
				if (myad == admin(name, pass))
				{
					while (!_kbhit()) {
						menu("Меню", "Добавить нового пользователя");
						if ((char)_getch() == '1')myad.create_user();
						if ((char)_getch() == '0')throw"Успешно вышли из записи";
					};
					flag = flag||true;
				};
				(flag) ? throw"" : throw"Неверный пароль!";
			}
			catch (const char* s) {
				cout << s << endl;
				system("pause");
				system("cls");
			}
		}

	}
	catch(const char* s){
		cout << s;
	}
}