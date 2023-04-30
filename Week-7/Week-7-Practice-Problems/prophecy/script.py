import csv
from cs50 import SQL
from os import path

FILENAME = "students.csv"
DBNAME = "assignments.db"


def main():
    students = list()

    # Open CSV file
    try:
        with open(FILENAME, "r") as file:
            # Create reader
            reader = csv.DictReader(file)

            # Iterate over CSV file, saving students in a list
            for row in reader:
                students.append(row)
    except:
        print(f"The file {FILENAME} doesn't exist.")
        return 1

    # Open database
    if path.exists(DBNAME):
        db = SQL("sqlite:///assignments.db")
    else:
        print(f"The database {FILENAME} doesn't exist.")
        return 1

    # Insert data into the database
    for student in students:
        db.execute(
            "INSERT OR IGNORE INTO students (id, student_name) VALUES (?, ?)",
            student["id"],
            student["student_name"],
        )
        db.execute(
            "INSERT OR IGNORE INTO houses (house, head) VALUES (?, ?)",
            student["house"],
            student["head"],
        )
        db.execute(
            "INSERT OR IGNORE INTO assignments (student_id, house_id) VALUES (?, ?)",
            student["id"],
            student["house"],
        )

    # Compunicate, operation successfully completed
    print(f"The database {DBNAME} it's been populated")
    return 0


if __name__ == "__main__":
    main()
