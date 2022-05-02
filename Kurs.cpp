#include <iostream>
#include <string.h>
#include <Windows.h>

#define STRLEN 20

using namespace std;

const char* main_menu[] = {
	"1 - Создание нового файла",
	"2 - Генерация нового файла",
	"3 - Загрузка существующего файла",
	"4 - Печать файла",
	"5 - Выйти из программы"
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
	"5 - Дата поступления ",
	"6 - Институт ",
	"7 - Кафедра ",
	"8 - Группа ",
	"9 - Номер зачётки ",
	"10 - Пол ",
	"12 - Вернуться в меню"
};
const char* nazvanie_faila[] = { "\nУкажите название файла " };
const char* polja_students[] = {
	"Фамилия ",
	"Имя ",
	"Отчество ",
	"Дата рождения ",
	"Дата поступления ",
	"Институт ",
	"Кафедра ",
	"Группа ",
	"Номер зачетки ",
	"Пол ",
	"Предмет "
};
const char* polja_semestr[] = {
	"Количество предметов в семестре "
};
const char* polja_predmet[] = {
	"Название предмета ",
	"Оценка "
};
const char* m4[] = { "\nОшибка: дата рождения не соответствует году поступления",
					 "\nОшибка: ввод номера месяца",
					 "\nОшибка: ввод дня месяца",
					 "\nОшибка: название предмета повторяется в данной сессии",
					 "\nОшибка: ввод оценки",
					 "\nОшибка: данный файл отсутствует в системе",
					 "\nОшибка: такой номер зачетки уже вводился" };

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

class PrintStr {};
class Error : public virtual PrintStr {
public:
	void printerror(const char* z[]) { cout << z; }
};
class Input {
public:
	void In(char* S) { scanf_s("%s", S, 20); }
	int In(int k) {};
	int In(Date* t) {};
	int In(Predmet* p) {};
};
class FileWork :public Error, public Input
{
	FILE* t;
	char NameFile[20];
	char type[3];
public:
	void GetFileName() { printf("%s", nazvanie_faila[0]); In(NameFile); }
	void OpenFile() {
		if (!fopen_s(&t, NameFile, type)) return;
		else printerror(&m4[5]);
	}

};

class St : public FileWork, virtual PrintStr {
	Student x;
public: St() {};
};

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
	int num, strl, i, j, letter;
	int day, month, yearBD, yearGR;
	char file_name[STRLEN];

	char a[] = "qwertyuiopasdfghjklzxcvbnm";
	char A[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
	char sex[] = "MW";
	char word[STRLEN];

	printf("Введите кол-во записей: ");
	scanf_s("%d", &num);

	FILE* file;
	printf("%s", nazvanie_faila[0]);
	scanf_s("%s", file_name, STRLEN - 1);
	fopen_s(&file, file_name, "wb");

	Student student;

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

		day = rand() % 31 + 1;
		month = rand() % 12 + 1;
		yearBD = 2002 + rand() % 2;
		yearGR = 2020 + rand() % 2;

		student.bdyear.d = day;
		student.bdyear.m = month;
		student.bdyear.y = yearBD;
		student.gryear.y = yearGR;

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
}

