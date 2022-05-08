#include <iostream>
#include <string.h>
#include <Windows.h>
#include <io.h>

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
	"\nИмя ",
	"\nОтчество ",
	"\nДата рождения ",
	"\nГод поступления ",
	"\nИнститут ",
	"\nКафедра ",
	"\nГруппа ",
	"\nНомер зачетки ",
	"\nПол ",
	"\nПредмет "
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
					 "\nОшибка: кол-во предметов выходит за пределы\n"
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

int Error_exist(char* file_name) {
	return _access(file_name, 0);
}
int Error_zero(char* file_name) {
	FILE* file;
	fopen_s(&file, file_name, "rb");
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0) { // Проверка на пустой файл
		return -1;
	}
	fclose(file);
}
int Error_zachetka(FILE* file, char* studak) {
	Student student;
	int flag = 0;
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


void print_zapis(Student* student) {
	printf("ФИО: %s %s %s Пол: %c\n", (*student).F, (*student).N, (*student).O, (*student).sex);
	printf("Инст: %s Каф: %s\n", (*student).inst, (*student).cafedra);
	printf("Группа: %s Студак: %s\n", (*student).group, (*student).zachetka);
	printf("Дата рождения: %02d.%02d.%d\n", (*student).bdyear.d, (*student).bdyear.m, (*student).bdyear.y);
	printf("Дата поступления: %d\n", (*student).gryear.y);
	for (int i = 0; i < (*student).Snum; ++i) {
		printf("Cеместр %d :\n", (*student).S[i].count);
		for (int j = 0; j < (*student).S[i].Pnum; ++j) {
			printf("Предмет >> Оценка: %s >> %d\n", (*student).S[i].p[j].Name, (*student).S[i].p[j].mark);
		}
	}
}
void print_file() { // Для главного меню
	char file_name[STRLEN];
	Student student;
	FILE* file;
	printf("%s", nazvanie_faila[0]);
	scanf_s("%s", file_name, STRLEN - 1);
	if (Error_exist(file_name) == -1) { printf("%s", Error[5]); return; }
	if (Error_zero(file_name) == -1) { printf("%s", Error[6]); return; }
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
void print_file(FILE* file) { // Файл мы уже открыли, он просто для вывода этого файла
	Student student;
	fseek(file, 0, SEEK_SET);
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		print_zapis(&student);
	}
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
	for (int i = 0; i < 12; ++i) {
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
void input_zapis(Student* student, FILE* file) {
	int vibor;
	// Фамилия
	printf("%s ", polja_students[0]);
	input_text((*student).F, STRLEN - 1);

	// Имя
	printf("%s ", polja_students[1]);
	input_text((*student).N, STRLEN - 1);

	// Отчество
	printf("%s ", polja_students[2]);
	input_text((*student).O, STRLEN - 1);
	a1:
	// Дата рождения
a2:
	printf("%s ", polja_students[3]);
	printf("\nДень: ");
	input_decimal(&(*student).bdyear.d);
	printf("Месяц: ");
	input_decimal(&(*student).bdyear.m);

	if ((*student).bdyear.m <= 0 || (*student).bdyear.m > 12) { printf("%s", Error[1]); goto a2; }

	switch ((*student).bdyear.m) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12: 
		if ((*student).bdyear.d <= 0 || (*student).bdyear.d > 31) { printf("%s", Error[2]); goto a1; } break;
	case 4: case 6: case 9: case 11: 
		if ((*student).bdyear.d <= 0 || (*student).bdyear.d > 30) { printf("%s", Error[2]); goto a1; } break;
	case 2: 
		if ((*student).bdyear.d <= 0 || (*student).bdyear.d > 28) { printf("%s", Error[2]); goto a1; } break;
	default: break;
	}

	printf("Год: ");
	input_decimal(&(*student).bdyear.y);
	a7: 
	// Год поступления
	printf("%s", polja_students[4]);
	input_decimal(&(*student).gryear.y);

	if (((*student).gryear.y - (*student).bdyear.y) < 17) {
		printf("%s", Error[0]);
		a9:
		printf("0 - Изменить дату рождения\n1 - Изменить год поступления\n");
		input_decimal(&vibor);
		switch (vibor) {
		case 0: goto a2; break;
		case 1: goto a7; break;
		default: printf("Введите корректное значение!"); goto a9;
		}
	}

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
	a6:
	printf("%s", polja_students[8]);
	input_text((*student).zachetka, STRLEN - 1);
	if (Error_zachetka(file, (*student).zachetka) == 1) {
		printf("%s", Error[7]);
		goto a6;
	}

	// Пол
	printf("%s", polja_students[9]);
	input_char(&(*student).sex);

	// Семестры
	a3:
	printf("Введите кол-во семестров: ");
	input_decimal(&(*student).Snum);
	if ((*student).Snum <= 0 || (*student).Snum > 9) { printf("%s", Error[8]); goto a3; }
	for (int i = 0; i < (*student).Snum; ++i) {
		(*student).S[i].count = i + 1;
		a8:
		printf("Ввод %d семестра\n", i + 1);
		// Предметы 
		a4:
		printf("%s: ", polja_semestr[0]);
		input_decimal(&(*student).S[i].Pnum);
		if ((*student).S[i].Pnum <= 0 || (*student).S[i].Pnum > 10) { printf("%s", Error[9]); goto a4; }
		char** buffer = new char* [(*student).S[i].Pnum];
		for (int j = 0; j < (*student).S[i].Pnum; j++) {
			printf("%s ", polja_predmet[0]);
			input_text((*student).S[i].p[j].Name, STRLEN - 1);
			buffer[j] = new char[strlen((*student).S[i].p[j].Name) + 1];
			strcpy_s(buffer[j], 20, (*student).S[i].p[j].Name);
		a5:
			printf("%s: ", polja_predmet[1]);
			input_decimal(&(*student).S[i].p[j].mark);
			if ((*student).S[i].p[j].mark <= 1 || (*student).S[i].p[j].mark > 5) { printf("%s", Error[4]); goto a5; }
		}
		for (int j = 0; j < (*student).S[i].Pnum; j++) {
			for (int k = j + 1; k < (*student).S[i].Pnum; k++) {
				if (strcmp(buffer[j], buffer[k]) == 0) {
					printf("%s", Error[3]);
					delete[] buffer;
					goto a8;
				}
			}
		}
		delete[] buffer;
	}
}

// Меню для Load_File
void find_zapis(FILE* file, char* studak) {
f:
	Student student;
	fseek(file, 0, SEEK_SET);
	while (1) {
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		if (feof(file)) break;
		if (strcmp(student.zachetka, studak) == 0) {
			fseek(file, -(int)sizeof(student), SEEK_CUR);
			break;
		}
	}
	if (feof(file)) {
		printf("Запись с таким номером студенческого билета не найдена!\n");
	f1:
		printf("0 - Повторить поиск\n1 - Вернуться назад");
		int n;
		input_decimal(&n);
		switch (n) {
		case 0:
			goto f;
		case 1:
			break;
		default:
			printf("Некорректный ввод! Повторите еще раз!");
			goto f1;
		}
	}
}
void add_zapis(FILE* file) {
	Student student;
	fseek(file, 0L, SEEK_END);
	int vibor;
	for (;;) {
		input_zapis(&student, file);
		fwrite(&student, sizeof(student), 1, file);
		printf("Хотите продолжить ввод другого студента?, 1 - да, 0 - нет ");
		input_decimal(&vibor);
		if (vibor == 0) {
			break;
		}
	}
	fseek(file, 0L, SEEK_SET);
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
void change_zapis(FILE* file) {
c:
	printf("Введите ID студенческого билета: ");
	char studak[STRLEN];
	char s[STRLEN];
	input_text(studak, STRLEN);
	find_zapis(file, studak);
	if (feof(file)) {
	c1:
		printf("Запись для изменения не найдена!\n0 - Повторить поиск\n1 - Вернуться назад");
		int n;
		input_decimal(&n);
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
		Student student;
		fread_s(&student, sizeof(student), sizeof(student), 1, file);
		print_zapis(&student);
	c2:
		printf("Что вы хотите изменить?\n");
		print_sub_menu2();
		int n;
		input_decimal(&n);
		switch (n) {
		case 1:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.F, sizeof(student.F), s);
			break;
		case 2:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.N, sizeof(student.N), s);
			break;
		case 3:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.O, sizeof(student.O), s);
			break;
		case 4:
			printf("Введите новое значение: \n");
			a1:
			printf("День: ");
			input_decimal(&student.bdyear.d);
			a2:
			printf("Месяц: ");
			input_decimal(&student.bdyear.m);
			if (student.bdyear.m <= 0 || student.bdyear.m > 12) { printf("%s", Error[1]); goto a2; }

			switch (student.bdyear.m) {
			case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				if (student.bdyear.d <= 0 || student.bdyear.d > 31) { printf("%s", Error[2]); goto a1; } break;
			case 4: case 6: case 9: case 11:
				if (student.bdyear.d <= 0 || student.bdyear.d > 30) { printf("%s", Error[2]); goto a1; } break;
			case 2:
				if (student.bdyear.d <= 0 || student.bdyear.d > 28) { printf("%s", Error[2]); goto a1; } break;
			default: break;
			}
			printf("Год: ");
			input_decimal(&student.bdyear.y);
			break;
		case 5:
			a6:
			printf("Введите новое значение: ");
			input_decimal(&student.gryear.y);
			if ((student.gryear.y - student.bdyear.y) < 17) { 
				printf("%s", Error[0]); 
				goto a6; 
			}
			break;
		case 6:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.inst, sizeof(student.inst), s);
			break;
		case 7:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.cafedra, sizeof(student.cafedra), s);
			break;
		case 8:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.group, sizeof(student.group), s);
			break;
		case 9:
			a7:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.zachetka, sizeof(student.zachetka), s);
			if (Error_zachetka(file, student.zachetka) == 1) {
				printf("%s", Error[7]);
				goto a7;
			}
			break;
		case 10:
			printf("Введите новое значение: ");
			input_char(&student.sex);
			break;
		case 11: 
		a3:
			printf("Введите кол-во семестров: ");
			input_decimal(&student.Snum);
			if (student.Snum <= 0 || student.Snum > 9) { printf("%s", Error[8]); goto a3; }
			for (int i = 0; i < student.Snum; ++i) {
				student.S[i].count = i + 1;
			a8:
				printf("Ввод %d семестра\n", i + 1);
			a4:
				printf("%s: ", polja_semestr[0]);
				input_decimal(&student.S[i].Pnum);
				if (student.S[i].Pnum <= 0 || student.S[i].Pnum > 10) { printf("%s", Error[9]); goto a4; }
				char** buffer = new char* [student.S[i].Pnum];
				for (int j = 0; j < student.S[i].Pnum; j++) {
					printf("%s ", polja_predmet[0]);
					input_text(student.S[i].p[j].Name, STRLEN - 1);
					buffer[j] = new char[strlen(student.S[i].p[j].Name) + 1];
					strcpy_s(buffer[j], 20, student.S[i].p[j].Name);
					printf("%s: ", polja_predmet[1]);
				a5:
					input_decimal(&student.S[i].p[j].mark);
					if (student.S[i].p[j].mark <= 1 || student.S[i].p[j].mark > 5) { printf("%s", Error[4]); goto a5; }
				}
				for (int j = 0; j < student.S[i].Pnum; j++) {
					for (int k = j + 1; k < student.S[i].Pnum; k++) {
						if (strcmp(buffer[j], buffer[k]) == 0) {
							printf("%s", Error[3]);
							delete[] buffer;
							goto a8;
						}
					}
				}
				delete[] buffer;
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
		input_decimal(&n);
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
	char studak[STRLEN];
	input_text(studak, STRLEN);
	find_zapis(file, studak);
	if (feof(file)) {
	c1:
		printf("Запись для изменения не найдена!\n0 - Повторить поиск\n1 - Вернуться назад");
		int n;
		input_decimal(&n);
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
		FILE* file_izm;
		Student student;
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
		copy_file(file_name);
	}
}

