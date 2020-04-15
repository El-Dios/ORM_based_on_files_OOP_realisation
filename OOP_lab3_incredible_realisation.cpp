#include <iostream>
#include <iomanip> 
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <Windows.h>

using namespace std;


class Animal
{
	string nickname, spec;
	int age;

public:

	Animal() {}
	Animal(string _nickname, string _spec, int _age) :
		nickname(_nickname), spec(_spec), age(_age) {}
	string get_nickname() { return this->nickname; }
	string get_spec() { return this->spec; }
	int get_age() { return this->age; }
	void set_nickname(string _nickname) { this->nickname = _nickname; }
	void set_spec(string _spec) { this->spec = _spec; }
	void set_age(int _age) { this->age = _age; }

	virtual void voice() = 0;
	virtual void call() = 0;

	void (Animal::* fcnPtrArr[2])() =
	{
		&Animal::voice,
		&Animal::call
	};
};


class Cat : public Animal
{
public:

	Cat() {}
	Cat(string _nickname, string _spec, int _age)
		: Animal(_nickname, _spec, _age) {}

	virtual void voice() { cout << "Мяу\n"; }
	virtual void call() { cout << "Мурр\n"; }
};

class Dog : public Animal
{
public:

	Dog() {}
	Dog(string _nickname, string _spec, int _age)
		: Animal(_nickname, _spec, _age) {}

	virtual void voice() { cout << "Гав-гав\n"; }
	virtual void call() { cout << "Вуууу..\n"; }
};

class Pbd {

	struct adjunct {
		string nickname;
		string spec;
		int age;
	};

	const enum string_code
	{
		eNickname,
		eSpec,
		eAge
	};

	static string_code hashit(string inString) {
		for_each(inString.begin(), inString.end(), [](char& c) {
			c = ::tolower(c);
			});
		try {
			if (inString == "nickname") return eNickname;
			if (inString == "spec") return eSpec;
			if (inString == "age") return eAge;
			else {
				throw 228;
			}
		}
		catch (int i)
		{
			cout << "Ошибка " << i << " - введено некорректное имя поля!!!" << endl;
		}
	}

	static void Print_obj(Animal* p)
	{
		cout << setw(10) <<p->get_nickname() << "|" << setw(5) << p->get_spec() << "|" << setw(3) << p->get_age() << "|" << "\n";
	}

	static void Rewrite_data(const string& filename, vector<Animal*>& vec_anml)
	{
		ofstream fout;
		fout.open(filename, ofstream::out);
		if (!fout.is_open())
		{
			cout << "Error with oppening file" << endl;
		}
		else
		{
			for (Animal* p : vec_anml) {
				string str;
				str = p->get_nickname() + " " + p->get_spec() + " " + to_string(p->get_age()) + "\n";
				fout << str;
				//fout.write(tmp.c_str(), tmp.length());
			}
		}
		fout.close();
	}

public:

