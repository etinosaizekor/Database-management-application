-- drop database universityDatabase;
 -- create database universityDatabase;
 
 
 use universityDatabase;
 create table if not exists faculty(
	name varchar(40),
    primary key(name)	
 );
 
create table if not exists department(
	dept_name varchar(40),
    building varchar(10),
    budget int,
    faculty varchar(40),
    primary key(dept_name),
    foreign key(faculty) references faculty(name)
);

create table if not exists instructor(
	ID char(6),
    name varchar(30),
    dept_name varchar(40),
    salary numeric(8, 2),
    faculty varchar(40),
    pass varchar(9),
    primary key(ID),
    foreign key(dept_name) references department(dept_name),
    foreign key(faculty) references faculty(name)
);

create table if not exists student(
	ID char(6),
    name varchar(30),
    dept_name varchar(30),
    faculty varchar(40),
    tot_cred int,
    cgpa numeric(3, 2),
    pass varchar(9),
    primary key(ID),
    foreign key(dept_name) references department(dept_name),
    foreign key(faculty) references faculty(name)
);

create table if not exists advisor(
	s_id char(6),
    i_id char(6),
    primary key(s_id),
	foreign key(s_id) references student(ID),
    foreign key(i_id) references instructor(ID)
);

create table if not exists classroom(
	building varchar(5),
    room_number char(3),
    capacity int,
    
    constraint building_room primary key(building, room_number)	
);

create table if not exists course(
	course_id varchar(10),
    title varchar(50),
    dept_name varchar(40),
    faculty varchar(40),
    credits int check(credits >=0 and credits <= 4),
    primary key(course_id),
    foreign key(dept_name) references department(dept_name)
);

create table if not exists prereq(
	course_id varchar(10),
    prereq_id varchar(10),
    primary key(course_id, prereq_id),
    foreign key(course_id) references course(course_id),
    foreign key(prereq_id) references course(course_id)
);

create table if not exists time_slot(
	time_slot_id int,
    day varchar(10),
    start_time numeric(4, 2),
    end_time numeric(4, 2),
    primary key(time_slot_id, day, start_time)
);

create table if not exists section(
	course_id varchar(10),
    sec_id int,
    semester varchar(6),
    year varchar(9),
    building varchar(5),
    room_number char(5),
    time_slot_id int,
    primary key(course_id, sec_id, semester, year),
    foreign key(building, room_number) references classroom(building, room_number), 
    foreign key(time_slot_id) references time_slot(time_slot_id)
);

create table if not exists teaches(
	ID char(6),
    course_id varchar(10),
    sec_id int,
    semester varchar(6),
    year varchar(9),
    primary key(ID, course_id, sec_id, semester, year),
    foreign key(ID) references instructor(ID),
    foreign key(course_id, sec_id, semester, year) references section(course_id, sec_id, semester, year)
);

create table if not exists takes(
	ID char(6),
    course_id varchar(10),
    sec_id int,
    semester varchar(6),
    year varchar(6),
    grade char(2),
    primary key(ID, course_id, sec_id, semester, year),
    foreign key(course_id, sec_id, semester, year) references section(course_id, sec_id, semester, year)
);

create table if not exists faculty_coordinator(
    ID char(6),
    name varchar(30),
    pass char(6),
    primary key(ID)
);

insert ignore into faculty_coordinator values
('223144', "Afolabi Justice", '111111');

insert ignore into faculty values
("Faculty of Engineering"), ("Faculty of Law"), ("Faculty of Pharmacy"), ("Faculty of Health Science"), 
("Faculty of Economics"), ("Faculty of communication studies"), 
("Faculty of Arts"), ("Faculty of sciences");

insert ignore into department values
('Biology', 'ECZ', 9000, 'Faculty of Engineering'),
('Computer Engineering', 'AS', 100000, 'Faculty of Engineering'),
('Electrical Engineering', 'AS', 85000, 'Faculty of Engineering'),
('Finance', 'CL', 120000, 'Faculty of Economics'),
('History', 'Alt', 50000, 'Faculty of communication studies'),
('Music', 'CL', 80000, 'Faculty of Arts'),
('Physics', 'AS', 70000, 'Faculty of sciences'),
('Software Engineering', 'CL', 200000, 'Faculty of Engineering'),
('Civil Engineering', 'AS', 100000, 'Faculty of Engineering');