// Меню главное 
void encrypt() {}
void decrypt() {}
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
}
void create_new_file() {
	char file_name[STRLEN];
	int vibor;
	FILE* file;
	Student student;
	printf("%s\n", main_menu[0]);
	printf("Введите название файла: ");
	input_text(file_name, STRLEN - 1);
	fopen_s(&file, file_name, "w+b");
	for (int count = 0; ; count++) {
		printf("Введите информацию о %d студенте:\n", count + 1);
		input_zapis(&student, file);
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
	FILE* file;
	printf("%s", nazvanie_faila[0]);
	input_text(file_name, STRLEN - 1);
	if (Error_exist(file_name) == -1) { printf("%s", Error[5]); return; }
	if (Error_zero(file_name) == -1) { printf("%s", Error[6]); return; }
	fopen_s(&file, file_name, "r+b");
a2:
	print_sub_menu();
	input_decimal(&vibor);

	switch (vibor) {
	case 1:
		print_file(file);
		goto a2;
	case 2:
		add_zapis(file);
		goto a2;
	case 3:
		change_zapis(file);
		goto a2;
	case 4:
		delete_zapis(file, file_name);
		fopen_s(&file, file_name, "r+b");
		goto a2;
	case 5:
		encrypt();
		break;
	case 6:
		decrypt();
		goto a2;
	case 7:
		goto a3;
	default:
		goto a2;
	}
a3:
	fclose(file);
}
 
int main() {
	setlocale(0, "");
a1:
	print_main_menu();
	int c;
	input_decimal(&c);
	switch (c)
	{
	case 1:
		create_new_file();
		break;
	case 2:
		generate_file();
		break;
	case 3:
		load_file();
		break;
	case 4:
		print_file();
		break;
	case 5:
		return 0;
	default:
		break;
	}
	goto a1;
}
