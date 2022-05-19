#include <iostream>
#include <iomanip>
#include <string.h>
#include <Windows.h>
#include <io.h>
#include <regex>
#include <wincrypt.h>

#define STRLEN 20

using namespace std;

const char* main_menu[] = {
	"1 - Создание нового файла",
	"2 - Генерация нового файла",
	"3 - Загрузка существующего файла",
	"4 - Печать файла",
	"5 - Реализация 50 варианта",
	"6 - Выйти из программы"
};
const char* sub_menu[] = {
	"1 - Печать файла",
	"2 - Добавление записи",
	"3 - Изменение записи",
	"4 - Удаление записи",
	"5 - Зашифровать",
	"6 - Расшифровать",
	"7 - Вернуться в главное меню"
};
const char* sub_menu2[] = {
	"1 - Фамилия ",
	"2 - Имя ",
	"3 - Отчество ",
	"4 - Дата рождения ",
	"5 - Год поступления ",
	"6 - Институт ",
	"7 - Кафедра ",
	"8 - Группа ",
	"9 - Номер зачётки ",
	"10 - Пол ",
	"11 - Предметы ",
	"12 - Вернуться в меню"
};
const char* nazvanie_faila[] = { "\nУкажите название файла " };
const char* polja_students[] = {
	"\nФамилия ",
	"Имя ",
	"Отчество ",
	"Дата рождения ",
	"Год поступления ",
	"Институт ",
	"Кафедра ",
	"Группа ",
	"Номер зачетки ",
	"Пол (Мужчина - M, Женщина - W) ",
	"Предмет "
};
const char* polja_semestr[] = {
	"Количество предметов в семестре "
};
const char* polja_predmet[] = {
	"Название предмета ",
	"Оценка "
};
const char* Error[] = { "\nОшибка: дата рождения не соответствует году поступления\n",
					 "\nОшибка: ввод номера месяца\n",
					 "\nОшибка: ввод дня месяца\n",
					 "\nОшибка: название предмета повторяется в данной сессии\n",
					 "\nОшибка: ввод оценки\n",
					 "\nОшибка: данный файл отсутствует в системе\n",
					 "\nОшибка: данный файл пустой\n",
					 "\nОшибка: такой номер зачетки уже вводился",
					 "\nОшибка: кол-во семестров выходит за пределы\n",
					 "\nОшибка: кол-во предметов выходит за пределы\n",
					 "\nОшибка: некорректный формат ввода\n",
					 "\nОшибка: некорректный формат пола"
};

struct Date {
	int d;
	int m;
	int y;
};
struct Predmet { int mark; char Name[20]; };
struct Semestr { Predmet p[10]; int Pnum; int count; };
struct Student {
	char F[STRLEN];
	char N[STRLEN];
	char O[STRLEN];
	Date bdyear;
	Date gryear;
	char inst[STRLEN];
	char cafedra[STRLEN];
	char group[STRLEN];
	char zachetka[STRLEN];
	char sex;
	Semestr S[10];
	int Snum;
};