insert ignore into instructor values
('222223', 'Einstein', 'Physics', 95000, 'Faculty of sciences', 'ist222223'),
('121214', 'Wu', 'Finance', 90000, 'Faculty of Economics', 'ist121214'),
('323435', 'El Said', 'History', 60000, 'Faculty of communication studies', 'ist323435'),
('455657', 'Katz', 'Computer Engineering', 75000, 'Faculty of Engineering', 'ist455657'),
('902243', 'Soydan Redif', 'Computer Engineering', 80000, 'Faculty of Engineering', 'ist902243'),
('983451', 'Kim', 'Electrical Engineering', 80000, 'Faculty of Engineering', 'ist983451'),
('567123', 'Dr. Cem Kalyoncu', 'Software Engineering', 100000, 'Faculty of Engineering', 'ist567123'),
('223490', 'Zafer Erenel', 'Computer Engineering', 50000, 'Faculty of Engineering', 'ist223490'),
('167234', 'Ezgi Ulker', 'Software Engineering', 60000, 'Faculty of Engineering', 'ist167234');


insert ignore into student values
('181123', 'James Peterson', 'Computer Engineering', 'Faculty of Engineering', 20, 3.22, 'std181123'),
('187538', 'Paula Umana', 'Computer Enginefering','Faculty of Engineering', 19, 3.22, 'std187538'),
('184164', 'Myles Paul', 'Physics','Faculty of sciences', 18, 3.01, 'std184164'),
('182341', 'John Boston', 'Software Engineering', 'Faculty of Engineering', 20, 2.72, 'std182341'),
('182124', 'Peter Johnson', 'Computer Engineering','Faculty of Engineering', 19, 3.57, 'std182124'),
('192054', 'Carolina Benjamin', 'Computer Engineering','Faculty of Engineering', 19, 3.11, 'std192054'),
('199522', 'Tina Benjamin', 'Computer Engineering','Faculty of Engineering', 19,'3.12', 'std199522'),
('191554', 'Venessa Miles', 'Computer Engineering','Faculty of Engineering', 19,'3.51', 'std191554'),
('199554', 'Carolina Benjamin', 'Computer Engineering','Faculty of Engineering', 19,'3.51', 'std199554'),
('185021', 'Stella Ambrose', 'Civil Engineering','Faculty of Engineering', 22, 2.77, 'std185021'),
('184221', 'Jack Anderson', 'Biology','Faculty of Health Science', 22, 2.88, 'std184221'),
('182945', 'Denis Bolt', 'Software Engineering','Faculty of Engineering', 23, 3.32, 'std182945'),
('189231', 'Grace Anderson', 'Computer Engineering','Faculty of Engineering', 19, 2.99, 'std189231'),
('181555', 'James White', 'History', 'Faculty of communication studies',20, 2.03, 'std181555'),
('182145', 'Allison Gates', 'Finance', 'Faculty of Economics', 21, 2.79, 'std182145'),
('187341', 'Favour Malcolm', 'Computer Engineering','Faculty of Engineering', 19, 3.04, 'std187341'),
('185828', 'Alfred Wilson', 'Software Engineering','Faculty of Engineering', 22, 3.88, 'std185828'),
('184527', 'Frederick Scott', 'Software Engineering','Faculty of Engineering', 	22, 2.80, 'std184527');


insert ignore into advisor values
('181123', '455657'),
('187538', '455657'),
('184164', '222223'),
('182341', '902243'),
('182124', '902243'),
('192054', '902243'),
('199522', '567123'),
('191554', '567123'),
('199554', '567123'),
('185021', '567123'),
('184221', '567123'),
('182945', '567123'),
('189231', '167234'),
('181555', '323435'),
('182145', '121214'),
('187341', '223490'),
('185828', '223490'),
('185828', '983451'),
('184527', '983451');


