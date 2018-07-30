/****************************     a)     **************************************/
SELECT DISTINCT course_id 
FROM ScheduledIn 
WHERE room_number='2001';

/****************************     b)     **************************************/
SELECT DISTINCT course_id 
FROM ScheduledIn 
WHERE letter='C';

/****************************     c)     **************************************/
SELECT DISTINCT division
FROM ScheduledIn 
WHERE room_number IN ('L2','L3');

/****************************     d)     **************************************/
SELECT DISTINCT Sch1.course_id 
FROM ScheduledIn Sch1 
JOIN ScheduledIn Sch2 
ON Sch1.course_id = Sch2.course_id 
WHERE Sch1.room_number <> Sch2.room_number;

/****************************     e)     **************************************/
SELECT name 
FROM Department 
WHERE department_id IN 
(
	SELECT department_id 
	FROM ScheduledIn 
	WHERE room_number IN('L1','L2','L3','L4')
);

/****************************     f)     **************************************/
SELECT name 
FROM Department 
WHERE department_id NOT IN 
(
	SELECT department_id 
	FROM ScheduledIn 
	WHERE room_number IN('L1','L2')
);

/****************************     g)     **************************************/
SELECT dept.name
FROM Department AS dept
WHERE dept.department_id IN 
(
	SELECT d.department_id 
	
	FROM ( SELECT DISTINCT department_id, letter, day FROM ScheduledIn) AS d
	
	GROUP BY d.department_id
	
	HAVING COUNT(*) = (	SELECT COUNT(*) FROM Slot)
);

/****************************     h)     **************************************/
SELECT Sch1.letter, Sch1.day, COUNT(*) AS number_of_courses 
FROM ( SELECT DISTINCT course_id, letter, day FROM ScheduledIn) AS Sch1
GROUP BY Sch1.letter, Sch1.day 
ORDER BY number_of_courses;

/****************************     i)     **************************************/
SELECT Sch1.room_number, COUNT(*) as number_of_courses 
FROM( SELECT DISTINCT room_number, course_id FROM ScheduledIn) AS Sch1
GROUP BY Sch1.room_number 
ORDER BY number_of_courses DESC;

/****************************     j)     **************************************/

SELECT Sch3.letter
FROM( SELECT DISTINCT letter,course_id FROM ScheduledIn) AS Sch3
GROUP BY Sch3.letter
HAVING COUNT(Sch3.course_id) = (

SELECT MIN(Sch2.number_of_courses)
FROM
(
	SELECT Sch1.letter, COUNT(*) AS number_of_courses 
	FROM (	SELECT DISTINCT letter, course_id FROM ScheduledIn)AS Sch1
	GROUP BY Sch1.letter
) AS Sch2

);

/****************************     k)     **************************************/
SELECT DISTINCT letter, day 
FROM ScheduledIn 
WHERE course_id LIKE '__%M';

/****************************     l)     **************************************/

SELECT Dep1.name,Sl1.letter,Sl1.day
FROM Department AS Dep1 , Slot AS Sl1
WHERE NOT EXISTS
(
	SELECT *
	FROM ScheduledIn AS Sch1
	WHERE ( Sch1.department_id = Dep1.department_id AND Sch1.letter = Sl1.letter AND Sch1.day = Sl1.day)
)
ORDER BY Dep1.name;