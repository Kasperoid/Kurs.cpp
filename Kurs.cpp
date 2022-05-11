#include <iostream>
#include <string.h>
#include <Windows.h>
#include <io.h>
#include <regex>

#define STRLEN 20

using namespace std;

const char* main_menu[] = {
	"1 - Создание нового файла",
	"2 - Генерация нового файла",
	"3 - Загрузка существующего файла",
	"4 - Печать файла",
	"5 - Выйти из программы",
	"6 - Реализация варианта"
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
	"\nПол (Мужчина - M, Женщина - W) ",
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

// Проверки
int Error_exist(char* file_name) {
	return _access(file_name, 0); // 0 - если верно, -1 - если не верно
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
int Error_name(char* name) {
	regex regName("[A-Z][a-z]+");
	int flag;
	flag = regex_match(name, regName);
	return flag;
}
int Error_data_month(int month) {
	int flag = 0;
	if (month < 1 || month > 12) {
		flag = 1;
	}
	return flag;
}
int Error_data_day(int month, int day, int year) {
	int flag = 0;
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
	int flag = 0;
	if (sex != 'W' && sex != 'M') {
		flag = 1;
	}
	return flag;
}
int Error_semestr_num(int sem_num) {
	int flag = 0;
	if (sem_num < 1 || sem_num > 9) {
		flag = 1;
	}
	return flag;
}
int Error_pred_num(int pred_num) {
	int flag = 0;
	if (pred_num < 1 || pred_num > 10) {
		flag = 1;
	}
	return flag;
}
int Error_repeat_pred(Semestr* sem, int count_predmet) {
	int flag = 0;
	for (int k = 0; k < count_predmet; k++) {
		if (strcmp(sem->p[k].Name, sem->p[count_predmet].Name) == 0) {
			flag = 1;
		}
	}
	return flag;
}
int Error_repeat_pred(Semestr sem, int count_predmet) {
	int flag = 0;
	for (int k = 0; k < count_predmet; k++) {
		if (strcmp(sem.p[k].Name, sem.p[count_predmet].Name) == 0) {
			flag = 1;
		}
	}
	return flag;
}
int Error_mark(int mark) {
	int flag = 0;
	if (mark < 2 || mark > 5) {
		flag = 1;
	}
	return flag;
}

// Вывод 
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
	for (int i = 0; i < 6; ++i) {
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

// Очистка буфера
void clear_buf() { // ????
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
	name1:
	printf("%s ", polja_students[0]);
	input_text((*student).F, STRLEN - 1);
	if (Error_name((*student).F) == 0) {
		printf("%s", Error[10]);
		goto name1;
	}

	// Имя
	name2:
	printf("%s ", polja_students[1]);
	input_text((*student).N, STRLEN - 1);
	if (Error_name((*student).N) == 0) {
		printf("%s", Error[10]);
		goto name2;
	}

	// Отчество
	name3:
	printf("%s ", polja_students[2]);
	input_text((*student).O, STRLEN - 1);
	if (Error_name((*student).O) == 0) {
		printf("%s", Error[10]);
		goto name3;
	}

	// Дата рождения
data1:
	printf("%s ", polja_students[3]);
	printf("\nДень: ");
	input_decimal(&(*student).bdyear.d);
data2:
	printf("Месяц: ");
	input_decimal(&(*student).bdyear.m);

	if (Error_data_month((*student).bdyear.m) == 1) { 
		printf("%s", Error[1]); 
		goto data2; 
	}

	printf("Год: ");
	input_decimal(&(*student).bdyear.y);

	if (Error_data_day((*student).bdyear.m, (*student).bdyear.d, (*student).bdyear.y) == 1) {
		printf("%s", Error[2]);
		goto data1;
	}

	// Год поступления
	data3: 
	printf("%s", polja_students[4]);
	input_decimal(&(*student).gryear.y);
	if (((*student).gryear.y - (*student).bdyear.y) < 17) {
		printf("%s", Error[0]);
		data4:
		printf("0 - Изменить дату рождения\n1 - Изменить год поступления\n");
		input_decimal(&vibor);
		switch (vibor) {
		case 0: goto data2; break;
		case 1: goto data3; break;
		default: printf("Введите корректное значение!"); goto data4;
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
	stud1:
	printf("%s", polja_students[8]);
	input_text((*student).zachetka, STRLEN - 1);
	if (Error_zachetka(file, (*student).zachetka) == 1) {
		printf("%s", Error[7]);
		goto stud1;
	}

	// Пол
	sex1:
	printf("%s", polja_students[9]);
	input_char(&(*student).sex);
	if (Error_sex((*student).sex) == 1) {
		printf("%s", Error[11]);
		goto sex1;
	}

	// Семестры
	sem1:
	printf("Введите кол-во семестров: ");
	input_decimal(&(*student).Snum);
	if (Error_semestr_num((*student).Snum) == 1) { 
		printf("%s", Error[8]); 
		goto sem1; 
	}
	for (int i = 0; i < (*student).Snum; ++i) {
		(*student).S[i].count = i + 1;
		// Предметы 
		printf("Ввод %d семестра\n", i + 1);
		sem2:
		printf("%s: ", polja_semestr[0]);
		input_decimal(&(*student).S[i].Pnum);
		if (Error_pred_num((*student).S[i].Pnum) == 1) { 
			printf("%s", Error[9]); 
			goto sem2; 
		}
		for (int j = 0; j < (*student).S[i].Pnum; j++) {
			sem3:
			printf("%s ", polja_predmet[0]);
			input_text((*student).S[i].p[j].Name, STRLEN - 1);
			if (Error_repeat_pred(&student->S[i], j) == 1) {
				printf("%s", Error[3]);
				goto sem3;
			}
		sem4:
			printf("%s: ", polja_predmet[1]);
			input_decimal(&(*student).S[i].p[j].mark);
			if (Error_mark((*student).S[i].p[j].mark)) {
				printf("%s", Error[4]); 
				goto sem4; 
			}
		}
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
			name1:
			printf("\nВведите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.F, sizeof(student.F), s);
			if (Error_name(student.F) == 0) {
				printf("%s", Error[10]);
				goto name1;
			}
			break;
		case 2:
			name2:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
			strcpy_s(student.N, sizeof(student.N), s);
			if (Error_name(student.N) == 0) {
				printf("%s", Error[10]);
				goto name2;
			}
			break;
		case 3:
			name3:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
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
			input_decimal(&student.bdyear.d);
		data2:
			printf("Месяц: ");
			input_decimal(&student.bdyear.m);

			if (Error_data_month(student.bdyear.m) == 1) {
				printf("%s", Error[1]);
				goto data2;
			}

			printf("Год: ");
			input_decimal(&student.bdyear.y);

			if (Error_data_day(student.bdyear.m, student.bdyear.d, student.bdyear.y) == 1) {
				printf("%s", Error[2]);
				goto data1;
			}
			break;
		case 5:
			a:
			printf("Введите новое значение: ");
			input_decimal(&student.gryear.y);
			if ((student.gryear.y - student.bdyear.y) < 17) { 
				printf("%s", Error[0]); 
				goto a; 
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
			zach1:
			printf("Введите новое значение: ");
			input_text(s, STRLEN);
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
			input_char(&student.sex);
			if (Error_sex(student.sex) == 1) {
				printf("%s", Error[11]);
				goto sex1;
			}
			break;
		case 11: 
		sem1:
			printf("Введите кол-во семестров: ");
			input_decimal(&student.Snum);
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
				input_decimal(&student.S[i].Pnum);
				if (Error_pred_num(student.S[i].Pnum) == 1) {
					printf("%s", Error[9]);
					goto sem2;
				}
				for (int j = 0; j < student.S[i].Pnum; j++) {
				sem3:
					printf("%s ", polja_predmet[0]);
					input_text(student.S[i].p[j].Name, STRLEN - 1);
					if (Error_repeat_pred(student.S[i], j) == 1) {
						printf("%s", Error[3]);
						goto sem3;
					}
				sem4:
					printf("%s: ", polja_predmet[1]);
					input_decimal(&student.S[i].p[j].mark);
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
void file_perebor(int size, char* file_name) {
	Student student;
	FILE* file;
	FILE* file_buf;
	int i = 0;
	char** buf_stud = new char* [size]; //Массив для зачёток
	double* buf_srznach = new double[size]; //Массив для среднего значения оценок всех
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
		buf_srznach[i] = (double) srznach_sum / srznach_count;
		i++;
	}
	double temp_num;
	char temp_zach[20];
	for (int j = 1; j < size; ++j) {
		for (int k = 0; k < size - j; k++) {
			if (buf_srznach[k] < buf_srznach[k + 1]) {
				temp_num = buf_srznach[j];
				buf_srznach[j] = buf_srznach[j + 1];
				buf_srznach[j + 1] = temp_num;
				strcpy_s(temp_zach, 20 ,buf_stud[j]);
				strcpy_s(buf_stud[j], 20 ,buf_stud[j + 1]);
				strcpy_s(buf_stud[j + 1], 20 ,temp_zach);
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
	copy_file(file_name);
}
void solve_ex() {
	int size = 0, year, size_1 = 0, size_2 = 0;
	char file_name[STRLEN], sex, file_name_pod[STRLEN], file_name_ost[STRLEN];
	FILE* file;
	FILE* file_1;
	FILE* file_2;
	Student student;
	strcpy_s(file_name_pod, 20 ,"Student_Pod.txt");
	strcpy_s(file_name_ost, 20 ,"Student_Ost.txt");
	printf("%s", nazvanie_faila[0]);
	input_text(file_name, STRLEN - 1);
	if (Error_exist(file_name) == -1) { 
		printf("%s", Error[5]); 
		return; 
	}
	if (Error_zero(file_name) == -1) { 
		printf("%s", Error[6]); 
		return; 
	}
	a1:
	printf("Введите:");
	printf("%s", polja_students[9]);
	input_char(&sex);
	if (Error_sex(sex) == 1) {
		printf("%s", Error[11]);
		goto a1;
	}
	printf("%s", polja_students[4]);
	input_decimal(&year);
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
	file_perebor(size_1, file_name_pod);
	file_perebor(size_2, file_name_ost);
	fopen_s(&file_1, file_name_pod, "r+b");
	fopen_s(&file_2, file_name_ost, "r+b");
	print_file(file_1);
	printf("==============\n");
	print_file(file_2);
	fclose(file_1);
	fclose(file_2);
} // Довести до ума
void load_file() {
	char file_name[STRLEN];
	int vibor;
	FILE* file;
	printf("%s", nazvanie_faila[0]);
	input_text(file_name, STRLEN - 1);
	if (Error_exist(file_name) == -1) { 
		printf("%s", Error[5]); 
		return; 
	}
	if (Error_zero(file_name) == -1) { 
		printf("%s", Error[6]); 
		return; 
	}
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
		/*encrypt();*/
		break;
	case 6:
		/*decrypt();*/
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
	case 6: 
		solve_ex();
		break;
	default:
		break;
	}
	goto a1;
}