void print_file() { // Для главного меню
	char file_name[STRLEN];
	Student student;
	FILE* file;
	printf("%s", nazvanie_faila[0]);
	scanf_s("%s", file_name, STRLEN - 1);
	fopen_s(&file, file_name, "rb");
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		printf("ФИО: %s %s %s Пол: %c\n", student.F, student.N, student.O, student.sex);
		printf("Инст: %s Каф: %s\n", student.inst, student.cafedra);
		printf("Группа: %s Студак: %s\n", student.group, student.zachetka);
		printf("Дата рождения: %02d.%02d.%d\n", student.bdyear.d, student.bdyear.m, student.bdyear.y);
		printf("Дата поступления: %d\n", student.gryear.y);
		for (int i = 0; i < student.Snum; ++i) {
			printf("Cеместр %d :\n", student.S[i].count);
			for (int j = 0; j < student.S[i].Pnum; ++j) {
				printf("Предмет >> Оценка: %s >> %d\n", student.S[i].p[j].Name, student.S[i].p[j].mark);
			}
		}
	}
	fclose(file);
}
void print_file(char* file_name) { // Файл мы уже открыли, он просто для вывода этого файла
	FILE* file;
	Student student;
	fopen_s(&file, file_name, "r+b");
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		printf("ФИО: %s %s %s Пол: %c\n", student.F, student.N, student.O, student.sex);
		printf("Инст: %s Каф: %s\n", student.inst, student.cafedra);
		printf("Группа: %s Студак: %s\n", student.group, student.zachetka);
		printf("Дата рождения: %02d.%02d.%d\n", student.bdyear.d, student.bdyear.m, student.bdyear.y);
		printf("Дата поступления: %d\n", student.gryear.y);
		for (int i = 0; i < student.Snum; ++i) {
			printf("Cеместр %d :\n", student.S[i].count);
			for (int j = 0; j < student.S[i].Pnum; ++j) {
				printf("Предмет >> Оценка: %s >> %d\n", student.S[i].p[j].Name, student.S[i].p[j].mark);
			}
		}
	}
	fclose(file);
}
void print_main_menu() {
	for (int i = 0; i < 5; ++i) {
		printf("%s\n", main_menu[i]);
	}
}
void print_sub_menu() {
	for (int i = 0; i < 7; ++i) {
		printf("%s\n", sub_menu[i]);
	}
}
void print_sub_menu2() {
	for (int i = 0; i < 11; ++i) {
		printf("%s\n", sub_menu2[i]);
	}
}
void clear_buf() {
	while (cin.get() != '\n');
}

// Ввод данных
void input_text(char* dest, int str_len) {
	scanf_s("%s", dest, str_len);
	clear_buf();
}
void input_decimal(int* dest) {
	scanf_s("%d", dest);
	clear_buf();
}
void input_char(char* dest) {
	scanf_s("%c", dest);
	clear_buf();
}

