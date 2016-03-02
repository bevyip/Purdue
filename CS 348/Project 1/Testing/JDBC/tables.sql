create table Department(deptid integer, dname varchar(30), location varchar(30), primary key(deptid));

create table Faculty(fid integer, fname varchar(30), deptid integer, primary key(fid), foreign key(deptid) references Department(deptid));

create table Student(snum integer, sname varchar(30), deptid integer, slevel varchar(10), age integer, primary key(snum), foreign key (deptid) references Department(deptid));

create table Class(cname varchar(30), meets_at date, room varchar(10), fid integer, primary key(cname));

create table Enrolled(snum integer, cname varchar(30), grade varchar(2), primary key(snum, cname), foreign key(snum) references Student(snum), foreign key(cname) references Class(cname));