insert ignore into classroom values
('CL', '313', '50'),
('AS', '212', 60),
('ECZ', '110', 70),
('CL', '119', '100'),
('AS', '105', 70),
('PHM', '122', 100),
('CL', '232', 90),
('ECZ', '221', 60);

insert ignore into course values
('COMP117', 'Computing foundation', 'Computer Engineering', 'Faculty of Engineering', 4),
('COMP124', 'Computer programming', 'Software Engineering', 'Faculty of Engineering', 4),
('COMP217', 'Data structures', 'Computer Engineering','Faculty of Engineering', 3),
('COMP218', 'Object-Oriented Programming', 'Software Engineering','Faculty of Engineering', 4),
('COMP315', 'Object Oriented Programming II', 'Software Engineering','Faculty of Engineering', 4),
('Biol140', 'Introduction to biological chemistry', 'Biology','Faculty of health sciences', 3),
('Biol215', 'Vertebrate Physiology', 'Biology','Faculty of Health sciences', 4),
('Phys212', 'Vector Calculus', 'Physics','Faculty of sciences', 4),
('Phys222', 'Electromagnetism', 'Physics','Faculty of sciences', 4),
('HST117', 'Turkish History', 'History','Faculty of communication studies', 2),
('COMP335', 'Analysis of Algorithm', 'Software Engineering','Faculty of Engineering', 3),
('MTH222', 'Calculus', 'Computer Engineering','Faculty of Engineering', 3);

insert ignore into prereq values
('COMP124', 'COMP117'),
('COMP217', 'COMP117'),
('COMP218', 'COMP217'),
('COMP218', 'COMP124'),
('COMP217', 'COMP124'),
('COMP315', 'COMP218'),
('COMP315', 'COMP124'),
('COMP335', 'COMP315'),
('Phys222', 'Phys212'),
('Biol215', 'Biol140');

insert ignore into time_slot values
(1, 'Monday', 09.00, 12.00),
(2, 'Tuesday', 15.00, 18.00),
(3, 'Wednesday', 12.00, 15.00),
(4, 'Thursday', 09.00, 12.00),
(5, 'Friday', 12.00, 15.00);

insert ignore into section values
('COMP315', 1, 'Spring', '2020', 'CL', '232', 5),
('COMP315', 2, 'Spring', '2020', 'CL', '232', 4),
('COMP117', 1, 'Fall', '2020', 'AS', '212', 1),
('COMP117', 2, 'Fall', '2020', 'AS', '212', 2),
('COMP117', 1, 'Spring', '2020', 'AS', '212', 2),
('COMP218', 1, 'Spring', '2020', 'CL', '119', 1),
('COMP218', 2, 'Spring', '2021', 'CL', '119', 3),
('COMP218', 2, 'Fall', '2021', 'CL', '119', 3),
('COMP218', 2, 'Fall', '2020', 'CL', '119', 3),
('COMP217', 1, 'Spring', '2020', 'CL', '313', 2),
('COMP217', 2, 'Spring', '2020', 'AS', '105', 5),
('COMP218', 2, 'Spring', '2020', 'CL', '119', 2),
('COMP124', 1, 'Spring', '2020', 'CL', '313', 3),
('COMP124', 2, 'Spring', '2020', 'CL', '313', 2),
('COMP124', 2, 'Fall', '2020', 'CL', '313', 2),
('COMP124', 1, 'Fall', '2020', 'CL', '313', 2),
('Biol215', 1, 'Fall', '2020', 'ECZ', '110', 3),
('Biol215', 2, 'Fall', '2020', 'ECZ', '110', 1),
('Phys222', 1, 'Spring', '2020', 'PHM', '122', 4),
('Phys222', 2, 'Fall', '2021', 'PHM', '122', 4),
('Hst117', 1, 'Fall', '2020', 'ECZ', '221', 5),
('Hst117', 1, 'Spring', '2020', 'ECZ', '221', 5),
('Hst117', 2, 'Spring', '2021', 'ECZ', '221', 5),
('COMP335', 1, 'Spring', '2021', 'ECZ', '110', 2),
('COMP335', 1, 'Fall', '2021', 'ECZ', '110', 2),
('COMP335', 1, 'Spring', '2020', 'ECZ', '110', 2),
('MTH222', 1, 'Spring', '2020', 'AS', '212', 3),
('MTH222', 1, 'Fall', '2020', 'AS', '212', 3);