	static vector<Animal*> Get_data(const string& filename)
	{
		ifstream fin(filename);
		vector<Animal*> anml_data;
		vector<adjunct> addit;
		if (!fin.is_open())
		{
			cout << "Error with oppening file" << endl;
		}
		else
		{
			int c_amount = 0, d_amount = 0, ad = 0;
			while (!fin.eof())
			{
				string str_to_transform;
				getline(fin, str_to_transform);
				if (str_to_transform != "" && str_to_transform != "\n")
				{
					istringstream iss(str_to_transform);
					vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());
					if (results[1] == "cat") {
						adjunct adj; 
						adj.nickname = results[0];
						adj.spec = results[1];
						adj.age = stoi(results[2]);
						addit.push_back(adj);
						c_amount++;
					}
					else if (results[1] == "dog") {
						adjunct adj;
						adj.nickname = results[0];
						adj.spec = results[1];
						adj.age = stoi(results[2]);
						addit.push_back(adj);
						d_amount++;
					}
				}
			}

			Cat* c_buff = new Cat[c_amount]; 
			Dog* d_buff = new Dog[d_amount]; 
			for (int i = 0, c = 0, d = 0; i < addit.size(); i++) 
			{
				if (addit[i].spec == "cat") {
					c_buff[c] = Cat(addit[i].nickname, addit[i].spec, addit[i].age);
					anml_data.push_back(&c_buff[c]);
					c++;
				}

				else if (addit[i].spec == "dog") {
					d_buff[d] = Dog(addit[i].nickname, addit[i].spec, addit[i].age);
					anml_data.push_back(&d_buff[d]);
					d++;
				}
			}
		}
		fin.close();
		return anml_data;
	}

	static int Get_num_str(const string& filename)
	{
		ifstream fin(filename);
		int str_num = 0;
		if (!fin.is_open())
		{
			cout << "Error with oppening file" << endl;
		}
		else
		{
			char* buf_str = new char[1024];
			while (!fin.eof())
			{
				fin.getline(buf_str, 1024, '\n');
				str_num++;
			}
			delete buf_str;
		}
		fin.close();
		if (str_num <= 0)
			return 0;
		else
			return str_num - 1;
	}

	static void Add_data(const string& filename, Animal& anml)
	{
		string str;
		str = anml.get_nickname() + " " + anml.get_spec() + " " + to_string(anml.get_age()) + "\n";
		ofstream fout;
		fout.open(filename, ofstream::app);
		if (!fout.is_open())
		{
			cout << "Error with oppening file" << endl;
		}
		else
		{
			fout << str;
			//fout.write(tmp.c_str(), tmp.length());
		}
		fout.close();
	}

	static void Modify_line(const string& filename, int line_num,
		string _nickname, string _spec, int _age)
	{
		vector<Animal*>data = Get_data(filename);
		data[line_num - 1]->set_nickname(_nickname);
		data[line_num - 1]->set_spec(_spec);
		data[line_num - 1]->set_age(_age);
		Rewrite_data(filename, data);
	}

	static void Delete_line(const string& filename, int line_num)
	{
		vector<Animal*>data = Get_data(filename);
		data.erase(data.begin() + line_num - 1);
		vector<Animal*>(data).swap(data);
		Rewrite_data(filename, data);
	}

	static void Find_data(const string& filename, string field, string value)
	{
		//Sort_data(filename, field);
		vector<Animal*> data = Get_data(filename);
		string_code f = hashit(field);

		vector<Animal*> data_result;
		int i = 1;
		vector<int> k;
		for_each(data.begin(), data.end(), [&f, &value, &data_result, &i, &k](Animal* _p)
			{
				switch (f)
				{
				case 0:
					if (_p->get_nickname() == value) {
						data_result.push_back(_p);
						k.push_back(i);
					}
				case 1:
					if (_p->get_spec() == value) {
						data_result.push_back(_p);
						k.push_back(i);
					}
				case 2:
					if (to_string(_p->get_age()) == value) {
						data_result.push_back(_p);
						k.push_back(i);
					}
				default:
					break;
				}
				i++;
			});

		if (data_result.size() == 0)
			cout << "Данных, соответствующих запросу, не найдено :(\n";
		else
		{
			int i = 0;
			for_each(data_result.begin(), data_result.end(), [&i, &k](Animal* _p)
				{
					cout << setw(3) << k[i] << "|";
					Print_obj(_p);
					i++;
				});
		}
		
	}

	static void Sort_data(const string& filename, string field)
	{
		vector<Animal*>data = Get_data(filename);
		string_code f = hashit(field);
		sort(data.begin(), data.end(), [f](Animal* a, Animal* b) {
			switch (f)
			{
			case 0:
				return a->get_nickname() < b->get_nickname();
			case 1:
				return a->get_spec() < b->get_spec();
			case 2:
				return a->get_age() < b->get_age();
			}
			});
		Rewrite_data(filename, data);
	}

	static void Print_all(const string& filename)
	{
		vector<Animal*> data = Get_data(filename);
		int i = 1, tmp;
		tmp = Pbd::Get_num_str(filename);
		if (tmp == 0)
			cout << "В настоящий момент файл пуст\n"
			<< "Но вы можете заполнить его значениями";
		else {
			if (tmp / 10 != 0)
				tmp = 3;
			else
				tmp = 2;
			cout << right << setw(tmp) << "id" << "|"
				<< setw(11) << "Nickname" << "|"
				<< setw(5) << "Spec" << "|"
				<< setw(3) << "Age" << "|\n";
			if (tmp == 3)
				cout << "---+-----------+-----+---+\n";
			else if (tmp == 2)
				cout << "--+-----------+-----+---+\n";
			for (Animal* n : data) {
				cout << setw(tmp) << i << "| ";
				Print_obj(n);
				i++;
			}
		}
	}
	static void Ask_animals(const string& filename)
	{
		vector<Animal*> data = Get_data(filename);

		for (Animal* n : data) {
			Print_obj(n);
			for (int i = 0; i != 2; i++) {
				(n->*n->fcnPtrArr[i])();
			}
			cout << "\n";
		}
	}
};

bool Inviter(bool cond) {
	char sol;
	bool sol_cond = true;
	cout << "\n";
	while (sol_cond) {
		cout << "Вернуться к выбору действий [y/n]? ";
		cin >> sol;
		switch (sol)
		{
		case 'y':
		case 'Y':
			sol_cond = false;
			continue;
		case 'n':
		case 'N':
			sol_cond = false;
			cond = false;
		default:
			continue;
		}
	}
	return cond;
}

