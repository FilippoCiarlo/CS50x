-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the crime scene description
SELECT description
FROM crime_scene_reports
WHERE month=7
    AND day=28
    AND year=2021
    AND street='Humphrey Street';
/*
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                                                                                                       description                                                                                                        |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/


-- Find the witnesses interviews
SELECT i.name,i.transcript
FROM interviews i
WHERE month=7
    AND day=28
    AND year=2021
    AND i.transcript LIKE '%thief%'
ORDER BY i.name;
/*
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/


-- Find the people who withdraw money from atm at Leggett Street the day of the theft
SELECT p.name
FROM people p
WHERE p.id IN (
        SELECT ba.person_id
        FROM atm_transactions at
            JOIN bank_accounts ba ON ba.account_number=at.account_number
        WHERE month=7
            AND day=28
            AND year=2021
            AND atm_location='Leggett Street'
            AND transaction_type='withdraw'
    )
ORDER BY p.name;
/*
+---------+
|  name   |
+---------+
| Benista |
| Brooke  |
| Bruce   |
| Diana   |
| Iman    |
| Kenny   |
| Luca    |
| Taylor  |
+---------+
*/


-- Find the people who had a call for less than a minute the day of the theft
SELECT p.name
FROM people p
WHERE p.phone_number IN (
        SELECT pc.caller
        FROM phone_calls pc
        WHERE month=7
            AND day=28
            AND year=2021
            AND duration<=60
    )
ORDER BY p.name;
/*
+---------+
|  name   |
+---------+
| Benista |
| Bruce   |
| Carina  |
| Diana   |
| Kathryn |
| Kelsey  |
| Kenny   |
| Sofia   |
| Taylor  |
+---------+
*/

-- Find the people who left the bakery parking lot the day of the theft
SELECT p.name
FROM people p
WHERE p.license_plate IN (
        SELECT bsl.license_plate
        FROM bakery_security_logs bsl
        WHERE month=7
            AND day=28
            AND year=2021
            AND hour=10
            AND minute>=15
            AND minute<=25
            AND activity='exit'
    )
ORDER BY p.name;
/*
+---------+
|  name   |
+---------+
| Barry   |
| Bruce   |
| Diana   |
| Iman    |
| Kelsey  |
| Luca    |
| Sofia   |
| Vanessa |
+---------+
*/


-- Find the earliest flights the day after the theft
SELECT f.id,f.hour,f.minute,org.city,dst.city
FROM flights f
    JOIN airports org  ON org.id=f.origin_airport_id
    JOIN airports dst  ON dst.id=f.destination_airport_id
WHERE f.month=7
    AND f.day=29
    AND f.year=2021
    AND org.city='Fiftyville'
ORDER BY f.hour, f.minute
LIMIT 1;
/*
+----+------+--------+------------+---------------+
| id | hour | minute |    city    |     city      |
+----+------+--------+------------+---------------+
| 36 | 8    | 20     | Fiftyville | New York City |
+----+------+--------+------------+---------------+
*/


-- Find the people who took the earliest flight the day after the theft
SELECT p.name
FROM people p
WHERE p.passport_number IN (
        SELECT ps.passport_number
        FROM passengers ps
        WHERE ps.flight_id IN (
                SELECT f.id
                FROM flights f
                    JOIN airports org  ON org.id=f.origin_airport_id
                WHERE f.month=7
                    AND f.day=29
                    AND f.year=2021
                    AND org.city='Fiftyville'
                ORDER BY f.hour, f.minute
                LIMIT 1
            )
    )
ORDER BY p.name;
/*
+--------+
|  name  |
+--------+
| Bruce  |
| Doris  |
| Edward |
| Kelsey |
| Kenny  |
| Luca   |
| Sofia  |
| Taylor |
+--------+
*/


-- Find the people who:
-- 1) withdraw money from atm at Leggett Street the day of the theft
-- 2) had a call for less than a minute the day of the theft
-- 3) left the bakery parking lot the day of the theft
-- 4) took the earliest flight the day after the theft
-- So since all these clues, find the thief
SELECT p.name
FROM people p
WHERE p.id IN (
        SELECT ba.person_id
        FROM atm_transactions at
            JOIN bank_accounts ba ON ba.account_number=at.account_number
        WHERE month=7
            AND day=28
            AND year=2021
            AND atm_location='Leggett Street'
            AND transaction_type='withdraw'
    )
    AND
    p.phone_number IN (
        SELECT pc.caller
        FROM phone_calls pc
        WHERE month=7
            AND day=28
            AND year=2021
            AND duration<=60
    )
    AND
    p.license_plate IN (
        SELECT bsl.license_plate
        FROM bakery_security_logs bsl
        WHERE month=7
            AND day=28
            AND year=2021
            AND hour=10
            AND minute>=15
            AND minute<=25
            AND activity='exit'
    )
    AND
    p.passport_number IN (
        SELECT ps.passport_number
        FROM passengers ps
        WHERE ps.flight_id IN (
                SELECT f.id
                FROM flights f
                    JOIN airports org  ON org.id=f.origin_airport_id
                WHERE f.month=7
                    AND f.day=29
                    AND f.year=2021
                    AND org.city='Fiftyville'
                ORDER BY f.hour, f.minute
                LIMIT 1
            )
    )
ORDER BY p.name;
/*
The thief is...
+-------+
| name  |
+-------+
| Bruce |
+-------+
*/


-- Find the thief's accomplice
SELECT p.name
FROM people p
WHERE p.phone_number IN (
        SELECT pc.receiver
        FROM phone_calls pc
        WHERE month=7
            AND day=28
            AND year=2021
            AND caller = (
                SELECT p.phone_number
                FROM people p
                WHERE p.name='Bruce'
            )
            AND duration<=60
    )
ORDER BY p.name;
/*
+-------+
| name  |
+-------+
| Robin |
+-------+
*/

