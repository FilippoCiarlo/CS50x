def main():
    # Take the height in input
    valid_input = False
    while not valid_input:
        h = input("Height: ")
        if h >= "1" and h <= "8":
            h = int(h)
            valid_input = True

    # Print a double half-pyramid
    for i in range(0, h):
        n_spaces = h - (i + 1)
        n_blocks = i + 1

        # print left-half
        print(" " * n_spaces, end="")
        print("#" * n_blocks, end="")

        # space
        print("  ", end="")

        # print right-half
        print("#" * n_blocks)


if __name__ == "__main__":
    main()