class Check{
private:
	int flag;
	FILE* file;
	Student student;
public:
	int Error_exist(char* file_name) {
		return _access(file_name, 0);
	}
	int Error_zero(char* file_name) {
		fopen_s(&file, file_name, "rb");
		fseek(file, 0, SEEK_END);
		if (ftell(file) == 0) {
			return -1;
		}
		fclose(file);
	}
	int Error_zachetka(FILE* file, char* studak) {
		flag = 0;
		fseek(file, 0, SEEK_SET);
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			if (strcmp(student.zachetka, studak) == 0) {
				flag = 1;
			}
		}
		return flag;
	}
	int Error_name(char* name) {
		regex regName("[A-Z][a-z]+");
		flag = regex_match(name, regName);
		return flag;
	}
	int Error_data_month(int month) {
		flag = 0;
		if (month < 1 || month > 12) {
			flag = 1;
		}
		return flag;
	}
	int Error_data_day(int month, int day, int year) {
		flag = 0;
		switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if (day < 1 || day > 31) { flag = 1; } break;
		case 4: case 6: case 9: case 11:
			if (day < 1 || day > 30) { flag = 1; } break;
		case 2:
			if (year % 4 == 0) {
				if (day < 1 || day > 29) { flag = 1; }
			}
			else if (day < 1 || day > 28) { flag = 1; }
			break;
		default: flag = 1;
		}
		return flag;
	}
	int Error_sex(char sex) {
		flag = 0;
		if (sex != 'W' && sex != 'M') {
			flag = 1;
		}
		return flag;
	}
	int Error_semestr_num(int sem_num) {
		flag = 0;
		if (sem_num < 1 || sem_num > 9) {
			flag = 1;
		}
		return flag;
	}
	int Error_pred_num(int pred_num) {
		flag = 0;
		if (pred_num < 1 || pred_num > 10) {
			flag = 1;
		}
		return flag;
	}
	int Error_repeat_pred(Semestr* sem, int count_predmet) {
		flag = 0;
		for (int k = 0; k < count_predmet; k++) {
			if (strcmp(sem->p[k].Name, sem->p[count_predmet].Name) == 0) {
				flag = 1;
			}
		}
		return flag;
	}
	int Error_repeat_pred(Semestr sem, int count_predmet) {
		flag = 0;
		for (int k = 0; k < count_predmet; k++) {
			if (strcmp(sem.p[k].Name, sem.p[count_predmet].Name) == 0) {
				flag = 1;
			}
		}
		return flag;
	}
	int Error_mark(int mark) {
		flag = 0;
		if (mark < 2 || mark > 5) {
			flag = 1;
		}
		return flag;
	}
};
class Input : public Check {
private:
	int vibor;
public:
	void predmet_null(Predmet* p) {
		p->mark = 0;
		memset(p->Name, '\0', STRLEN);
	}
	void semestr_null(Semestr* s) {
		for (int i = 0; i < 10; ++i) {
			predmet_null(&s->p[i]);
		}
		s->Pnum = 0;
		s->count = 0;
	}
	void student_null(Student* s) {
		memset(s->F, '\0', STRLEN);
		memset(s->N, '\0', STRLEN);
		memset(s->O, '\0', STRLEN);
		memset(s->inst, '\0', STRLEN);
		memset(s->cafedra, '\0', STRLEN);
		memset(s->group, '\0', STRLEN);
		memset(s->zachetka, '\0', STRLEN);
		s->sex = '\0';
		for (int i = 0; i < 9; ++i) {
			semestr_null(&s->S[i]);
		}
		s->Snum = 0;
	}
	void clear_buf() {
		while (cin.get() != '\n');
	}
	void input(char* dest, int str_len) {
		printf("\nВвод >> ");
		scanf_s("%s", dest, str_len);
		printf("\n");
		clear_buf();
	}
	void input(int* dest) {
		*dest = 1234;
		a1:
		printf("\nВвод >> ");
		scanf_s("%d", dest);
		clear_buf();
		if (*dest == 1234) {
			printf("Ошибка формата ввода!\n");
			goto a1;
		}
		printf("\n");
	}
	void input(char* dest) {
		printf("\nВвод >> ");
		scanf_s("%c", dest);
		printf("\n");
		clear_buf();
	}
	void input(Student* student, FILE* file) {
		student_null(student);
	name1:
		printf("%s ", polja_students[0]);
		input((*student).F, STRLEN - 1);
		if (Error_name((*student).F) == 0) {
			printf("%s", Error[10]);
			goto name1;
		}


	name2:
		printf("%s ", polja_students[1]);
		input((*student).N, STRLEN - 1);
		if (Error_name((*student).N) == 0) {
			printf("%s", Error[10]);
			goto name2;
		}


	name3:
		printf("%s ", polja_students[2]);
		input((*student).O, STRLEN - 1);
		if (Error_name((*student).O) == 0) {
			printf("%s", Error[10]);
			goto name3;
		}


	data1:
		printf("%s ", polja_students[3]);
		printf("\nДень: ");
		input(&(*student).bdyear.d);
	data2:
		printf("Месяц: ");
		input(&(*student).bdyear.m);

		if (Error_data_month((*student).bdyear.m) == 1) {
			printf("%s", Error[1]);
			goto data2;
		}
		printf("Год: ");
		input(&(*student).bdyear.y);
		if (Error_data_day((*student).bdyear.m, (*student).bdyear.d, (*student).bdyear.y) == 1) {
			printf("%s", Error[2]);
			goto data1;
		}


	data3:
		printf("%s", polja_students[4]);
		input(&(*student).gryear.y);
		if (((*student).gryear.y - (*student).bdyear.y) < 17) {
			printf("%s", Error[0]);
		data4:
			printf("0 - Изменить дату рождения\n1 - Изменить год поступления\n");
			input(&vibor);
			switch (vibor) {
			case 0: goto data2; break;
			case 1: goto data3; break;
			default: printf("Введите корректное значение!"); goto data4;
			}
		}


		printf("%s", polja_students[5]);
		input((*student).inst, STRLEN - 1);


		printf("%s", polja_students[6]);
		input((*student).cafedra, STRLEN - 1);


		printf("%s", polja_students[7]);
		input((*student).group, STRLEN - 1);


	stud1:
		printf("%s", polja_students[8]);
		input((*student).zachetka, STRLEN - 1);
		if (Error_zachetka(file, (*student).zachetka) == 1) {
			printf("%s", Error[7]);
			goto stud1;
		}


	sex1:
		printf("%s", polja_students[9]);
		input(&(*student).sex);
		if (Error_sex((*student).sex) == 1) {
			printf("%s", Error[11]);
			goto sex1;
		}


	sem1:
		printf("Введите кол-во семестров: ");
		input(&(*student).Snum);
		if (Error_semestr_num((*student).Snum) == 1) {
			printf("%s", Error[8]);
			goto sem1;
		}
		for (int i = 0; i < (*student).Snum; ++i) {
			(*student).S[i].count = i + 1;
			printf("Ввод %d семестра\n", i + 1);
		sem2:
			printf("%s: ", polja_semestr[0]);
			input(&(*student).S[i].Pnum);
			if (Error_pred_num((*student).S[i].Pnum) == 1) {
				printf("%s", Error[9]);
				goto sem2;
			}
			for (int j = 0; j < (*student).S[i].Pnum; j++) {
			sem3:
				printf("%s ", polja_predmet[0]);
				input((*student).S[i].p[j].Name, STRLEN - 1);
				if (Error_repeat_pred(&student->S[i], j) == 1) {
					printf("%s", Error[3]);
					goto sem3;
				}
			sem4:
				printf("%s: ", polja_predmet[1]);
				input(&(*student).S[i].p[j].mark);
				if (Error_mark((*student).S[i].p[j].mark)) {
					printf("%s", Error[4]);
					goto sem4;
				}
			}
		}
	}
};
class Print {
public:
	void print_zapis(Student* student) {
		printf("\n");
		int size = strlen((*student).F) + strlen((*student).N) + strlen((*student).O) + 20;
		cout << "|" << setfill('-') << setw(size) << "|" << endl;
		cout << "| ФИО: " << (*student).F << " " << (*student).N << " " << (*student).O << " |" << " Пол: " << (*student).sex << setfill(' ') << setw(size - 17 - strlen((*student).F) - strlen((*student).N) - strlen((*student).O)) << "|" << endl;
		cout << "|" << setfill('-') << setw(size) << "|" << endl;
		cout << "| Институт: " << (*student).inst << " |" << " Кафедра: " << (*student).cafedra << setfill(' ') << setw(size - (23 + strlen((*student).cafedra) + strlen((*student).inst))) << "|" << endl;
		cout << "|" << setfill('-') << setw(size) << "|" << endl;
		cout << "| Группа: " << (*student).group << " |" << " Студак: " << (*student).zachetka << setfill(' ') << setw(size - (20 + strlen((*student).group) + strlen((*student).zachetka))) << "|" << endl;
		cout << "|" << setfill('-') << setw(size) << "|" << endl;
		printf("| Д/Р: %02d.%02d.%d ", (*student).bdyear.d, (*student).bdyear.m, (*student).bdyear.y);
		printf("| Г/П: %d", (*student).gryear.y);
		cout << setfill(' ') << setw(size - 28) << "|" << endl;
		cout << "|" << setfill('-') << setw(size) << "|" << endl;
		for (int i = 0; i < (*student).Snum; ++i) {
			cout << "|" << setfill(' ') << setw(size / 2 + 5) << "Семестр # " << (*student).S[i].count << setw(size / 2 - 5) << "|" << endl;
			cout << "|" << setfill('-') << setw(size) << "|" << endl;
			for (int j = 0; j < (*student).S[i].Pnum; ++j) {
				cout << "| " << (*student).S[i].p[j].Name << " : " << (*student).S[i].p[j].mark << setfill(' ') << setw(size - 5 - strlen((*student).S[i].p[j].Name)) << "|" << endl;
				cout << "|" << setfill('-') << setw(size) << "|" << endl;
			}
		}
	}
	void print_main_menu() {
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		cout << "|" << setfill(' ') << setw(23) << "ГЛАВНОЕ МЕНЮ" << setw(12) << "|" << endl;
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		for (int i = 0; i < 6; ++i) {
			cout << "| " << main_menu[i] << setfill(' ') << setw(34 - strlen(main_menu[i])) << "|" << endl;
		}
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
	}
	void print_sub_menu() {
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		cout << "|" << setfill(' ') << setw(21) << "МЕНЮ ФАЙЛА" << setw(14) << "|" << endl;
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		for (int i = 0; i < 7; ++i) {
			cout << "| " << sub_menu[i] << setfill(' ') << setw(34 - strlen(sub_menu[i])) << "|" << endl;
		}
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
	}
	void print_sub_menu2() {
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		cout << "|" << setfill(' ') << setw(25) << "МЕНЮ ИЗМЕНЕНИЯ" << setw(10) << "|" << endl;
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
		for (int i = 0; i < 12; ++i) {
			cout << "| " << sub_menu2[i] << setfill(' ') << setw(34 - strlen(sub_menu2[i])) << "|" << endl;
		}
		cout << "|" << setfill('-') << setw(35) << "|" << endl;
	}
};
class File : public Print, public Input{
private:
	Student student;
	FILE* file_izm;
	FILE* file;
	char file_name[STRLEN];
	int vibor;
	DWORD count;
public:
	void create_new_file() {
		printf("Введите название файла");
		input(file_name, STRLEN - 1);
		fopen_s(&file, file_name, "w+b");
		for (int count = 0; ; count++) {
			printf("Введите информацию о %d студенте:\n", count + 1);
			input(&student, file);
			fwrite(&student, sizeof(student), 1, file);
			printf("Хотите продолжить ввод другого студента?, 1 - да, 0 - нет ");
			input(&vibor);
			if (vibor == 0) {
				break;
			}
		}
		fclose(file);
	}
	void generate_string(char* out, char* A, char* a) {
		char word[STRLEN];
		int j;
		int letter, strl;
		for (j = 0; j < STRLEN; ++j) {
			word[j] = '\0';
		}
		letter = rand() % 26;

		word[0] = A[letter];
		strl = rand() % (STRLEN - 1) + 1;
		for (j = 1; j < strl; ++j) {
			letter = rand() % 26;
			word[j] = a[letter];
		}

		strcpy_s(out, STRLEN, word);
	}
	void generate_file() {
		int num, i, j, letter;

		char a[] = "qwertyuiopasdfghjklzxcvbnm";
		char A[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
		char sex[] = "MW";

		printf("%s", nazvanie_faila[0]);
		input(file_name, STRLEN - 1);
		fopen_s(&file, file_name, "wb");

		printf("Введите кол-во записей: ");
		input(&num);

		for (i = 0; i < num; ++i) {
			generate_string(student.F, A, a);
			generate_string(student.N, A, a);
			generate_string(student.O, A, a);
			generate_string(student.inst, A, a);
			generate_string(student.cafedra, A, a);
			generate_string(student.group, A, a);
			generate_string(student.zachetka, A, a);

			letter = rand() % 2;
			student.sex = sex[letter];

			student.bdyear.d = rand() % 31 + 1;
			student.bdyear.m = rand() % 12 + 1;
			student.bdyear.y = 2002 + rand() % 2;
			student.gryear.y = 2020 + rand() % 2;

			for (j = 0; j < (1 + rand() % 10); ++j) {
				student.S[j].count = j + 1;
				student.Snum = j + 1;
				for (int k = 0; k < (1 + rand() % 10); ++k) {
					generate_string(student.S[j].p[k].Name, A, a);
					student.S[j].p[k].mark = 2 + rand() % 4;
					student.S[j].Pnum = k + 1;
				}
			}
			fwrite(&student, sizeof(student), 1, file);
		}
		fclose(file);
		printf("\nФайл успешно сгенерирован!\n");
	}
	void print_file() {
		printf("%s", nazvanie_faila[0]);
		input(file_name, STRLEN);
		if (Error_exist(file_name) == -1) { printf("%s", Error[5]); return; }
		if (Error_zero(file_name) == -1) { printf("%s", Error[6]); return; }
		fopen_s(&file, file_name, "rb");
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			print_zapis(&student);
		}
		fclose(file);
	}
	void print_file(FILE* file) {
		fseek(file, 0, SEEK_SET);
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			print_zapis(&student);
		}
	}
	void crypt_str(char* str, HCRYPTKEY hSessionKey) {
		count = strlen(str);
		CryptEncrypt(hSessionKey, 0, true, 0, (BYTE*)str, &count, strlen(str));
	}
	void crypt_file(FILE* file, char* file_name, HCRYPTKEY hSessionKey) {
		fseek(file, 0, SEEK_SET);
		fopen_s(&file_izm, "Izm.txt", "w+b");
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			crypt_str(student.F, hSessionKey);
			crypt_str(student.N, hSessionKey);
			crypt_str(student.O, hSessionKey);
			crypt_str(student.inst, hSessionKey);
			crypt_str(student.cafedra, hSessionKey);
			crypt_str(student.group, hSessionKey);
			crypt_str(student.zachetka, hSessionKey);
			for (int i = 0; i < student.Snum; i++) {
				for (int j = 0; j < student.S[i].Pnum; j++) {
					crypt_str(student.S[i].p[j].Name, hSessionKey);
				}
			}
			fwrite(&student, sizeof(student), 1, file_izm);
		}
		fclose(file);
		fclose(file_izm);
		remove(file_name);
		rename("Izm.txt", file_name);
		printf("Файл успешно зашифрован!\n");
	}
	void decrypt_str(char* str, HCRYPTKEY hSessionKey) {
		count = strlen(str);
		CryptDecrypt(hSessionKey, 0, true, 0, (BYTE*)str, &count);
	}
	void decrypt_file(FILE* file, char* file_name, HCRYPTKEY hSessionKey) {
		fseek(file, 0, SEEK_SET);
		fopen_s(&file_izm, "Izm.txt", "w+b");
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			decrypt_str(student.F, hSessionKey);
			decrypt_str(student.N, hSessionKey);
			decrypt_str(student.O, hSessionKey);
			decrypt_str(student.inst, hSessionKey);
			decrypt_str(student.cafedra, hSessionKey);
			decrypt_str(student.group, hSessionKey);
			decrypt_str(student.zachetka, hSessionKey);
			for (int i = 0; i < student.Snum; i++) {
				for (int j = 0; j < student.S[i].Pnum; j++) {
					decrypt_str(student.S[i].p[j].Name, hSessionKey);
				}
			}
			fwrite(&student, sizeof(student), 1, file_izm);
		}
		fclose(file_izm);
		fclose(file);
		remove(file_name);
		rename("Izm.txt", file_name);
		printf("Файл успешно расшифрован!\n");
	}
};
class Anketa : public File{
private:
	Student student;
	FILE* file_izm;
	char studak[STRLEN];
	char s[STRLEN];
	char file_name[STRLEN];
	int vibor, size;
public:
	void add_zapis(FILE* file) {
		fseek(file, 0, SEEK_END);
		for (;;) {
			input(&student, file);
			fwrite(&student, sizeof(student), 1, file);
			printf("Хотите продолжить ввод другого студента?, 1 - да, 0 - нет ");
			input(&vibor);
			if (vibor == 0) {
				break;
			}
		}
		fseek(file, 0L, SEEK_SET);
	}
	void find_zapis(FILE* file, char* studak) {
	f:
		fseek(file, 0, SEEK_SET);
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			if (strcmp(student.zachetka, studak) == 0) {
				fseek(file, -(int)sizeof(student), SEEK_CUR);
				break;
			}
		}
	}
	void change_zapis(FILE* file) {
	c:
		printf("Введите ID студенческого билета");
		input(studak, STRLEN);
		find_zapis(file, studak);
		if (feof(file)) {
		c1:
			printf("Запись для изменения не найдена!\n0 - Повторить поиск\n1 - Вернуться назад");
			int n;
			input(&n);
			switch (n) {
			case 0:
				goto c;
			case 1:
				break;
			default:
				printf("Некорректный ввод! Повторите еще раз!\n");
				goto c1;
			}
		}
		else {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			print_zapis(&student);
		c2:
			printf("Что вы хотите изменить?\n");
			print_sub_menu2();
			int n;
			input(&n);
			switch (n) {
			case 1:
			name1:
				printf("\nВведите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.F, sizeof(student.F), s);
				if (Error_name(student.F) == 0) {
					printf("%s", Error[10]);
					goto name1;
				}
				break;
			case 2:
			name2:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.N, sizeof(student.N), s);
				if (Error_name(student.N) == 0) {
					printf("%s", Error[10]);
					goto name2;
				}
				break;
			case 3:
			name3:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.O, sizeof(student.O), s);
				if (Error_name(student.O) == 0) {
					printf("%s", Error[10]);
					goto name3;
				}
				break;
			case 4:
				printf("Введите новое значение: \n");
			data1:
				printf("%s ", polja_students[3]);
				printf("\nДень: ");
				input(&student.bdyear.d);
			data2:
				printf("Месяц: ");
				input(&student.bdyear.m);

				if (Error_data_month(student.bdyear.m) == 1) {
					printf("%s", Error[1]);
					goto data2;
				}

				printf("Год: ");
				input(&student.bdyear.y);

				if (Error_data_day(student.bdyear.m, student.bdyear.d, student.bdyear.y) == 1) {
					printf("%s", Error[2]);
					goto data1;
				}
				break;
			case 5:
			a:
				printf("Введите новое значение: ");
				input(&student.gryear.y);
				if ((student.gryear.y - student.bdyear.y) < 17) {
					printf("%s", Error[0]);
					goto a;
				}
				break;
			case 6:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.inst, sizeof(student.inst), s);
				break;
			case 7:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.cafedra, sizeof(student.cafedra), s);
				break;
			case 8:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.group, sizeof(student.group), s);
				break;
			case 9:
			zach1:
				printf("Введите новое значение: ");
				input(s, STRLEN);
				strcpy_s(student.zachetka, sizeof(student.zachetka), s);
				if (Error_zachetka(file, student.zachetka) == 1) {
					printf("%s", Error[7]);
					goto zach1;
				}
				break;
			case 10:
			sex1:
				printf("Введите новое значение: ");
				printf("%s", polja_students[9]);
				input(&student.sex);
				if (Error_sex(student.sex) == 1) {
					printf("%s", Error[11]);
					goto sex1;
				}
				break;
			case 11:
			sem1:
				printf("Введите кол-во семестров: ");
				input(&student.Snum);
				if (Error_semestr_num(student.Snum) == 1) {
					printf("%s", Error[8]);
					goto sem1;
				}
				for (int i = 0; i < student.Snum; ++i) {
					student.S[i].count = i + 1;
					// Предметы 
					printf("Ввод %d семестра\n", i + 1);
				sem2:
					printf("%s: ", polja_semestr[0]);
					input(&student.S[i].Pnum);
					if (Error_pred_num(student.S[i].Pnum) == 1) {
						printf("%s", Error[9]);
						goto sem2;
					}
					for (int j = 0; j < student.S[i].Pnum; j++) {
					sem3:
						printf("%s ", polja_predmet[0]);
						input(student.S[i].p[j].Name, STRLEN - 1);
						if (Error_repeat_pred(student.S[i], j) == 1) {
							printf("%s", Error[3]);
							goto sem3;
						}
					sem4:
						printf("%s: ", polja_predmet[1]);
						input(&student.S[i].p[j].mark);
						if (Error_mark(student.S[i].p[j].mark)) {
							printf("%s", Error[4]);
							goto sem4;
						}
					}
				}
			case 12: break;
			default:
				printf("Некорректный ввод! Повторите еще раз!\n");
				goto c2;
			}
			printf("Новая запись:\n");
			print_zapis(&student);
		c3:
			printf("0 - Сохранить\n1 - Изменить\n");
			input(&n);
			switch (n)
			{
			case 0:
				fseek(file, -(int)sizeof(student), SEEK_CUR);
				fwrite(&student, sizeof(student), 1, file);
				break;
			case 1:
				goto c2;
			default:
				printf("Некорректный ввод! Повторите еще раз!\n");
				goto c3;
			}

		}
	}
	void delete_zapis(FILE* file, char* file_name) {
	c:
		printf("Введите ID студенческого билета: ");
		input(studak, STRLEN);
		find_zapis(file, studak);
		if (feof(file)) {
		c1:
			printf("Запись для изменения не найдена!\n0 - Повторить поиск\n1 - Вернуться назад");
			int n;
			input(&n);
			switch (n) {
			case 0:
				goto c;
			case 1:
				break;
			default:
				printf("Некорректный ввод! Повторите еще раз!\n");
				goto c1;
			}
		}
		else {
			fseek(file, 0L, SEEK_SET);
			fopen_s(&file_izm, "Izm.txt", "w+b");
			while (1) {
				fread_s(&student, sizeof(student), sizeof(student), 1, file);
				if (feof(file)) break;
				if (strcmp(student.zachetka, studak) != 0) {
					fwrite(&student, sizeof(student), 1, file_izm);
				}
			}
			fclose(file);
			fclose(file_izm);
			remove(file_name);
			rename("Izm.txt", file_name);
		}
		printf("\nЗапись успешно удалена!\n");
	}
	void file_perebor(int size, char* file_name) {
		if (Error_zero(file_name) == -1) {
			printf("В %s нет подходящих данных\n", file_name);
			printf("==========\n");
			return;
		}
		FILE* file;
		FILE* file_buf;
		int i = 0;
		char** buf_stud = new char* [size];
		double* buf_srznach = new double[size];
		fopen_s(&file, file_name, "r+b");
		while (1) {
			int srznach_count = 0, srznach_sum = 0;
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			buf_stud[i] = new char[20];
			strcpy_s(buf_stud[i], 20, student.zachetka);
			for (int j = 0; j < student.Snum; j++) {
				for (int k = 0; k < student.S[j].Pnum; k++) {
					srznach_sum += student.S[j].p[k].mark;
				}
				srznach_count += student.S[j].Pnum;
			}
			buf_srznach[i] = (double)srznach_sum / srznach_count;
			i++;
		}
		double temp_num;
		char temp_zach[20];
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size - 1; k++) {
				if (buf_srznach[k] < buf_srznach[k + 1]) {
					temp_num = buf_srznach[k];
					buf_srznach[k] = buf_srznach[k + 1];
					buf_srznach[k + 1] = temp_num;
					strcpy_s(temp_zach, 20, buf_stud[k]);
					strcpy_s(buf_stud[k], 20, buf_stud[k + 1]);
					strcpy_s(buf_stud[k + 1], 20, temp_zach);
				}
			}
		}
		i = 0;
		fseek(file, 0, SEEK_SET);
		fopen_s(&file_buf, "Izm.txt", "w+b");
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (strcmp(student.zachetka, buf_stud[i]) == 0) {
				fwrite(&student, sizeof(student), 1, file_buf);
				fseek(file, 0, SEEK_SET);
				i++;
			}
			if (i == size) {
				break;
			}
		}
		fclose(file);
		fclose(file_buf);
		delete[] buf_stud;
		delete[] buf_srznach;
		remove(file_name);
		rename("Izm.txt", file_name);
		fopen_s(&file, file_name, "r+b");
		print_file(file);
		fclose(file);
	}
	void solve_ex() {
		int size = 0, year, size_1 = 0, size_2 = 0;
		char sex, file_name_pod[STRLEN] = "Student_Pod.txt", file_name_ost[STRLEN] = "Student_Ost.txt";
		FILE* file;
		FILE* file_1;
		FILE* file_2;
		Student student;
	f1:
		printf("%s", nazvanie_faila[0]);
		input(file_name, STRLEN - 1);
		if (Error_exist(file_name) == -1) {
			printf("%s", Error[5]);
			printf("0 - Исправить название файла\n 1 - Выйти в главное меню");
			input(&vibor);
			switch (vibor) {
			case 0:
				goto f1;
			case 1:
				return;
			}
		}
		if (Error_zero(file_name) == -1) {
			printf("%s", Error[6]);
			printf("0 - Исправить название файла\n 1 - Выйти в главное меню");
			input(&vibor);
			switch (vibor) {
			case 0:
				goto f1;
			case 1:
				return;
			}
		}
	a1:
		printf("%s", polja_students[9]);
		input(&sex);
		if (Error_sex(sex) == 1) {
			printf("%s", Error[11]);
			goto a1;
		}
		printf("%s", polja_students[4]);
		input(&year);
		fopen_s(&file, file_name, "r+b");
		fopen_s(&file_1, file_name_pod, "w+b");
		fopen_s(&file_2, file_name_ost, "w+b");
		while (1) {
			fread_s(&student, sizeof(student), sizeof(student), 1, file);
			if (feof(file)) break;
			if (student.sex == sex && student.gryear.y == year) {
				fwrite(&student, sizeof(student), 1, file_1);
				size_1++;
			}
			else {
				fwrite(&student, sizeof(student), 1, file_2);
				size_2++;
			}
		}
		fclose(file);
		fclose(file_1);
		fclose(file_2);
		printf("\nВывод подходящих анкет!\n");
		file_perebor(size_1, file_name_pod);
		printf("\nВывод остальных анкет!\n");
		file_perebor(size_2, file_name_ost);
	}
};
class menu : public File{
private: 
	Anketa ank;
	FILE* file;
	int vibor;
	char file_name[STRLEN];
	DWORD dwLen;
	HCRYPTKEY hKey;
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
public:
	void main_menu() {
	a1:
		print_main_menu();
		int c;
		input(&c);
		switch (c)
		{
		case 1:
			create_new_file();
			break;
		case 2:
			generate_file();
			break;
		case 3:
			sub_menu();
			break;
		case 4:
			print_file();
			break;
		case 5:
			ank.solve_ex();
			break;
		case 6:
			return;
		default:
			break;
		}
		goto a1;
	}
	void sub_menu() {
		char szPassword[] = "123456\0";
		printf("%s", nazvanie_faila[0]);
		input(file_name, STRLEN - 1);
		if (Error_exist(file_name) == -1) {
			printf("%s", Error[5]);
			return;
		}
		if (Error_zero(file_name) == -1) {
			printf("%s", Error[6]);
			return;
		}
		fopen_s(&file, file_name, "r+b");
		dwLen = (DWORD)strlen(szPassword);
		CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
		CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
		CryptHashData(hHash, (BYTE*)szPassword, dwLen, 0);
		CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey);
	a2:
		print_sub_menu();
		input(&vibor);

		switch (vibor) {
		case 1:
			print_file(file);
			goto a2;
		case 2:
			ank.add_zapis(file);
			goto a2;
		case 3:
			ank.change_zapis(file);
			goto a2;
		case 4:
			ank.delete_zapis(file, file_name);
			fopen_s(&file, file_name, "r+b");
			goto a2;
		case 5:
			crypt_file(file, file_name, hKey);
			fopen_s(&file, file_name, "r+b");
			goto a2;
		case 6:
			decrypt_file(file, file_name, hKey);
			fopen_s(&file, file_name, "r+b");
			goto a2;
		case 7:
			goto a3;
		default:
			goto a2;
		}
	a3:
		fclose(file);
	}
};

int main() {
	menu m;
	setlocale(0, "");
	m.main_menu();
}
