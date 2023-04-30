import csv
import sys

STRs = ["AGATC", "TTTTTTCT", "AATG", "TCTAG", "GATA", "TATC", "GAAA", "TCTG"]


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 1
    else:
        filename1 = sys.argv[1]
        filename2 = sys.argv[2]

    # Read database file into a variable
    db_file = open_file(filename1)
    reader = csv.DictReader(db_file)
    db = []
    for row in reader:
        db.append(row)
    db_file.close()

    # Read DNA sequence file into a variable
    dna_file = open_file(filename2)
    dna = dna_file.readline().strip()
    dna_file.close()

    # Find longest match of each STR in DNA sequence
    results = {}
    for subseq in STRs:
        results[subseq] = longest_match(dna, subseq)

    # Compute how many digit is a match
    if not db:
        MATCH = 0
    else:
        MATCH = len(db[0]) - 1

    # Check database for matching profiles:
    for person in db:
        match_count = 0
        # print(person)
        for subseq in STRs:
            # A match found
            if subseq in person and int(person[subseq]) == results[subseq]:
                match_count += 1
                # Match Found
                if match_count == MATCH:
                    print(person["name"])
                    return 0
    print("No match")

    return 0


def open_file(filename):
    try:
        # Try to open the file
        file = open(filename, "r")
    except OSError:
        # If an error occurs exit with error code 1
        print(f"File {filename} doesn't exist.")
        sys.exit(1)
    else:
        # Return the file pointer
        return file


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