void input_zapis(Student* student) {
	int semnum, prednum;

	// Фамилия
	printf("%s ", polja_students[0]);
	input_text((*student).F, STRLEN - 1);

	// Имя
	printf("%s ", polja_students[1]);
	input_text((*student).N, STRLEN - 1);

	// Отчество
	printf("%s ", polja_students[2]);
	input_text((*student).O, STRLEN - 1);

	// Дата рождения
	printf("%s ", polja_students[3]);
	printf("\nДень: ");
	input_decimal(&(*student).bdyear.d);
	printf("Месяц: ");
	input_decimal(&(*student).bdyear.m);
	printf("Год: ");
	input_decimal(&(*student).bdyear.y);

	// Год поступления
	printf("%s", polja_students[4]);
	input_decimal(&(*student).gryear.y);

	// Институт
	printf("%s", polja_students[5]);
	input_text((*student).inst, STRLEN - 1);

	// Кафедра
	printf("%s", polja_students[6]);
	input_text((*student).cafedra, STRLEN - 1);

	// Группа
	printf("%s", polja_students[7]);
	input_text((*student).group, STRLEN - 1);

	// Зачетка
	printf("%s", polja_students[8]);
	input_text((*student).zachetka, STRLEN - 1);

	// Пол
	printf("%s", polja_students[9]);
	input_char(&(*student).sex);

	// Семестры
	printf("Введите кол-во семестров: ");
	input_decimal(&semnum);
	(*student).Snum = semnum;
	for (int i = 0; i < semnum; ++i) {
		(*student).S[i].count = i + 1;
		printf("Ввод %d семестра\n", i + 1);

		// Предметы
		printf("%s: ", polja_semestr[0]);
		input_decimal(&prednum);
		(*student).S[i].Pnum = prednum;
		for (int j = 0; j < prednum; j++) {
			printf("%s ", polja_predmet[0]);
			input_text((*student).S[i].p[j].Name, STRLEN - 1);
			printf("%s: ", polja_predmet[1]);
			input_decimal(&(*student).S[i].p[j].mark);
		}
	}
}
void add_zapis(char* file_name) {
	FILE* file;
	Student student;
	int vibor;
	fopen_s(&file, file_name, "ab");
	for (;;) {
		input_zapis(&student);
		fwrite(&student, sizeof(student), 1, file);
		printf("Хотите продолжить ввод другого студента?, 1 - да, 0 - нет ");
		input_decimal(&vibor);
		if (vibor == 0) {
			break;
		}
	}
	fclose(file);
}
void copy_file(char* file_name) {
	Student student;
	FILE* file;
	FILE* file_izm;
	fopen_s(&file, file_name, "w+b");
	fopen_s(&file_izm, "Izm.txt", "r+b");
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file_izm);
		if (feof(file_izm)) break;
		fwrite(&student, sizeof(student), 1, file);
	}
	fclose(file);
	fclose(file_izm);
}
void change_fam(int count, char* file_name) {
	int i = 0;
	FILE* file;
	FILE* file_izm;
	Student student;
	fopen_s(&file, file_name, "r+b");
	fopen_s(&file_izm, "Izm.txt", "w+b");
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		if (i == (count - 1)) {
			printf("%s", polja_students[0]);
			input_text(student.F, STRLEN - 1);
		}
		fwrite(&student, sizeof(student), 1, file_izm);
		i++;
	}
	fclose(file);
	fclose(file_izm);
	copy_file(file_name);
}
void change_zapis(char* file_name) {
	FILE* file;
	Student student;
	int vibor, count = 0;
	char zachetka[STRLEN];
	fopen_s(&file, file_name, "r+b");
	printf("%s", polja_students[8]);
	input_text(zachetka, STRLEN - 1);
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		count++;
		if (student.zachetka == zachetka) break;
	}
	fclose(file);
	a3:
	printf("Что изменить:\n");
	print_sub_menu2();
	input_decimal(&vibor);
	switch (vibor) {
	case 1: change_fam(count, file_name);
	case 12: break;
	default: goto a3;
	}
}
void delete_zapis() {}
void encrypt() {}
void decrypt() {}

void create_new_file() {
	char file_name[STRLEN];
	int vibor;
	FILE* file;
	Student student;
	printf("%s\n", main_menu[0]);
	printf("Введите название файла: ");
	input_text(file_name, STRLEN - 1);
	fopen_s(&file, file_name, "r+b");
	for (int count = 0; ; count++) {
		printf("Введите информацию о %d студенте:\n", count + 1);
		input_zapis(&student);
		fwrite(&student, sizeof(student), 1, file);
		printf("Хотите продолжить ввод другого студента?, 1 - да, 0 - нет ");
		input_decimal(&vibor);
		if (vibor == 0) {
			break;
		}
	}
	fclose(file);
}
void load_file() {
	char file_name[STRLEN];
	int vibor;
	printf("%s", nazvanie_faila[0]);
	input_text(file_name, STRLEN - 1);
a2:
	print_sub_menu();
	input_decimal(&vibor);

	switch (vibor) {
	case 1:
		print_file(file_name);
		goto a2;
	case 2:
		add_zapis(file_name);
		goto a2;
	case 3:
		change_zapis(file_name);
		goto a2;
	case 4:
		delete_zapis();
		goto a2;
	case 5:
		encrypt();
		break;
	case 6:
		decrypt();
		goto a2;
	case 7:
		break;
	default:
		goto a2;
	}

}

int main() {
	setlocale(0, "");
a1:
	print_main_menu();
	char c[2];
	input_text(c, 2);
	switch (c[0])
	{
	case '1':
		create_new_file();
		break;
	case '2':
		generate_file();
		break;
	case '3':
		load_file();
		break;
	case '4':
		print_file();
		break;
	case '5':
		return 0;
	default:
		break;
	}
	goto a1;
}
