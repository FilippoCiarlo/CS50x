import sys

from pyfiglet import Figlet


# Manage a bad exit
def bad_exit():
    print("Invalid usage")
    sys.exit(1)


# Figlet object
figlet = Figlet()

# Check command-line arguments
argc = len(sys.argv)

if argc != 3 and argc != 1:
    bad_exit()
elif argc == 3:
    opt = sys.argv[1]
    font = sys.argv[2]

    # Check the option argument
    if opt not in ["-f", "--font"]:
        bad_exit()

    # Set the font
    if str(font) in figlet.getFonts():
        figlet.setFont(font=font)
    else:
        bad_exit()

# Prompt
str = input("Input: ")
print("Output:\n", figlet.renderText(str))