int main()
{
	system("chcp 1251");

	const string path = "DB.txt";
	bool cond = true;

	while (cond)
	{
		system("CLS");
		cout << "1. Вывести все записи\n"
			<< "2. Добавить новую запись\n"
			<< "3. Удалить строку записи\n"
			<< "4. Изменить строку записи\n"
			<< "5. Найти записи по значению\n"
			<< "6. Отсортировать по полю и вывести все записи\n"
			<< "7. Крикнуть животным!\n"
			<< "0. Выход\n";
		
		int u_inp;
		cout << "Ваше действие: ";
		cin >> u_inp;

		switch (u_inp)
		{
		case 1: 
		{
			system("CLS");
			Pbd::Print_all(path);
			cond = Inviter(cond);
			continue;
		}

		case 2:
		{
			system("CLS");
			string nick, r_spec;
			int age, u_spec;
			cout << "Введите кличку животного: ";
			cin >> nick;
			cout << "Выберите вид:\n\t1. Кот\n\t2. Собака\n: ";
			cin >> u_spec;
			switch (u_spec)
			{
			case 1:
				r_spec = "cat";
				break;
			case 2:
				r_spec = "dog";
				break;
			default:
				continue;
			}
			cout << "Введите его возраст: ";
			bool dgd = true;
			while (dgd) {
				cin >> age;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(32767, '\n');
				}
				else
					dgd = false;
			}
			if (u_spec == 1)
			{
				Cat neko(nick, r_spec, age);
				Pbd::Add_data(path, neko);
			}
			else if (u_spec == 2)
			{
				Dog inu(nick, r_spec, age);
				Pbd::Add_data(path, inu);
			}
			cout << "\n";
			cond = Inviter(cond);
			continue;
		}

		case 3:
		{
			system("CLS");
			int str_2_del, str_in = Pbd::Get_num_str(path);
			cout << "Всего строк с записями: "
				<< str_in << "\n"
				<< "Введите номер строки для удаления: ";
			cin >> str_2_del;
			if (0 < str_2_del <= str_in)
			{
				Pbd::Delete_line(path, str_2_del);
				cout << "Строка" << str_2_del << " успешно удалена\n";
			}
			else
				cout << "Неверный номер строки!\n";
			cond = Inviter(cond);
			continue;
		}

		case 4:
		{
			system("CLS");
			string nick, age, r_spec;
			int str2mod, u_spec, str_in = Pbd::Get_num_str(path);
			cout << "Всего строк с записями: "
				<< str_in << "\n"
				<< "Введите номер строки для изменения: ";
			cin >> str2mod;
			if (0 < str2mod <= str_in)
			{
				cout << "Введите кличку животного: ";
				cin >> nick;
				cout << "Выберите вид:\n\t1. Кот\n\t2. Собака\n: ";
				cin >> u_spec;
				switch (u_spec)
				{
				case 1:
					r_spec = "cat";
					break;
				case 2:
					r_spec = "dog";
					break;
				default:
					continue;
				}
				cout << "Введите его возраст: ";
				cin >> age;
				Pbd::Modify_line(path, str2mod, nick, r_spec, stoi(age));
			}
			cout << "\n";
			cond = Inviter(cond);
			continue;
		}

		case 5:
		{
			system("CLS");
			int u_field;
			string r_field, u_value;
			cout << "Выберите поле:\n\t1. Кличка\n\t2. Вид\n\t3. Возраст\n: ";
			cin >> u_field;
			if (0 < u_field < 4) {
				switch (u_field)
				{
				case 1:
					r_field = "nickname";
					break;
				case 2:
					r_field = "spec";
					break;
				case 3:
					r_field = "age";
					break;
				default:
					continue;
				}
			}
			else
				continue;
			cout << "Введите значение: ";
			cin >> u_value;
			Pbd::Find_data(path, r_field, u_value);
			cout << "\n";
			cond = Inviter(cond);
			continue;
		}

		case 6:
		{
			system("CLS");
			int u_field;
			string r_field;
			cout << "Выберите поле:\n\t1. Кличка\n\t2. Вид\n\t3. Возраст\n: ";
			cin >> u_field;
			if (0 < u_field < 4) {
				switch (u_field)
				{
				case 1:
					r_field = "nickname";
					break;
				case 2:
					r_field = "spec";
					break;
				case 3:
					r_field = "age";
					break;
				default:
					continue;
				}
			}
			else
				continue;
			Pbd::Sort_data(path, r_field);
			system("CLS");
			Pbd::Print_all(path);
			cout << "\n";
			cond = Inviter(cond);
			continue;
		}

		case 7:
		{
			system("CLS");
			string tmp;
			cout << "Наберите побольше воздуха в лёгкие, чтобы крикнуть так громко,\nчтобы все "
				<< Pbd::Get_num_str(path)
				<< " животных вас услышали!\nКак будете готовы нажмите ВВОД\n";
			cin.ignore(1000, '\n');
			getline(cin, tmp);
			system("CLS");
			Pbd::Ask_animals(path);
			cout << "\n";
			cond = Inviter(cond);
			continue;
		}

		case 0:
		{
			cond = false;
			continue;
		}

		default:
			system("CLS");
			break;
		}
	}

	/*//Автозаполнение файла
	for (int i = 1; i <= 10; ++i) {
		int t = rand() % 100;
		if (t % 2 == 0) {
			Dog first("Барбос", "dog", t);
			Pbd::Add_data(path, first);
			int t = rand() % 100;
			Cat second("Мурзик", "cat", t);
			Pbd::Add_data(path, second);
		}
		else
		{
			Dog first("Рэкс", "dog", t);
			Pbd::Add_data(path, first);
			int t = rand() % 100;
			Cat second("Пушок", "cat", t);
			Pbd::Add_data(path, second);
		}
		
	}*/

    return 0;
}