insert ignore into takes values
('181123', 'COMP117', 1, 'Fall', '2020', 'A-'),
('181123', 'COMP124', 2, 'Fall', '2020', 'C+'),
('187538', 'COMP117', 1, 'Fall', '2020', 'C-'),
('187538', 'COMP124', 2, 'Fall', '2020', 'B+'),
('184164', 'Phys222', 2, 'Fall', '2021', 'B+'),
('184164', 'Biol215', 1, 'Fall', '2020', 'B-'),
('182341', 'COMP218', 2, 'Spring', '2020', 'C+'),
('182341', 'COMP124', 1, 'Fall', '2020', 'C+'),
('182124', 'COMP335', 1, 'Spring', '2021', 'C-'),
('182124', 'COMP315', 1, 'Spring', '2020', 'D+'),
('192054', 'COMP315', 1, 'Spring', '2020', 'D+'),
('192054', 'COMP335', 1, 'Spring', '2021', 'F'),
('199522', 'COMP124', 2, 'Fall', '2020', 'B'),
('199522', 'COMP117', 2, 'Fall', '2020', 'C'),
('191554', 'COMP217', 2, 'Spring', '2020', 'C'),
('191554', 'COMP124', 2, 'Fall', '2020', 'C'),
('199554', 'COMP117', 1, 'Fall', '2020', 'C'),
('199554', 'COMP124', 2, 'Fall', '2020', 'C'),
('185021', 'COMP117', 1, 'Fall', '2020', 'C'),
('185021', 'MTH222', 1, 'Fall', '2020', 'C'),
('184221', 'Hst117', 1, 'Spring', '2020', 'C'),
('184221', 'Biol215', 1, 'Fall', '2020', 'C'),
('182945', 'COMP218', 2, 'Fall', '2020', 'C'),
('182945', 'COMP117', 1, 'Fall', '2020', 'C'),
('189231', 'COMP335', 1, 'Fall', '2021', 'C'),
('189231', 'COMP218', 1, 'Spring', '2020', 'C'),
('181555', 'MTH222', 1, 'Fall', '2020', 'C'),
('181555', 'COMP117', 1, 'Spring', '2020', 'C'),
('182145', 'MTH222', 1, 'Fall', '2020', 'C'),
('182145', 'COMP117', 1, 'Spring', '2020', 'C'),
('187341', 'COMP218', 2, 'Fall', '2020', 'C'),
('187341', 'COMP117', 1, 'Spring', '2020', 'C'),
('185828', 'COMP315', 2, 'Spring', '2020', 'C'),
('185828', 'COMP218', 1, 'Spring', '2020', 'C'),
('184527', 'COMP315', 2, 'Spring', '2020', 'C'),
('184527', 'COMP217', 2, 'Spring', '2020', 'C');



insert ignore into teaches values
('222223', 'Phys222', 1, 'Spring', '2020'),
('222223', 'MTH222', 1, 'Fall', '2020'),
('121214', 'COMP117', 1, 'Spring', '2020'),
('121214', 'MTH222', 1, 'Spring', '2020'),
('323435', 'Hst117', 1, 'Fall', '2020'),
('323435', 'MTH222', 1, 'Spring', '2020'),
('455657', 'COMP335', 1, 'Fall', '2021'),
('455657', 'COMP315', 1, 'Spring', '2020'),
('902243', 'COMP124', 1, 'Fall', '2020'),
('902243', 'COMP315', 2, 'Spring', '2020'),
('983451', 'Phys222', 1, 'Spring', '2020'),
('983451', 'COMP117', 1, 'Spring', '2020'),
('567123', 'COMP217', 2, 'Spring', '2020'),
('567123', 'COMP218', 2, 'Spring', '2020'),
('223490', 'COMP335', 1, 'Spring', '2021'),
('223490', 'COMP124', 1, 'Spring', '2020'),
('167234', 'COMP218', 2, 'Fall', '2021'),
('167234', 'COMP315', 2, 'Spring', '2020');
