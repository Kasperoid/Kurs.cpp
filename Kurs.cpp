#include <iostream>
#include <string.h>
#include <Windows.h>

#define STRLEN 20

using namespace std;
const char* m1[] = { "\nСоздание нового файла", "\nГенерация нового файла", "\nЗагрузка существующего файла" };
const char* m2[] = { "\nУкажите название файла " };
const char* m3[] = { "\nФамилия","\nИмя","\nОтчество",
					"\nДата рождения","\nПол","\nИнститут",
					"\nКафедра","\nНомер_зачетки","\nПредмет" };
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

struct Semestr { Predmet p[10]; int count; int Pnum; };

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
	void GetFileName() { printf("%s", m2[0]); In(NameFile); }
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
	printf("%s", m2[0]);
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

void print_file() {
	char file_name[STRLEN];
	Student student;
	FILE* file;
	printf("%s", m2[0]);
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
			for (int j = 0; j < student.S[i].Pnum; i++) {
				printf("Предмет >> Оценка: %s >> %d\n", student.S[i].p[j].Name, student.S[i].p[j].mark);
			}
		}
	}
	fclose(file);

}

int main()
{
	setlocale(0, "");
	/*SetConsoleCP(1251);
	SetConsoleOutputCP(1251);*/
	generate_file();
	print_file();
}
